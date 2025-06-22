#include "config.h"

void readConfig(const char* filename, HashTable* ht){
    FILE* fp = fopen(filename, "rw");
    if(fp == NULL){
        perror("fopen");
        return;
    }

    char buff[128] = {0};
    while(fgets(buff, sizeof(buff), fp) != NULL){
        buff[strcspn(buff, "\n")] = '\0';
        char* str[3] = {0};

        int cnt = 0;
        splitString(buff, "=", str, 3, &cnt); // calloc data
        insert(ht, str[0], str[1]);
        freeStrs(str, cnt);
    }

    fclose(fp);
}
