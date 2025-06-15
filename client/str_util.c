#include "str_util.h"

void splitString(const char *pstr, const char * delimiter, char *ptokens[], int max_tokens, int * pcount){
    int token_count = 0;
    char* token = strtok((char *)pstr, delimiter);

    while(token != NULL && token_count < max_tokens - 1){
        char *pstr = (char*)calloc(1, strlen(token)+1);
        strcpy(pstr, token);
        ptokens[token_count++] = pstr;
        token = strtok(NULL, delimiter);
    }

    ptokens[token_count] = NULL;
    *pcount = token_count;
}

void freeStrs(char * pstrs[], int count){
    for(int i=0; i<count; ++i){
        free(pstrs[i]);
    }
}
