//
// Created by ubuntu on 25-5-14.
//
#include "server.h"
#include "hash_map.h"
#include <pthread.h>

// Ensure buffer capacity
// 设置非阻塞
int set_nonblocking(int fd)
{
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1)
        return -1;
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

// 初始化socket
int create_and_bind()
{
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd < 0)
    {
        perror("socket");
        exit(1);
    }
    int opt = 1;
    // 设置地址重用
    setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(SERVER_PORT);

    if (bind(listen_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("bind");
        exit(1);
    }
    if (listen(listen_fd, SOMAXCONN) < 0)
    {
        perror("listen");
        exit(1);
    }
    return listen_fd;
}

head_buf_t *malloc_head_buf(int head_len)
{
    char *buf = (char *)malloc(sizeof(char) * head_len);
    if (!buf)
    {
        printf("malloc head buf failed\n");
        return NULL;
    }
    head_buf_t *head_buf = (head_buf_t *)malloc(sizeof(head_buf_t));
    if (!head_buf)
    {
        free(buf);
        printf("malloc head buf failed\n");
        return NULL;
    }
    head_buf->buf = buf;
    head_buf->data_len = head_len;
    head_buf->offset = 0;
    return head_buf;
}

void dealloc_head_buf(head_buf_t *head_buf)
{
    if (!head_buf)
    {
        return;
    }

    if (head_buf->buf)
    {
        free(head_buf->buf);
    }

    free(head_buf);
}

data_buf_t *malloc_data_buf(int type_id, int data_len)
{
    char *buf = (char *)malloc(sizeof(char) * data_len);
    if (!buf)
    {
        printf("malloc data buf failed\n");
        return NULL;
    }
    data_buf_t *data_buf = (data_buf_t *)malloc(sizeof(data_buf_t));
    if (!data_buf)
    {
        free(buf);
        printf("malloc data buf failed\n");
        return NULL;
    }
    data_buf->buf = buf;
    data_buf->type = type_id;
    data_buf->data_len = data_len;
    data_buf->offset = 0;
    return data_buf;
}

void dealloc_data_buf(data_buf_t *data_buf)
{
    if (!data_buf)
    {
        return;
    }

    if (data_buf->buf)
    {
        free(data_buf->buf);
    }

    free(data_buf);
}

send_node_t *malloc_send_node(int type_id, int data_len)
{
    send_node_t *send_node = (send_node_t *)malloc(sizeof(send_node_t));
    if (!send_node)
    {
        printf("malloc send_node failed\n");
        return NULL;
    }

    data_buf_t *send_buf = malloc_data_buf(type_id, data_len + HEAD_LEN);
    if (!send_buf)
    {
        free(send_node);
        printf("malloc send_node failed\n");
        return NULL;
    }

    send_node->send_buf = send_buf;
    send_node->next = NULL;

    return send_node;
}

void dealloc_send_node(send_node_t *send_node)
{
    if (!send_node)
    {
        return;
    }

    if (send_node->send_buf)
    {
        dealloc_data_buf(send_node->send_buf);
    }

    free(send_node);
}

send_que_t *malloc_send_que()
{

    send_que_t *send_que = (send_que_t *)malloc(sizeof(send_que_t));
    if (!send_que)
    {
        printf("malloc send que failed!\n");
        return NULL;
    }

    send_que->head = NULL;
    send_que->tail = NULL;
    if (pthread_mutex_init(&send_que->send_que_lock, NULL) != 0)
    {
        fprintf(stderr, "mutex init failed!\n");
        free(send_que);
        return NULL;
    }

    return send_que;
}

void dealloc_send_que(send_que_t *send_que)
{
    if (!send_que)
    {
        return;
    }

    // 1) 上锁：保证此时没有其他线程在操作队列
    if (pthread_mutex_lock(&send_que->send_que_lock) != 0)
    {
        fprintf(stderr, "failed to lock mutex in free_send_que\n");
        // 虽然锁失败，但我们仍尝试清理资源
    }

    // 2) 遍历并释放所有节点
    send_node_t *cur = send_que->head;
    while (cur)
    {
        send_node_t *next = cur->next;
        dealloc_send_node(cur); // 释放节点内存及其 buf
        cur = next;
    }
    send_que->head = NULL;

    // 3) 解锁
    if (pthread_mutex_unlock(&send_que->send_que_lock) != 0)
    {
        fprintf(stderr, "failed to unlock mutex in free_send_que\n");
    }

    // 4) 销毁互斥锁
    if (pthread_mutex_destroy(&send_que->send_que_lock) != 0)
    {
        fprintf(stderr, "failed to destroy mutex in free_send_que\n");
    }

    // 5) 释放队列结构体
    free(send_que);
}

session_t *session_create(int fd)
{
    session_t *session = (session_t *)malloc(sizeof(session_t));
    if (!session)
    {
        printf("failed to create session\n");
        return NULL;
    }
    session->fd = fd;
    session->data_buf = NULL;
    session->head_buf = malloc_head_buf(HEAD_LEN);
    session->send_que = malloc_send_que();
    session->recv_stage = NO_RECV;
    if (!(session->send_que))
    {
        printf("failed to create session send_que\n");
        free(session);
        return NULL;
    }

    return session;
}

