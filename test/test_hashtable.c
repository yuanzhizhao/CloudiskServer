#include "hashtable.h"
#include<assert.h>

int main()
{
    printf("Hello world\n");
    HashTable ht;
    initHashTable(&ht);

    // 1. insert
    const char* key = "yuanzhi";
    char* value = "10086";
    insert(&ht, key, value);

    if(find(&ht, key) == EMPTY){
        printf("Failed, %s\n", (const char*)find(&ht, key));
    }
    
    printHashTable(&ht);


    destroyHashTable(&ht);
    
    return 0;
}

