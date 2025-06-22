#pragma once
#include<func.h>
#include"str_util.h"
#include<dirent.h>

typedef enum {
    CMD_TYPE_PWD=1,
    CMD_TYPE_LS,
    CMD_TYPE_CD,
    CMD_TYPE_MKDIR,
    CMD_TYPE_RMDIR,
    CMD_TYPE_PUTS,
    CMD_TYPE_GETS,
    CMD_TYPE_NOTCMD,  //不是命令

    TASK_LOGIN_SECTION1 = 100,
    TASK_LOGIN_SECTION1_RESP_OK,
    TASK_LOGIN_SECTION1_RESP_ERROR,
    TASK_LOGIN_SECTION2,
    TASK_LOGIN_SECTION2_RESP_OK,
    TASK_LOGIN_SECTION2_RESP_ERROR,
}CmdType;

// 任务数据结构
typedef struct task_s{
    int peerfd;
    int epfd;
    CmdType type;
    char data[1000];
    struct task_s* pNext;
}task_t;

// 任务队列
typedef struct task_queue_s {
    task_t* pFront;
    task_t* pRear;

    // 队列数量和是否退出标志
    int queSize;
    int flag;

    // 互斥量锁定，线程安全
    pthread_mutex_t mutex;
    pthread_cond_t cond;
}task_queue_t;

typedef struct threadpool_s {
    // 线程实体+线程数量
    pthread_t* pthreads;
    int pthreadNum;

    // 待执行的任务
    task_queue_t que;
    
}threadpool_t;

// 任务队列相关功能
int queueInit(task_queue_t* que);
int queueDestroy(task_queue_t* que);
int queueIsEmpty(task_queue_t* que);
int queueTaskSize(task_queue_t* que);
int queueTaskEnque(task_queue_t* que, task_t* ptask);
task_t* queueTaskDeque(task_queue_t* que);
int broadcastAll(task_queue_t* que);

// 线程池相关功能
int threadpoolInit(threadpool_t* threadpool, int num);
int threadpoolDestroy(threadpool_t* threadpool);
int threadpoolStart(threadpool_t* threadpool);
int threadpoolStop(threadpool_t* threadpool);

// 服务器相关功能
int tcpInit(const char* ip, const char* port);
int addEpollReadfd(int epfd, int fd);
int delEpollReadfd(int epfd, int fd);
int transferFile(int sockfd);
int sendn(int sockfd, const void* buff, int len);
int recvn(int sockfd, void* buff, int len);

// 客户端和服务器通信
void handleMessage(int sockfd, int epfd, task_queue_t* que);
void doTask(task_t* task);

// 具体命令相关功能，可拓展
void cdCommand(task_t* task);
void lsCommand(task_t* task);
void pwdCommand(task_t* task);
void mkdirCommand(task_t* task);
void rmdirCommand(task_t* task);
void notCommand(task_t * task);
void putsCommand(task_t * task);
void getsCommand(task_t * task);


// 用户登录相关功能
