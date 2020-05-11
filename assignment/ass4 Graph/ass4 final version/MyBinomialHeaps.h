// Author: Yuanzhao Chen
// Student ID: 5041686
// Platform: (select one of Windows, Unix, Mac): Mac(macOS Mojave)
#ifndef MYBINOMIALHEAPS_H
#define MYBINOMIALHEAPS_H
#include "MyGraph.h"

typedef struct heap_node { 
	int key;                    // key is the distance of Edge 
	struct vertex *thisVertex;
	struct heap_node *parentNode;
	struct heap_node *rightSibling;
	struct heap_node *leftMostChild;
	int degree;
} HeapNode;

typedef struct biomial_heap{    
	int  size;	                // count of items in the heap
	HeapNode *firstTreeRoot;    // lowest degree bin-tree in heap
	HeapNode *lastTreeRoot;     // highest degree bin-tree in heap 
} BinomialHeap;

//----------------------------- functions implemented in MyBinomialHeaps.c -----------------------
HeapNode *newHeapNode(struct vertex *thisVertex);
BinomialHeap *newHeap();
int compareDegree(HeapNode *root1,HeapNode *root2);
void appendToHeap(BinomialHeap *T,HeapNode*root);
BinomialHeap *concatHeaps(BinomialHeap *T1, BinomialHeap *T2);
void linkBinTrees(HeapNode *root1,HeapNode *root2);
BinomialHeap *mergeHeaps(BinomialHeap *T1, BinomialHeap *T2);
void InsertToHeap(BinomialHeap *T, struct vertex *thisVertex);
HeapNode *findSamllestRoot(BinomialHeap *T);
HeapNode *removeSmallestRoot(BinomialHeap *T);
BinomialHeap *reverseOrder(BinomialHeap *T);
HeapNode *RemoveMin(BinomialHeap *T);
int Min(BinomialHeap *T);
void freeHeap_helper(HeapNode *root);
void freeHeap(BinomialHeap *T);
#endif