#pragma once

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_SIZE 100
#define EMPTY NULL

typedef struct {
    char key[50];
    void* value;
}KeyValue;


// 哈希表结构体
typedef struct{
    KeyValue table[MAX_SIZE];
    int size;
}HashTable;


// 哈希函数
unsigned int hash(const char* key);


// 初始化哈希表
void initHashTable(HashTable* ht);

// 插入键值对
void insert(HashTable* ht, const char* key, void* value);

// 查找值
void* find(HashTable* ht, const char* key);

// 删除值
void erase(HashTable* ht, const char* key);

// 打印哈希表内容
void printHashTable(HashTable* ht);

// 销毁哈希表
void destroyHashTable(HashTable* ht);

