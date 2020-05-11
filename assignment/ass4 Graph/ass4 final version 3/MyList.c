// Author: Yuanzhao Chen
// Student ID: 5041686
// Platform: (select one of Windows, Unix, Mac): Mac(macOS Mojave)
#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include"MyList.h"
#include"MyGraph.h"

ListNode *newListNode(Edge *thisEdge){
    ListNode *newNode=malloc(sizeof(ListNode));
    assert(newNode!=NULL);
    newNode->thisEdge=thisEdge;
    newNode->next=NULL;
    return newNode;
}

List *newList(){
    List *L=malloc(sizeof(List));
    assert(L!=NULL);
    L->firstNode=NULL;
    L->lastNode=NULL;
    L->size=0;
    return L;
}

// add node to the end of the list
void appendList(List *L,Edge *thisEdge){
    assert(L!=NULL);
    ListNode *newNode=newListNode(thisEdge);
    if(L->size==0){
        L->firstNode=newNode;
        L->lastNode=newNode;
    }else{
        L->lastNode->next=newNode;
        L->lastNode=newNode;
    }
    L->size+=1;
}

// let the new node be the first node in list
void pushList(List *L, Edge *thisEdge){
    assert(L!=NULL);
    ListNode *newNode=newListNode(thisEdge);
    ListNode *prevFirst=L->firstNode;
    L->firstNode=newNode;
    newNode->next=prevFirst;
    L->size+=1;
}

// remove the first node in the list
ListNode *popList(List *L){
    assert(L!=NULL && L->size>0);
    ListNode *res=L->firstNode;
    L->firstNode=res->next;
    L->size-=1;
    return res;
}

void freeList(List *L){
    if(L==NULL){return;}
    ListNode *curr=L->firstNode;
    while(curr!=NULL){
        ListNode *nextNode=curr->next;
        free(curr);
        curr=nextNode;
    }
    free(L);
}