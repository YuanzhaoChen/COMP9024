#ifndef MYHEAP_H
#define MYHEAP_H
#include "MyGraph.h"

typedef struct binary_heap{
    struct vertex **node;  // the vertex pointers of graph are all stored in an array 
    int n;          // current number of nodes
    int size;       // total number of nodes
}BinaryHeap;

BinaryHeap *newBinaryHeap(int size);
void BinaryHeap_addNode(BinaryHeap *heap,struct vertex *v);
void BinaryHeap_changeNode(BinaryHeap *heap,int pos,int newDistance);
struct vertex *BinaryHeap_removeMin(BinaryHeap *heap);
struct vertex *BinaryHeap_findMin(BinaryHeap *heap); 
void BinaryHeap_swap(BinaryHeap *heap, int src, int dst);
void BinaryHeap_upHeap(BinaryHeap *heap,int pos);
void BinaryHeap_downHeap(BinaryHeap *heap, int pos);
int BinaryHeap_parentPos(int pos);
int BinaryHeap_rightChildPos(int pos);
int BinaryHeap_leftChildPos(int pos);
int BinaryHeap_canUpHeap(BinaryHeap *heap,int pos);
int BinaryHeap_canDownHeap(BinaryHeap *heap,int pos);
void BinaryHeap_printHeap(BinaryHeap *heap);
void BinaryHeap_free(BinaryHeap *heap);
#endif