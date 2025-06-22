#include <func.h>
#include "client.h"

int main()
{
    const char* ip = "127.0.0.1";
    int port = 8080;
    int clientfd = tcpConnect(ip, port);

    // 用户登录操作
    // TODO


    // 使用epoll监听
    // 1. 用户的输入
    // 2. 服务器端的返回
    
    // 接收用户的输入
    char buf[128] = {0};
    train_t t;

    int epfd = epoll_create1(0);
    ERROR_CHECK(epfd, -1, "epoll create");

    // 添加用户输入事件到监听队列
    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = STDIN_FILENO;
    epoll_ctl(epfd, EPOLL_CTL_ADD, STDIN_FILENO, &ev);

    // 添加连接事件到用户监听队列
    ev.events = EPOLLIN;
    ev.data.fd = clientfd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, clientfd, &ev);
    
    while(1){
        //
        struct epoll_event events[1024];
        int n_ready = epoll_wait(epfd, events, sizeof(events), -1);
        if(n_ready == -1 && errno == EINTR){
            continue;
        }else if(n_ready == 0){
            printf("time out.\n");
        }else if(n_ready == -1){
            break;
        }else{
            for(int i=0; i<n_ready; ++i){
                int fd = events[i].data.fd;
                if(fd == STDIN_FILENO){
                    // TODO
                    memset(buf, 0, sizeof(buf));
                    int ret = read(STDIN_FILENO, buf, sizeof(buf));
                    if(ret == 0){
                        printf("byebye.\n");
                        break;
                    }else if(ret == 1){
                        continue;
                    }
                    memset(&t, 0, sizeof(t));

                    // 解析命令行
                    buf[strlen(buf)-1] = '\0';
                    parseCommand(buf, strlen(buf), &t);
                    sendn(clientfd, (const char*)&t, 4 + 4 + t.len);
                    if(t.type == CMD_TYPE_PUTS){
                        putsCommand(clientfd, &t);
                    }else if(t.type == CMD_TYPE_GETS){
                        getsCommand(clientfd, &t);
                    }
                }else if(fd == clientfd){
                    // TODO
                    memset(buf, 0, sizeof(buf));
                    int ret = recv(clientfd, buf, sizeof(buf), 0);
                    if(ret == 0){
                        printf("closed connect.\n");
                        exit(1);
                    }
                    printf("recv: %s\n", buf);
                }else{
                    continue;
                }
            }
        }
    }

    close(clientfd);
    return 0;
}

