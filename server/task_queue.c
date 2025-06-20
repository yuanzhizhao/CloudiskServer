#include"threadpool.h"

int queueInit(task_queue_t* que){
    if(que){
        que->pFront = NULL;
        que->pRear = NULL;
        que->queSize = 0;
        que->flag = 1;
        int ret = pthread_mutex_init(&que->mutex, NULL);
        if(ret != 0){
            error(1, errno, "pthread_mutex_init");
        }

        ret = pthread_cond_init(&que->cond, NULL);
        if(ret != 0){
            error(1, errno, "pthread_cond_init");
        }
    }

    return 0;
}

int queueDestroy(task_queue_t* que){
    if(que){
        int ret = pthread_mutex_destroy(&que->mutex);
        if(ret != 0){
            error(1, errno, "pthread_mutex_destroy");
        }

        ret = pthread_cond_destroy(&que->cond);
        if(ret != 0){
            error(1, errno, "pthread_cond_destroy");
        }
    }

    return 0;
}

int queueIsEmpty(task_queue_t* que){
    return que->queSize == 0;
}

int queueTaskSize(task_queue_t* que){
    return que->queSize;
}

int queueTaskEnque(task_queue_t* que, task_t* ptask){
    int ret = pthread_mutex_lock(&que->mutex);
    if(queueIsEmpty(que)){
        que->pFront = que->pRear = ptask;
    }else{
        que->pRear->pNext = ptask;
        que->pRear = que->pRear->pNext;
    }

    que->queSize++;
    ret = pthread_mutex_unlock(&que->mutex);

    ret = pthread_cond_signal(&que->cond);

    return 0;
}

task_t* queueTaskDeque(task_queue_t* que){
    int ret = pthread_mutex_lock(&que->mutex);
    task_t* pret;
    while(que->flag && queueIsEmpty(que)){
        pthread_cond_wait(&que->cond, &que->mutex);
    }

    if(que->flag){
        pret = que->pFront;
        if(queueTaskSize(que) == 1){
            que->pFront = que->pRear = NULL; 
        }else{
            que->pFront = que->pFront->pNext;
        }
        que->queSize--;
    }else{
        pret = NULL;
    }

    ret = pthread_mutex_unlock(&que->mutex);

    return pret;
}

int broadcastAll(task_queue_t* que){
    que->flag = 0;
    int ret = pthread_cond_broadcast(&que->cond);

    return 0;
}
