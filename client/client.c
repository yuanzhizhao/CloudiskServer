#include "client.h"
#include "str_util.h"


int tcpConnect(const char* ip, unsigned short port){
    // 初始化socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if(sockfd < 0){
        perror("socket");
        return -1;
    }

    // 构建地址，绑定
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ip);
    
    // 和服务器建立连接
    int ret = connect(sockfd, (const struct sockaddr*)&server_addr, sizeof(server_addr));
    if(ret<0){
        perror("connect");
        close(sockfd);
        exit(1);
        //return -1;
    }

    printf("connected to server: %s\n", ip);

    return sockfd;
}

// 确定接收len字节数据
int recvn(int sockfd, void* buff, int len){
    int left = len;
    char *pbuf = (char *)buff;
    int ret = -1;

    while(left>0){
        ret = recv(sockfd, pbuf, left, 0);
        if(ret == 0){
            break;
        }else if(ret < 0){
            perror("recvn");
            // close(sockfd);
            return -1;
        }
        left -= ret;
        pbuf += ret;
    }

    return len - left;
}

// 确定发送len字节数据
int sendn(int sockfd, const char* buff, int len){
    int left = len;
    const char *pbuf = buff;
    int ret = -1;
    while(left>0){
        ret = send(sockfd, pbuf, left, 0);
        if(ret == 0){
            break;
        }else if(ret < 0){
            perror("sendn");
            return -1;
        }
        left -= ret;
        pbuf += ret;
    }
    
    return len -left;
}

int userLogin(int sockfd){
    // TODO
    return 0;
}

int parseCommand(const char* input, int len, train_t* pt){
    char *pstrs[10] = {0};
    int cnt = 0;
    splitString(input, " ", pstrs, 10, &cnt);
    pt->type = getCommandType(pstrs[0]);
    // command type: 1 cmd, 2, cmd content
    if(cnt>1){
        pt->len = strlen(pstrs[1]);
        strncpy(pt->buff, pstrs[1], pt->len);
    }

    // 
    freeStrs(pstrs, cnt);
    return 0;
}

// 返回命令类型
CmdType getCommandType(const char* str){
    if(!strcmp(str, "pwd"))
        return CMD_TYPE_PWD;
    else if(!strcmp(str, "ls"))
        return CMD_TYPE_LS;
    else if(!strcmp(str, "cd"))
        return CMD_TYPE_CD;
    else if(!strcmp(str, "mkdir"))
        return CMD_TYPE_MKDIR;
    else if(!strcmp(str, "rmdir"))
        return CMD_TYPE_RMDIR;
    else if(!strcmp(str, "puts"))
        return CMD_TYPE_PUTS;
    else if(!strcmp(str, "gets"))
        return CMD_TYPE_GETS;
    else 
        return CMD_TYPE_NOTCMD;
}

void putsCommand(int sockfd, train_t* pt){
    // TODO
    // 发送数据到服务器
    char filename[20] = {0};
    strcpy(filename, pt->buff);

    int fd = open(filename, O_RDWR);
    if(fd < 0){
        perror("open");
        return;
    }

    // 获取文件大小
    struct stat st;
    memset(&st, 0, sizeof(st));
    fstat(fd, &st);
    printf("file length: %ld\n", st.st_size);
    // 发送文件大小
    sendn(sockfd, (const char*)&st.st_size, sizeof(st.st_size));
    off_t cur = 0;
    char buff[1000] = {0};
    int ret = 0;
    // 发送文件内容
    while(cur < st.st_size){
        memset(buff, 0, sizeof(buff));
        ret = read(fd, buff, sizeof(buff));
        if(ret == 0){
            break;
        }
        ret = sendn(sockfd, buff, ret);
        cur += ret;
    }
    
    close(fd);
}

void getsCommand(int sockfd, train_t* pt){
    // 下载文件到本地
    // 1.1 获取文件名
    char* ptokens[20] = {0};
    int max_tokens = 20;
    int cnt = 0;
    if(pt->buff[0] == '\0'){
        return;
    }
        
    splitString(pt->buff, "/", ptokens, max_tokens, &cnt);
    char* filename = calloc(1, sizeof(char*));
    strcpy(filename, ptokens[cnt-1]);

    freeStrs(ptokens, cnt);

    // 1.2 打开文件
    int fd = open(filename, O_RDWR | O_CREAT, 0664);
    if(fd<0){
        perror("open");
        return;
    }

    // 1.3 接收文件大小
    off_t length = 0;
    recvn(sockfd, &length, sizeof(length));
    off_t curSize = 0;
    char buff[1000] = {0};
    int ret = 0;
    while(curSize < length){
        memset(buff, 0, sizeof(buff));
        if(length - curSize >= 1000){
            ret = recvn(sockfd, buff, sizeof(buff));
        }else {
            ret = recvn(sockfd, buff, length-curSize);
        }

        if(ret == 0){
            break;
        }
        write(fd, buff, ret);
        curSize += ret;
    }

    close(fd);
}

