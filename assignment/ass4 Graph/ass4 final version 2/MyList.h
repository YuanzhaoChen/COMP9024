// Author: Yuanzhao Chen
// Student ID: 5041686
// Platform: (select one of Windows, Unix, Mac): Mac(macOS Mojave)
#ifndef MYLIST_H
#define MYLIST_H
#include "MyGraph.h"

typedef struct list_node{
    struct edge *thisEdge;
    struct list_node *next;
}ListNode;

typedef struct list{
    ListNode *firstNode;
    ListNode *lastNode;
    int size;
}List;

ListNode *newListNode(struct edge *thisEdge);
List *newList();
void appendList(List *L,struct edge *thisEdge);
void pushList(List *L,struct edge *thisEdge);
ListNode *popList(List *L);
void freeList(List *L);
#endif