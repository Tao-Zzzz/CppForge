#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/poll.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAX_CLIENTS 1024
#define PORT 8080

int main(){
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    struct pollfd fds[MAX_CLIENTS + 1];
    int nfds = 1;
    char buffer[1024];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);
    bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    listen(server_fd, 6);

    fds[0].fd = server_fd;
    fds[0].events = POLLIN; //可读, 或有新连接请求(对应socket)

    // pollfd 传入时配置fd和感兴趣的事件
    // 返回时会修改revents, real 真正发生的事件
    while(1){
        // 检查阵列前nfds个元素
        // -1, 永久阻塞, 直到有一个文件描述符准备就绪
        poll(fds, nfds, -1);

        // 是否有事件发生, 是可读事件
        if(fds[0].revents & POLLIN){
            client_fd = accept(server_fd,(struct sockaddr*)&client_addr, &addr_len);
            for(int i = 1; i < MAX_CLIENTS + 1; i++){
                if(fds[i].fd == 0){
                    fds[i].fd = client_fd;
                    fds[i].events = POLLIN;
                    nfds = (i + 1 > nfds) ? i + 1 : nfds;
                    printf("New client: %d\n", client_fd);
                    break;
                }
            }
        }

        for(int i = 1; i < nfds; i++){
            if(fds[i].fd > 0 && fds[i].revents & POLLIN){
                int n = read(fds[i].fd, buffer, sizeof(buffer));
                if(n <= 0){
                    close(fds[i].fd);
                    fds[i].fd = 0;
                }else{
                    buffer[n] = '\0';
                    printf("Receive: %s", buffer);
                    write(fds[i].fd, buffer, n);
                }
            }
        }
    }
    close(server_fd);
    return 0;
}