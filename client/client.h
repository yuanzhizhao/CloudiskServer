#pragma once

#define USER_NAME "please input a valid user name:\n"
#define PASSWARD "please input the right passward:\n"

typedef enum{
    CMD_TYPE_PWD=1,
    CMD_TYPE_LS,
    CMD_TYPE_CD,
    CMD_TYPE_MKDIR,
    CMD_TYPE_RMDIR,
    CMD_TYPE_PUTS,
    CMD_TYPE_GETS,
    CMD_TYPE_NOTCMD
}CmdType;

typedef struct{
    int len; // 记录内容长度
    CmdType type; // 命令类型
    char buff[1000]; // 记录内容本身
}train_t;

int tcpConnect(const char* ip, unsigned short port);
int recvn(int sockfd, void* buff, int len);
int sendn(int sockfd, const char* buff, int len);

int userLogin(int sockfd);

int parseCommand(const char* input, int len, train_t* pt);

// 
CmdType getCommandType(const char* str);
void putsCommand(int sockfd, train_t* pt);
void getsCommand(int sockfd, train_t* pt);
