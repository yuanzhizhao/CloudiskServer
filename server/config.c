#include "config.h"

void readConfig(const char* filename, HashTable* ht){
    FILE* fp = fopen(filename, "rw");
    if(fp == NULL){
        perror("fopen");
        return;
    }

    char buff[128] = {0};
    while(fgets(buff, sizeof(buff), fp) != NULL){
        char* str[3] = {0};

        int cnt = 0;
        splitString(buff, "=", str, 3, &cnt);
        char* value = (char*)calloc(1, strlen(str[1] +1));
        strcpy(value, str[1]);
        insert(ht, str[0], value);
        freeStrs(str, cnt);
    }

    fclose(fp);
}
