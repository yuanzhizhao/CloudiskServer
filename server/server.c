#include"threadpool.h"

int tcpInit(const char* ip, const char* port){
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if(listenfd<0){
        perror("socket");
        return -1;
    }

    int on = 1;
    int ret = setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    if(ret < 0){
        perror("setsockopt");
        close(listenfd);
        return -1;
    }

    // 绑定网络地址
    struct sockaddr_in serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(atoi(port));
    serveraddr.sin_addr.s_addr = inet_addr(ip);
    ret = bind(listenfd, (const struct sockaddr*)&serveraddr, sizeof(serveraddr));
    if(ret < 0){
        perror("bind");
        close(listenfd);
        return -1;
    }

    // 监听
    ret = listen(listenfd, 100);
    if(ret<0){
        perror("listen");
        close(listenfd);
        return -1;
    }

    printf("server starts to listen...\n");

    return listenfd;
}

int addEpollReadfd(int epfd, int fd){
    struct epoll_event evt;
    memset(&evt, 0, sizeof(evt));
    evt.data.fd = fd;
    evt.events = EPOLLIN;
    int ret = epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &evt);
    if(ret == -1){
        perror("epoll_ctl");
        return -1;
    }

    return 0;
}

int delEpollReadfd(int epfd, int fd){
    struct epoll_event evt;
    memset(&evt, 0, sizeof(evt));
    evt.data.fd = fd;
    evt.events = EPOLLIN;
    int ret = epoll_ctl(epfd, EPOLL_CTL_DEL, fd, &evt);
    if(ret == -1){
        perror("epoll_ctl del");
        return -1;
    }

    return 0;
}

int sendn(int sockfd, const void * buff, int len){
    //
    int left = len;
    int ret = -1;
    const char* curBuff= (const char*)buff;
    while(left){
        //
        ret = send(sockfd, curBuff, left, 0);
        if(ret < 0){
            perror("send");
            return -1;
        }

        left -= ret;
        curBuff += ret;
    }

    return len - left;
}

//其作用：确定接收len字节的数据
int recvn(int sockfd, void * buff, int len){
    int left = len;
    int ret = -1;
    char* curBuff= (char*)buff;

    while(left > 0){
        ret = recv(sockfd, curBuff, left, 0);
        if(ret == 0){
            break;
        }else if(ret < 0){
            perror("recvn");
            return -1;
        }

        left -= ret;
        curBuff += ret;
    }

    return len - left;
}