void dealloc_session(session_t *session)
{
    if (session->head_buf)
    {
        dealloc_head_buf(session->head_buf);
    }

    if (session->data_buf)
    {
        dealloc_data_buf(session->data_buf);
    }

    if (session->send_que)
    {
        dealloc_send_que(session->send_que);
    }

    free(session);
}

void session_destroy(session_t *conn, int epoll_fd)
{
    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, conn->fd, NULL);
    close(conn->fd);
    dealloc_session(conn);
}

void enqueue_node(send_que_t *q, send_node_t *node, int epoll_fd, int fd, uint32_t evs)
{
    if (q->head == NULL)
    {
        q->head = node;
        q->tail = node;
    }
    else
    {
        q->tail->next = node;
        q->tail = node;
    }

    if (evs & EPOLLOUT)
    {
        return;
    }
    // enable EPOLLOUT on fd
    struct epoll_event ev;
    ev.events = EPOLLIN | EPOLLET | EPOLLOUT;
    ev.data.fd = fd;
    epoll_ctl(epoll_fd, EPOLL_CTL_MOD, fd, &ev);
}

ssize_t async_send(session_t *sess, int epoll_fd, uint32_t evs, size_t msg_type, size_t body_len, char *buf)
{
    if (!sess || !sess->send_que)
    {
        return -1;
    }

    send_node_t *send_node = malloc_send_node(msg_type, body_len);
    if (!send_node)
    {
        printf("send_node allocate failed!\n");
        return -1;
    }

    uint16_t net_type = htons(msg_type);
    uint16_t net_len = htons(body_len);
    memcpy(send_node->send_buf->buf, &net_type, 2);
    memcpy(send_node->send_buf->buf + 2, &net_len, 2);
    memcpy(send_node->send_buf->buf + 4, buf, body_len);
    size_t remain = send_node->send_buf->data_len - send_node->send_buf->offset;

    // 加锁，保证线程安全
    if (pthread_mutex_lock(&sess->send_que->send_que_lock) != 0)
    {
        dealloc_send_node(send_node);
        fprintf(stderr, "Failed to lock mutex in send_data\n");
        return -1;
    }

    // 队列为空就直接发送
    if (sess->send_que->head == NULL)
    {
        ssize_t total_sent = 0;
        while (send_node->send_buf->offset < send_node->send_buf->data_len)
        {
            ssize_t result = write(sess->fd, send_node->send_buf->buf + send_node->send_buf->offset,
                                   send_node->send_buf->data_len - send_node->send_buf->offset);
            if (result > 0)
            {
                send_node->send_buf->offset += result;
                total_sent += result;
                continue;
            }

            if (errno == EAGAIN || errno == EWOULDBLOCK)
                break;

            // error情况
            printf("peer closed!\n");
            dealloc_send_node(send_node);
            // 解锁并返回
            pthread_mutex_unlock(&sess->send_que->send_que_lock);
            return -1;
        }

        if (send_node->send_buf->offset == send_node->send_buf->data_len)
        {
            // 数据发送完全
            dealloc_send_node(send_node);
            // 解锁并返回
            pthread_mutex_unlock(&sess->send_que->send_que_lock);
            return total_sent;
        }

        // 没发完全，节点入队
        enqueue_node(sess->send_que, send_node, epoll_fd, sess->fd, evs);
        // 解锁并返回
        pthread_mutex_unlock(&sess->send_que->send_que_lock);

        return total_sent;
    }

    // 队列不为空说明有其他线程发送EAGAIN了，那就放入队列
    enqueue_node(sess->send_que, send_node, epoll_fd, sess->fd, evs);
    pthread_mutex_unlock(&sess->send_que->send_que_lock);
    return 0;
}

// Dequeue and send on EPOLLOUT
void handle_epollout(int epoll_fd, int fd, send_que_t *q)
{
    pthread_mutex_lock(&q->send_que_lock);
    if (q->head == NULL)
    {
        pthread_mutex_unlock(&q->send_que_lock);
        return;
    }
    struct epoll_event ev;
    send_node_t *cur = q->head;

    while (cur)
    {
        data_buf_t *b = cur->send_buf;
        ssize_t n;
        while (b->offset < b->data_len)
        {
            n = write(fd, b->buf + b->offset, b->data_len - b->offset);
            if (n > 0)
            {
                b->offset += n;
                continue;
            }
            if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
                // kernel buffer full -> wait next EPOLLOUT
                ev.events = EPOLLIN | EPOLLET | EPOLLOUT;
                ev.data.fd = fd;
                epoll_ctl(epoll_fd, EPOLL_CTL_MOD, fd, &ev);
                pthread_mutex_unlock(&q->send_que_lock);
                return;
            }
            // unrecoverable error: close connection
            close(fd);
            pthread_mutex_unlock(&q->send_que_lock);
            return;
        }
        // fully sent: remove node
        send_node_t *next = cur->next;
        dealloc_send_node(cur);
        cur = next;
    }

    // 更新队列头尾
    q->head = NULL;
    q->tail = NULL;

    // queue empty: disable EPOLLOUT
    ev.events = EPOLLIN | EPOLLET;
    ev.data.fd = fd;
    epoll_ctl(epoll_fd, EPOLL_CTL_MOD, fd, &ev);
    pthread_mutex_unlock(&q->send_que_lock);
}