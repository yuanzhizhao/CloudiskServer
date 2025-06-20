#include "threadpool.h"

// 子线程的函数执行体
void* threadFunc(void* args){
    threadpool_t * pThreadPoll = (threadpool_t*)args;

    while(1){
        task_t* pTask = queueTaskDeque(&pThreadPoll->que);
        if(pTask){
            doTask(pTask);
            free(pTask);
        }else{
            break;
        }
    }

    printf("sub thread %ld is exiting.\n", pthread_self());
    return NULL;
}

// 线程池相关功能
// 线程池负责分配线程+接收任务+分配任务
int threadpoolInit(threadpool_t* threadpool, int num){
    threadpool->pthreadNum = num;
    threadpool->pthreads = (pthread_t*)calloc(num, sizeof(pthread_t));
    queueInit(&threadpool->que);

    return 0;
}

// 销毁线程池
int threadpoolDestroy(threadpool_t* threadpool){
    free(threadpool->pthreads);
    queueDestroy(&threadpool->que);
    
    return 0;
}

// 启动线程池
int threadpoolStart(threadpool_t* threadpool){
    if(threadpool){
        for(int i=0; i<threadpool->pthreadNum; ++i){
            int ret = pthread_create(&threadpool->pthreads[i], NULL, threadFunc, threadpool);
        }
    }

    return 0;
}

int threadpoolStop(threadpool_t* threadpool){
    while(!queueIsEmpty(&threadpool->que)){
        sleep(1);
    }

    broadcastAll(&threadpool->que);

    for(int i=0; i<threadpool->pthreadNum; ++i){
        pthread_join(threadpool->pthreads[i], NULL);
    }

    return 0;
}
