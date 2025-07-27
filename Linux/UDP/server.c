// udp_server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>    // close()
#include <arpa/inet.h> // inet_ntoa()
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main()
{
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    char buffer[BUFFER_SIZE];
    const char *response = "Hello, UDP Client!";
    socklen_t addr_len = sizeof(client_addr);
    ssize_t bytes_received, bytes_sent;

    // 创建UDP套接字
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    // 填充服务器信息
    memset(&server_addr, 0, sizeof(server_addr)); // 清零
    server_addr.sin_family = AF_INET;             // IPv4
    server_addr.sin_addr.s_addr = INADDR_ANY;     // 监听所有接口
    server_addr.sin_port = htons(PORT);           // 端口

    // 绑定套接字到指定IP和端口
    if (bind(sockfd, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("UDP Server is listening on port %d...\n", PORT);

    while (1)
    {
        // 接收数据
        bytes_received = recvfrom(sockfd, buffer, BUFFER_SIZE - 1, 0,
                                  (struct sockaddr *)&client_addr, &addr_len);
        if (bytes_received < 0)
        {
            perror("recvfrom failed");
            continue; // 继续接收下一个数据包
        }

        buffer[bytes_received] = '\0'; // null终止
        printf("Received from %s:%d: %s\n",
               inet_ntoa(client_addr.sin_addr),
               ntohs(client_addr.sin_port),
               buffer);

        // 发送响应
        bytes_sent = sendto(sockfd, response, strlen(response), 0,
                            (struct sockaddr *)&client_addr, addr_len);
        if (bytes_sent < 0)
        {
            perror("sendto failed");
            continue;
        }

        printf("Sent response to %s:%d\n",
               inet_ntoa(client_addr.sin_addr),
               ntohs(client_addr.sin_port));
    }

    // 关闭套接字（实际上，此处不会被执行到）
    close(sockfd);
    return 0;
}