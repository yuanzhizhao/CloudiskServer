#include "linkedlist.h"

// 创建新的链表节点
ListNode* createNode(void* val){
    ListNode* node = (ListNode*)calloc(1, sizeof(ListNode));
    if(!node){
        printf("calloc failed!\n");
        exit(1);
    }
    node->val = val;
    node->next = NULL;

    return node;
}

// 在链表末尾添加元素
void appendNode(ListNode** head, void* val){
    ListNode* newNode = createNode(val);
    if(*head == NULL){
        *head = newNode;
        return;
    }

    ListNode* curNode = *head;
    while(curNode->next != NULL){
        curNode = curNode->next;
    }

    curNode->next = newNode;
}

// 删除链表中值为target的节点, 仅删除一个
void deleteNode(ListNode** head, void* target){
    if(*head == NULL){
        return;
    }

    ListNode* curNode = *head;
    ListNode* preNode = *head;
    if(curNode->val == target){
        //
        *head = (*head)->next;
        free(curNode);
        return;
    }

    curNode = curNode->next;
    while(curNode != NULL){
        if(curNode->val == target){
            preNode->next = curNode->next;
            free(curNode);
            return;
        }
        curNode = curNode->next;
        preNode = preNode->next;
    }
}

// 删除链表中值为peerfd的节点，仅删除一个
void deleteNode2(ListNode** head, int peerfd){
    // TODO
}

// 打印链表
void printList(ListNode* head){
    ListNode* curNode = head;
    while(curNode != NULL){
        printf("%s\n", (char*)curNode->val);
        curNode = curNode->next;
    }
}

// 释放链表内存
void freeList(ListNode* head){
    ListNode* curNode = head;
    while(curNode != NULL){
        ListNode* temp = curNode;
        curNode = curNode->next;
        free(temp);
    }
}

