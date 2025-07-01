#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAX_CLIENTS 1024
#define PORT 8080

int main(){
    int server_fd, client_fd, max_fd;
    fd_set read_fds;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    int client_sockets[MAX_CLIENTS] = {0};
    char buffer[1024];

    server_fd = socket(AF_INET, SOCK_STREAM, 0); //IPv4, TCP, 默认协议
    server_addr.sin_family = AF_INET; //IPv4
    server_addr.sin_addr.s_addr = INADDR_ANY; //监听任意地址
    server_addr.sin_port = htons(PORT); // 转网络字节序
    bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)); //套接字绑定本地地址
    listen(server_fd, 5); // 等待连接队列长度


    while(1){
        FD_ZERO(&read_fds); // 所有位清零
        FD_SET(server_fd, &read_fds); //将fd添加到监听中
        max_fd = server_fd; // 用于select的第一个参数

        for(int i = 0; i < MAX_CLIENTS; i++){
            if(client_sockets[i] > 0){
                FD_SET(client_sockets[i], &read_fds);
                if(client_sockets[i] > max_fd)
                    max_fd = client_sockets[i];   
            }
        }

        // 阻塞然后遍历, 写事件, 异常事件, 超时时间
        select(max_fd + 1, &read_fds, NULL, NULL, NULL);

        if(FD_ISSET(server_fd, &read_fds)){
            client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &addr_len);
            for(int i = 0; i < MAX_CLIENTS; i++){
                if(client_sockets[i] == 0){
                    client_sockets[i] = client_fd;
                    printf("New Client: %d\n", client_fd);
                    break;
                }
            }
        }

        for(int i = 0; i < MAX_CLIENTS; i++){
            if(client_sockets[i] > 0 && FD_ISSET(client_sockets[i], &read_fds)){
                int n = read(client_sockets[i], buffer, sizeof(buffer));
                if(n <= 0){
                    close(client_sockets[i]);
                    client_sockets[i] = 0;
                }else{
                    buffer[n] = '\0';
                    printf("Received : %s", buffer);
                    write(client_sockets[i], buffer, n);
                }
            }
        }
    }
    close(server_fd);
    return 0;
}