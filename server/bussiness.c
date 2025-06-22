#include"threadpool.h"
#include<assert.h>

// 主线程调用，处理客户端发来的消息
void handleMessage(int sockfd, int epfd, task_queue_t* que){
    // 接收数据，将数据组装后放进que队列
    // 1.1 获取消息长度
    int length = -1;
    int ret = recvn(sockfd, &length, sizeof(length));
    if(ret == 0){
        goto end;
    }
    printf("\nrecvn length: %d\n", length);

    // 1.2获取消息类型
    int cmdType = -1;
    ret = recvn(sockfd, &cmdType, sizeof(cmdType));
    if(ret == 0){
        goto end;
    }
    
    // 1.3 获取消息内容
    task_t *ptask = (task_t*)calloc(1, sizeof(task_t));
    ptask->peerfd = sockfd;
    ptask->epfd = epfd; // 保存这个有啥用？
    ptask->type = (CmdType)cmdType;
    if(length>0){
        ret = recvn(sockfd, ptask->data, length);
        if(ret > 0){
            if(ptask->type == CMD_TYPE_PUTS){
                // 上传文件任务，暂时先从epoll中删除监听
                delEpollReadfd(epfd, sockfd);
            }
            queueTaskEnque(que, ptask);
        }

    }else if(length == 0){
        queueTaskEnque(que, ptask);
    }

end:
    // 连接断开情况
    if(ret == 0){
        delEpollReadfd(epfd, sockfd);
        close(sockfd);
    }
}

//子线程调用
void doTask(task_t* task){
    assert(task);
    switch(task->type){
    case CMD_TYPE_PWD:
        pwdCommand(task);
        break;
    case CMD_TYPE_LS:
        lsCommand(task);
        break;
    case CMD_TYPE_CD:
        cdCommand(task);
        break;
    case CMD_TYPE_PUTS:
        putsCommand(task);
        break;
    case CMD_TYPE_GETS:
        getsCommand(task);
        break;
    default:
        break;
    }
}

void putsCommand(task_t* task){
    // 上传文件
    int fd = open(task->data, O_RDWR | O_CREAT, 0644);
    if(fd < 0){
        perror("open");
        return;
    }

    int length;
    int ret = recvn(task->peerfd, &length, sizeof(length));
    printf("length = %d\n", length);
    char buff[1000] = {0};
    int left = length;
    while(left > 0){
        memset(buff, 0, sizeof(buff));
        if(left > 1000){
            ret = recvn(task->peerfd, buff, sizeof(buff));
        }else{
            ret = recvn(task->peerfd, buff, left);
        }
        write(fd, buff, ret);

        left -= ret;
    }

    close(fd);
}

void getsCommand(task_t* task){
    // 根据上传的文件名下载文件
    // 如果文件不存在，则返回-1告知用户
    int fd = open(task->data, O_RDWR, 0644);
    if(fd<0){
        perror("open");
        int length = -1;
        send(task->epfd, &length, sizeof(length), 0);
        return;
    }

    struct stat st;
    memset(&st, 0, sizeof(st));
    fstat(fd, &st);

    // 1.1 发送文件长度
    sendn(task->peerfd, (const char*)&st.st_size, sizeof(st.st_size));
    printf("file size = %ld\n", st.st_size);
    // 1.2发送文件内容
    off_t left = st.st_size;
    char buff[1000] = {0};
    int ret = 0;
    while(left>0){
        memset(buff, 0, sizeof(buff));
        ret = read(fd, buff, sizeof(buff));
        sendn(task->peerfd, buff, ret);

        left -= ret;
    }

    close(fd);
}

void pwdCommand(task_t* task){
    char pwd[1024];

    // 
    if(getcwd(pwd, sizeof(pwd)) != NULL){
        printf("%s\n", pwd);
    } else {
        perror("getcwd");
        return;
    }
}

void lsCommand(task_t* task){
    // 需要判断文件是否存在
    DIR* dir;
    struct dirent *entry;

    const char* dirname = task->data;
    if(dirname[0] != '\0'){
        printf("task->data: %s-\n", task->data);
        dir = opendir(dirname);
    }else {
        dir = opendir(".");
    }
    if(!dir){
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    while((entry = readdir(dir)) != NULL){
        printf("%s\n", entry->d_name);
    }

    closedir(dir);
}

void cdCommand(task_t* task){
    if(task->data[0] == '\0'){
        if(chdir("/home/") == -1){
            perror("chdir ~");
            exit(EXIT_FAILURE);
        }
        printf("Changed to ~\n");
    }else{
        if(chdir(task->data) == -1){
            perror("chdir");
            return;
            // exit(EXIT_FAILURE);
        }
        printf("Changed to %s\n", task->data);
    }
}
