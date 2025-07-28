#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 12345

// 从 fd 上完整地读 count 字节到 buf，直到读满或出错
static ssize_t read_full(int fd, void *buf, size_t count)
{
    size_t off = 0;
    ssize_t n;
    while (off < count)
    {
        n = read(fd, (char *)buf + off, count - off);
        if (n < 0)
        {
            if (errno == EINTR)
                continue;
            return -1;
        }
        if (n == 0)
        {
            // 对端关闭
            return off;
        }
        off += n;
    }
    return off;
}

int main()
{
    int sock;
    struct sockaddr_in srv_addr;

    // 1. 创建 socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // 2. 连接服务器
    memset(&srv_addr, 0, sizeof(srv_addr));
    srv_addr.sin_family = AF_INET;
    srv_addr.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &srv_addr.sin_addr) <= 0)
    {
        perror("inet_pton");
        close(sock);
        exit(EXIT_FAILURE);
    }

    if (connect(sock, (struct sockaddr *)&srv_addr, sizeof(srv_addr)) < 0)
    {
        perror("connect");
        close(sock);
        exit(EXIT_FAILURE);
    }
    printf("Connected to %s:%d\n", SERVER_IP, SERVER_PORT);

    // 3. 读用户输入-发送-接收-显示 循环
    while (1)
    {
        char *line = NULL;
        size_t len = 0;
        printf("> ");
        fflush(stdout);

        // getline 会分配或扩展 buffer
        if (getline(&line, &len, stdin) < 0)
        {
            // EOF 或出错
            break;
        }
        // 去掉末尾换行
        size_t body_len = strcspn(line, "\r\n");
        line[body_len] = '\0';

        // 4 字节头 + payload
        uint16_t msg_type = 1; // 你可以根据协议修改
        uint16_t net_type = htons(msg_type);
        uint16_t net_len = htons(body_len);
        size_t total = sizeof(net_type) + sizeof(net_len) + body_len;
        uint8_t *buf = malloc(total);
        if (!buf)
        {
            perror("malloc");
            free(line);
            break;
        }

        memcpy(buf + 0, &net_type, sizeof(net_type));
        memcpy(buf + 2, &net_len, sizeof(net_len));
        memcpy(buf + 4, line, body_len);

        // 4. 发送
        if (write(sock, buf, total) != (ssize_t)total)
        {
            perror("write");
            free(buf);
            free(line);
            break;
        }
        free(buf);

        // 5. 接收回复头
        uint16_t resp_type, resp_len;
        if (read_full(sock, &resp_type, sizeof(resp_type)) != sizeof(resp_type) ||
            read_full(sock, &resp_len, sizeof(resp_len)) != sizeof(resp_len))
        {
            fprintf(stderr, "connection closed or read error\n");
            free(line);
            break;
        }
        resp_type = ntohs(resp_type);
        resp_len = ntohs(resp_len);

        // 6. 接收回复 body
        uint8_t *body = malloc(resp_len + 1);
        if (!body)
        {
            perror("malloc body");
            free(line);
            break;
        }
        if (read_full(sock, body, resp_len) != resp_len)
        {
            fprintf(stderr, "failed to read full body\n");
            free(body);
            free(line);
            break;
        }
        body[resp_len] = '\0'; // 方便当成字符串

        // 7. 显示
        printf("<< type=%u, len=%u, data=\"%s\"\n",
               resp_type, resp_len, (char *)body);

        free(body);
        free(line);
    }

    printf("Closing connection.\n");
    close(sock);
    return 0;
}
