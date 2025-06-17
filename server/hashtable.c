#include "hashtable.h"

// 哈希函数
unsigned int hash(const char* key){
    unsigned int hashVal = 0;
    while (*key != '\0') {
        hashVal = (hashVal << 5) + hashVal + *key++;
    }

    return hashVal % MAX_SIZE;
}

// 初始化哈希表
void initHashTable(HashTable* ht){
    ht->size = 0;
    for(int i=0; i<MAX_SIZE; ++i){
        strcpy(ht->table[i].key, "");
        ht->table[i].value = EMPTY;
    }
}

// 插入键值对
void insert(HashTable* ht, const char* key, void* value){
    //
    unsigned int index = hash(key);
    while(ht->table[index].value != EMPTY){
        index = (index+1)%MAX_SIZE;
        if(strcmp(ht->table[index].key, key) == 0){
            ht->table[index].value = value;
            return;
        }
    }

    strcpy(ht->table[ht->size].key, key);
    ht->table[ht->size].value = value; 
    ht->size++;
}

// 查找值
void* find(HashTable* ht, const char* key){
    unsigned int index = hash(key);
    while(ht->table[index].value != EMPTY){
        if(strcmp(ht->table[index].key, key) == 0){
            return ht->table[index].value;
        }
        index = (index+1)%MAX_SIZE;
    }

    return EMPTY;
}

// 删除值
void erase(HashTable* ht, const char* key){
    unsigned int index = hash(key);
    while(ht->table[index].value != EMPTY){
        if(strcmp(ht->table[index].key, key) == 0){
            ht->table[index].value = EMPTY;
            free(ht->table[index].value);
            strcpy(ht->table[index].key, "");
            ht->size--;
        }

        index = (index+1)%MAX_SIZE;
    }
}

// 打印哈希表内容
void printHashTable(HashTable* ht){
    printf("HashTable's content: \n");
    for(int index=0; index<MAX_SIZE; ++index){
        if(ht->table[index].value != EMPTY){
            printf("key=%s, value=%s\n", ht->table[index].key, (const char*)ht->table[index].value);
        }
    }

    printf("\n");
}

// 销毁哈希表
void destroyHashTable(HashTable* ht){
    for(int index=0; index<MAX_SIZE; ++index){
        if(ht->table[index].value != EMPTY){
            strcpy(ht->table[index].key, "");
            ht->table[index].value = EMPTY;
            free(ht->table[index].value);
            ht->size--;
        }
    }
}

