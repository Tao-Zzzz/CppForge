// select_server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>    // close()
#include <arpa/inet.h> // inet_ntoa()
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <errno.h>

#define PORT 8080
#define BUFFER_SIZE 1024
#define MAX_CLIENTS 1024

int main()
{
    int server_fd, new_socket, client_socket[MAX_CLIENTS];
    struct sockaddr_in address;
    char buffer[BUFFER_SIZE];
    fd_set read_fds;
    int max_sd, sd, activity, i, valread;
    socklen_t addr_len = sizeof(address);

    // 初始化所有客户端套接字为0
    for (i = 0; i < MAX_CLIENTS; i++)
        client_socket[i] = 0;

    // 创建服务器端Socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // 设置Socket选项（可选）
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    {
        perror("setsockopt");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // 绑定Socket到指定IP和端口
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

    printf("Select Server is listening on port %d...\n", PORT);

    while (1)
    {
        // 清空文件描述符集合
        FD_ZERO(&read_fds);

        // 将服务器Socket添加到读集合中
        FD_SET(server_fd, &read_fds);
        max_sd = server_fd;

        // 添加客户端Socket到读集合中
        for (i = 0; i < MAX_CLIENTS; i++)
        {
            sd = client_socket[i];

            if (sd > 0)
                FD_SET(sd, &read_fds);

            if (sd > max_sd)
                max_sd = sd;
        }

        // 调用 select，等待I/O事件
        activity = select(max_sd + 1, &read_fds, NULL, NULL, NULL);

        if ((activity < 0) && (errno != EINTR))
        {
            perror("select error");
        }

        // 如果有新连接请求，处理它
        if (FD_ISSET(server_fd, &read_fds))
        {
            if ((new_socket = accept(server_fd, (struct sockaddr *)&address, &addr_len)) < 0)
            {
                perror("accept");
                exit(EXIT_FAILURE);
            }

            // 输出新连接的信息
            printf("New connection: socket fd %d, IP %s, Port %d\n",
                   new_socket,
                   inet_ntoa(address.sin_addr),
                   ntohs(address.sin_port));

            // 将新Socket添加到客户端列表中
            for (i = 0; i < MAX_CLIENTS; i++)
            {
                if (client_socket[i] == 0)
                {
                    client_socket[i] = new_socket;
                    printf("Added to list of clients at index %d\n", i);
                    break;
                }
            }
        }

        // 检查所有客户端Socket，看看是否有数据可读
        for (i = 0; i < MAX_CLIENTS; i++)
        {
            sd = client_socket[i];

            if (FD_ISSET(sd, &read_fds))
            {
                // 检查是否是关闭连接
                if ((valread = read(sd, buffer, BUFFER_SIZE)) == 0)
                {
                    // 客户端断开连接
                    getpeername(sd, (struct sockaddr *)&address, &addr_len);
                    printf("Host disconnected: IP %s, Port %d\n",
                           inet_ntoa(address.sin_addr),
                           ntohs(address.sin_port));

                    // 关闭Socket并清除
                    close(sd);
                    client_socket[i] = 0;
                }
                else
                {
                    // 收到数据，回显给客户端
                    buffer[valread] = '\0';
                    printf("Received from client [%d]: %s\n", sd, buffer);
                    send(sd, buffer, valread, 0);
                }
            }
        }
    }

    // 关闭服务器Socket（实际上，这行代码不会被执行到）
    close(server_fd);
    return 0;
}