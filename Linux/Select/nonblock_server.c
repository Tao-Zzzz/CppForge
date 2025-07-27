#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>

#define PORT 8080
#define BUFFER_SIZE 1024
#define MAX_CLIENTS 1024

int set_nonblocking(int sockfd)
{
    int flags;
    if ((flags = fcntl(sockfd, F_GETFL, 0)) < 0)
    {
        perror("fcntl F_GETFL");
        return -1;
    }
    flags |= O_NONBLOCK;
    if (fcntl(sockfd, F_SETFL, flags) < 0)
    {
        perror("fcntl F_SETFL");
        return -1;
    }
    return 0;
}

int main()
{
    int server_fd, new_socket, client_socket[MAX_CLIENTS];
    struct sockaddr_in address;
    char buffer[BUFFER_SIZE];
    fd_set read_fds, write_fds;
    int max_sd, sd, activity, i, valread;
    socklen_t addr_len = sizeof(address);

    // 初始化所有客户端套接字为0
    for (i = 0; i < MAX_CLIENTS; i++)
        client_socket[i] = 0;

    // 创建服务器Socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // 设置非阻塞模式
    if (set_nonblocking(server_fd) < 0)
    {
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // 设置Socket选项
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    {
        perror("setsockopt");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // 绑定Socket
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // 监听
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    while (1)
    {
        // 清空文件描述符集合
        FD_ZERO(&read_fds);
        FD_ZERO(&write_fds);

        // 添加服务器Socket到读集合
        FD_SET(server_fd, &read_fds);
        max_sd = server_fd;

        // 添加客户端Socket到集合
        for (i = 0; i < MAX_CLIENTS; i++)
        {
            sd = client_socket[i];

            if (sd > 0)
            {
                FD_SET(sd, &read_fds);
                // 可选：根据需要添加写集合
                // FD_SET(sd, &write_fds);
            }

            if (sd > max_sd)
                max_sd = sd;
        }

        // 等待I/O事件
        activity = select(max_sd + 1, &read_fds, NULL, NULL, NULL);

        if ((activity < 0) && (errno != EINTR))
        {
            perror("select error");
        }

        // 如果有新连接
        if (FD_ISSET(server_fd, &read_fds))
        {
            while ((new_socket = accept(server_fd, (struct sockaddr *)&address, &addr_len)) >= 0)
            {
                printf("New connection: socket fd %d, IP %s, Port %d\n",
                       new_socket,
                       inet_ntoa(address.sin_addr),
                       ntohs(address.sin_port));

                // 设置新套接字为非阻塞
                if (set_nonblocking(new_socket) < 0)
                {
                    close(new_socket);
                    continue;
                }

                // 添加到客户端列表
                for (i = 0; i < MAX_CLIENTS; i++)
                {
                    if (client_socket[i] == 0)
                    {
                        client_socket[i] = new_socket;
                        printf("Added to list of clients at index %d\n", i);
                        break;
                    }
                }

                // 如果未找到空位，关闭连接
                if (i == MAX_CLIENTS)
                {
                    printf("Too many clients. Connection refused.\n");
                    close(new_socket);
                }
            }

            if (new_socket == -1 && errno != EAGAIN && errno != EWOULDBLOCK)
            {
                perror("accept");
            }
        }

        // 处理客户端Socket的I/O
        for (i = 0; i < MAX_CLIENTS; i++)
        {
            sd = client_socket[i];

            if (sd <= 0)
                continue;

            // 检查是否有数据可读
            if (FD_ISSET(sd, &read_fds))
            {
                valread = recv(sd, buffer, BUFFER_SIZE, 0);
                if (valread > 0)
                {
                    buffer[valread] = '\0';
                    printf("Received from client [%d]: %s\n", sd, buffer);

                    // 回显数据
                    send(sd, buffer, valread, 0);
                }
                else if (valread == 0)
                {
                    // 客户端断开连接
                    getpeername(sd, (struct sockaddr *)&address, &addr_len);
                    printf("Host disconnected: IP %s, Port %d\n",
                           inet_ntoa(address.sin_addr),
                           ntohs(address.sin_port));
                    close(sd);
                    client_socket[i] = 0;
                }
                else
                {
                    if (errno != EAGAIN && errno != EWOULDBLOCK)
                    {
                        perror("recv failed");
                        close(sd);
                        client_socket[i] = 0;
                    }
                }
            }
        }
    }

    close(server_fd);
    return 0;
}