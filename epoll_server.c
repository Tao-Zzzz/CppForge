#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>

#define MAX_EVENTS 1024
#define PORT 8080

void set_nonblocking(int fd){
    // 读取当前模式
    int flags = fcntl(fd, F_GETFL, 0);
    // 添加非阻塞模式
    fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

int main(){
    int server_fd, client_fd, epoll_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    struct epoll_event ev, events[MAX_EVENTS];
    char buffer[1024];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);
    bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    listen(server_fd, 5);

    // epoll实例,创建文件描述符
    epoll_fd = epoll_create1(0);
    // 添加服务监听事件
    ev.events = EPOLLIN;
    ev.data.fd = server_fd;
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &ev);

    while(1){
        // 等待注册到epoll_fd上的文件描述符上的事件发生
        // -1永久阻塞
        // events阵列, 當事件發生時，內核會將就緒的事件信息填充到這個陣列中
        // 返回实际有多少个文件描述符发生了事件
        int nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);

        for(int i = 0; i < nfds; i++){
            if(events[i].data.fd == server_fd){
                client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &addr_len);
                set_nonblocking(client_fd);
                // 设置边缘触发
                ev.events = EPOLLIN | EPOLLET;
                ev.data.fd = client_fd;
                epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &ev);
                printf("New client: %d\n", client_fd);
            }

            else{
                int n;
                // 因为是边缘触发, 一次就要全部读完才行
                while((n = read(events[i].data.fd, buffer, sizeof(buffer))) > 0){
                    buffer[n] = '\0';
                    printf("Received: %s", buffer);
                    write(events[i].data.fd, buffer, n);
                }
                if(n == 0 || (n < 0 && errno != EAGAIN)){
                    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, events[i].data.fd, NULL);
                    close(events[i].data.fd);
                }
            }
        }
    }

    close(server_fd);
    close(epoll_fd);
}