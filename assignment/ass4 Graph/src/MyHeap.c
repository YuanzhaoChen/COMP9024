#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "MyHeap.h"
#include "MyGraph.h"

BinaryHeap *newBinaryHeap(int size){
    BinaryHeap *heap=malloc(sizeof(BinaryHeap));
    assert(heap!=NULL);
    heap->node=malloc((size+1)*sizeof(Vertex *));
    heap->node[0]=NULL; // position at 0 is UNUSED, root is at position 1
    heap->n=1;          // number of vertices currently in heap, 0 position is also counted although it's NULL
    heap->size=size+1;
    return heap;
}

void BinaryHeap_addNode(BinaryHeap *heap,Vertex *v){
    assert(heap->n<heap->size);
    heap->node[heap->n]=v;
    v->positionInHeap=heap->n;
    heap->n+=1;
    BinaryHeap_upHeap(heap,v->positionInHeap);
}

void BinaryHeap_changeNode(BinaryHeap *heap,int pos,int newDistance){
    heap->node[pos]->distanceFromStartVertex=newDistance;
    if(BinaryHeap_canUpHeap(heap,pos)){
        BinaryHeap_upHeap(heap,pos);
    }else if(BinaryHeap_canDownHeap(heap,pos)){
        BinaryHeap_downHeap(heap,pos);
    }
    
}

void BinaryHeap_swap(BinaryHeap *heap, int src, int dst){
    assert(src>=1 && src<heap->n && dst>=1 && dst<heap->n);
    Vertex *tmp=heap->node[src];
    heap->node[src]=heap->node[dst];
    heap->node[src]->positionInHeap=src;
    heap->node[dst]=tmp;
    heap->node[dst]->positionInHeap=dst;
}

Vertex *BinaryHeap_removeMin(BinaryHeap *heap){
    assert(heap->n>1);
    Vertex *min=heap->node[1];  // root of binary heap is always the smallest
    if(heap->n==2){             // if only one vertex in heap, easy, return directly
        heap->node[1]=NULL;
        heap->n-=1;
        return min;
    }
    BinaryHeap_swap(heap,1,heap->n-1);
    heap->node[heap->n-1]=NULL;
    heap->n-=1;
    if(BinaryHeap_canDownHeap(heap,1)){
        BinaryHeap_downHeap(heap,1);
    }
    return min;
}

Vertex *BinaryHeap_findMin(BinaryHeap *heap){
    assert(heap->n>1);
    return heap->node[1];
}

int BinaryHeap_parentPos(int pos){
    if(pos%2==0){
        return pos/2;
    }else{
        return (pos-1)/2;
    }
}

int BinaryHeap_leftChildPos(int pos){
    return 2*pos;
}

int BinaryHeap_rightChildPos(int pos){
    return 2*pos+1;
}

int BinaryHeap_canUpHeap(BinaryHeap *heap,int pos){
    // A binary heap can swim up when it is not root and it is smaller than it's parent
    // Ususally an up heap happended after changeNode, in this case the vertex goes up along the heap if shortest distance is decreased
    assert(pos>=1 && pos<heap->n);
    if(pos>1 && \
    heap->node[BinaryHeap_parentPos(pos)]->distanceFromStartVertex>heap->node[pos]->distanceFromStartVertex){
        return 1;
    }
    return 0;
}

int BinaryHeap_canDownHeap(BinaryHeap *heap,int pos){
    // A binary heap can sink down when it has child and it is bigger than one of it's children
    // Usually a down heap happened after removeMin, in this case we try to sink down root vertex
    assert(pos>=1 && pos<heap->n);
    if(BinaryHeap_leftChildPos(pos)<heap->n && heap->node[pos]->distanceFromStartVertex>heap->node[BinaryHeap_leftChildPos(pos)]->distanceFromStartVertex){
        return 1;   // if it is less than left child, return 1 
    }
    if(BinaryHeap_rightChildPos(pos)<heap->n && heap->node[pos]->distanceFromStartVertex>heap->node[BinaryHeap_rightChildPos(pos)]->distanceFromStartVertex){
        return 1;   // if it is less than right child, return 1
    }
    return 0;
}

void BinaryHeap_upHeap(BinaryHeap *heap,int pos){
    while(BinaryHeap_canUpHeap(heap,pos)){
        BinaryHeap_swap(heap,BinaryHeap_parentPos(pos),pos);
        pos=BinaryHeap_parentPos(pos);
    }
}

void BinaryHeap_downHeap(BinaryHeap *heap, int pos){
    while(BinaryHeap_canDownHeap(heap,pos)){
        if(heap->node[pos]->distanceFromStartVertex>heap->node[BinaryHeap_leftChildPos(pos)]->distanceFromStartVertex){
            BinaryHeap_swap(heap,pos,BinaryHeap_leftChildPos(pos));
            pos=BinaryHeap_leftChildPos(pos);
        }else{
            BinaryHeap_swap(heap,pos,BinaryHeap_rightChildPos(pos));
            pos=BinaryHeap_rightChildPos(pos);
        }
    }
}

void BinaryHeap_printHeap(BinaryHeap *heap){
    for(int i=1;i<heap->n;i+=1){
        printf("%d|%d:%s\t%d\n",i,heap->node[i]->positionInHeap,heap->node[i]->thisBusStop->name,heap->node[i]->distanceFromStartVertex);
    }
}

void BinaryHeap_free(BinaryHeap *heap){
    free(heap->node);
}