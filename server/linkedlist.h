#pragma once
#include<stdio.h>
#include<stdlib.h>

typedef struct ListNode{
    void* val;
    struct ListNode *next;
}ListNode;

// 创建新的链表节点
ListNode* createNode(void* val);

// 在链表末尾添加元素
void appendNode(ListNode** head, void* val);

// 删除链表中值为target的节点, 仅删除一个
void deleteNode(ListNode** head, void* target);

// 删除链表中值为peerfd的节点，仅删除一个
void deleteNode2(ListNode** head, int peerfd);

// 打印链表
void printList(ListNode* head);

// 释放链表内存
void freeList(ListNode* head);
