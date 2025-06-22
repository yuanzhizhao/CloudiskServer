#include"threadpool.h"
#include"config.h"

int exitPipe[2];
#define EPOLL_ARR_SIZE 100

void sigHandler(int num){
    printf("\nsig is coming.\n");
    int one = 1;
    write(exitPipe[1], &one, sizeof(one));
}

int main(int argc, char** argv){
    //
    // read configs
    ARGS_CHECK(argc,2);

    pipe(exitPipe);

    pid_t pid = fork();
    if(pid>0){
        printf("ppid=%d, pid = %d\n", getpid(), pid);
        close(exitPipe[0]); //读端
        signal(SIGUSR1, sigHandler); //优雅退出
        wait(NULL);
        close(exitPipe[1]);
        printf("\nparent process exit.\n");
        exit(0);
    }

    // 子进程
    close(exitPipe[1]);
    
    HashTable ht;
    initHashTable(&ht);
    readConfig(argv[1], &ht);
    
    // 创建线程池
    threadpool_t threadpool;
    memset(&threadpool, 0, sizeof(threadpool));
    threadpoolInit(&threadpool, atoi((const char*)find(&ht, THREAD_NUM)));
    threadpoolStart(&threadpool);

    // 创建监听套接字
    int listenfd = tcpInit((const char*)find(&ht, IP), (const char*)find(&ht, PORT));

    // 创建epoll实例
    int epfd = epoll_create1(0);
    ERROR_CHECK(epfd, -1, "epoll_create1");

    // 对listenfd进行监听
    addEpollReadfd(epfd, listenfd);
    addEpollReadfd(epfd, exitPipe[0]);

    struct epoll_event* pEventArr = (struct epoll_event*)calloc(EPOLL_ARR_SIZE, sizeof(struct epoll_event));
    while(1){
        int n_ready = epoll_wait(epfd, pEventArr, EPOLL_ARR_SIZE, -1);
        printf("nready = %d.\n", n_ready);
        if(n_ready == -1 && errno == EINTR){
            continue;
        } else if(n_ready == -1){
            ERROR_CHECK(n_ready, -1, "epoll_wait");
        } else {
            // 一共三类消息体
            for(int i=0; i<n_ready; ++i){
                int fd = pEventArr[i].data.fd;
                if(fd == listenfd){
                    // 新连接到来
                    int peerfd = accept(listenfd, NULL, NULL);
                    ERROR_CHECK(peerfd, -1, "accept");
                    printf("\n conn %d has connected.\n", peerfd);
                    addEpollReadfd(epfd, peerfd);
                } else if(fd == exitPipe[0]){
                    // 线程池要退出
                    int howmany = 0;
                    read(exitPipe[0], &howmany, sizeof(howmany));
                    threadpoolStop(&threadpool);

                    threadpoolDestroy(&threadpool);
                    close(listenfd);
                    close(epfd);
                    close(exitPipe[0]);
                    destroyHashTable(&ht);
                    exit(0);
                } else {
                    // 已连接的客户端发来消息
                    handleMessage(fd, epfd, &threadpool.que);
                }
            }
        }
    }

    return 0;
}
