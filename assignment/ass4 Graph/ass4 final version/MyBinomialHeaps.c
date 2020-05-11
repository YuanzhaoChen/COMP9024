// Author: Yuanzhao Chen
// Student ID: 5041686
// Platform: (select one of Windows, Unix, Mac): Mac(macOS Mojave)
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>
#include<math.h>
#include "MyBinomialHeaps.h"
#include "MyGraph.h"

// Return a new HeapNode
HeapNode *newHeapNode(Vertex *thisVertex){
	HeapNode *newNode=malloc(sizeof(HeapNode));
	assert(newNode != NULL);
	newNode->key=thisVertex->distanceFromStartVertex;
    newNode->thisVertex=thisVertex;
	newNode->parentNode=NULL;
	newNode->rightSibling=NULL;
	newNode->leftMostChild=NULL;
	newNode->degree=0;
	return newNode;
}

// Return a new binomial heap
BinomialHeap *newHeap(){ 
	BinomialHeap *T;
	T = malloc(sizeof(BinomialHeap));
	assert (T != NULL);
	T->firstTreeRoot=NULL;
	T->lastTreeRoot=NULL;
	T->size=0;
	return T;
}

int compareDegree(HeapNode *root1,HeapNode *root2){
	return root1->degree-root2->degree;
}

// append a bin-tree to the rightmost of a bin-heap
void appendToHeap(BinomialHeap *T,HeapNode*root){
	if(T->firstTreeRoot==NULL){
		T->firstTreeRoot=root;
		T->lastTreeRoot=root;
	}else{
		T->lastTreeRoot->rightSibling=root;
		T->lastTreeRoot=root;
	}
}

// run-time complexity: O(log(N+M)), only the sibling relationship of bin-tree roots are changed
// simply concatenate T1 and T2 without modifying any bin-tree structure
// output is ordered by degree of bin-tree, the process is similar to merge sort
BinomialHeap *concatHeaps(BinomialHeap *T1, BinomialHeap *T2){
	BinomialHeap *T3=newHeap();
	HeapNode *node1=T1->firstTreeRoot;
	HeapNode *node2=T2->firstTreeRoot;
	while(node1!=NULL && node2!=NULL){
		int cmp=compareDegree(node1,node2);
		if(cmp>0){
			appendToHeap(T3,node2);
			node2=node2->rightSibling;
		}else if(cmp==0){
			appendToHeap(T3,node1);
			node1=node1->rightSibling;
			appendToHeap(T3,node2);
			node2=node2->rightSibling;
		}else{
			appendToHeap(T3,node1);
			node1=node1->rightSibling;
		}
	}
	while(node1!=NULL){
		appendToHeap(T3,node1);
		node1=node1->rightSibling;
	}
	while(node2!=NULL){
		appendToHeap(T3,node2);
		node2=node2->rightSibling;
	}
	T3->size=T1->size+T2->size;
	return T3;
}

// run-time complexity: O(1), 
// combine two bin-trees with degree k-1 into one, degree becomes k
void linkBinTrees(HeapNode *root1,HeapNode *root2){
	assert(root1->degree==root2->degree);
	HeapNode *smallerRoot;
	HeapNode *largerRoot;
	if(root1->key<=root2->key){
		smallerRoot=root1;
		largerRoot=root2;
	}else{
		smallerRoot=root2;
		largerRoot=root1;
	}
	largerRoot->parentNode=smallerRoot;
	largerRoot->rightSibling=smallerRoot->leftMostChild;
	smallerRoot->leftMostChild=largerRoot;
	smallerRoot->degree+=1;
}

// tmpHeap2ing heap is stored in T1
BinomialHeap *mergeHeaps(BinomialHeap *T1, BinomialHeap *T2){
	assert(T1!=NULL);
	assert(T2!=NULL);
	BinomialHeap *T3=concatHeaps(T1,T2);
	if(T3->firstTreeRoot==NULL){return T3;}
	HeapNode *prevNode=NULL;
	HeapNode *currNode=T3->firstTreeRoot;
	HeapNode *nextNode=currNode->rightSibling;
	while(nextNode!=NULL){
		if(currNode->degree!=nextNode->degree || (nextNode->rightSibling!=NULL&&nextNode->rightSibling->degree==currNode->degree)){
			prevNode=currNode;
			currNode=nextNode;
		}else{
			if(currNode->key<=nextNode->key){
				currNode->rightSibling=nextNode->rightSibling;
				linkBinTrees(currNode,nextNode);
			}else{
				if(prevNode==NULL){
					T3->firstTreeRoot=nextNode;
				}else{
					prevNode->rightSibling=nextNode;
				}
				linkBinTrees(nextNode,currNode);
				currNode=nextNode;
			}
		}
		nextNode=currNode->rightSibling;
	}
	return T3;
}

// run-time complexity: O(logN)   
void InsertToHeap(BinomialHeap *T, struct vertex *thisVertex){
	assert(T!=NULL);
	HeapNode *new=newHeapNode(thisVertex);
	assert(new!=NULL);
	BinomialHeap *tmpHeap1=newHeap();
	tmpHeap1->firstTreeRoot=new;
	tmpHeap1->lastTreeRoot=new;
	tmpHeap1->size=1;
	if(T->firstTreeRoot==NULL){
		T->firstTreeRoot=new;
		T->lastTreeRoot=new;
		T->size=1;
		return;
	}
	BinomialHeap *tmpHeap2=mergeHeaps(T,tmpHeap1);
	T->firstTreeRoot=tmpHeap2->firstTreeRoot;
	T->lastTreeRoot=tmpHeap2->lastTreeRoot;
	T->size=tmpHeap2->size;
	free(tmpHeap1);
	free(tmpHeap2);
}

// run-time complexity: O(logN)
HeapNode *findSamllestRoot(BinomialHeap *T){
	HeapNode *first=T->firstTreeRoot;
	HeapNode *smallestRoot=first;
	while(first!=NULL){
		if(first->key<smallestRoot->key){smallestRoot=first;}
		first=first->rightSibling;
	}
	return smallestRoot;
}

// run-time complexity: O(logN)
HeapNode *removeSmallestRoot(BinomialHeap *T){
	assert(T->firstTreeRoot!=NULL);
	HeapNode *smallestRoot=findSamllestRoot(T);
	if(smallestRoot==T->firstTreeRoot){
		T->firstTreeRoot=smallestRoot->rightSibling;
		if(T->firstTreeRoot==T->lastTreeRoot){
			T->lastTreeRoot=NULL;
		}
		return smallestRoot;
	}
	HeapNode *prevNode=NULL;
	HeapNode *currNode=T->firstTreeRoot;
	HeapNode *nextNode=currNode->rightSibling;
	while(currNode!=NULL){
		if(currNode==smallestRoot){
			prevNode->rightSibling=nextNode;
			break;
		}
		prevNode=currNode;
		currNode=nextNode;
		if(nextNode!=NULL){nextNode=nextNode->rightSibling;}
	}
	return smallestRoot;
}

// run-time complexity: O(logN), since only the root order are changed
BinomialHeap *reverseOrder(BinomialHeap *T){
	HeapNode *ref=T->lastTreeRoot;
	HeapNode *currNode=T->firstTreeRoot;
	while(currNode!=ref){
		T->lastTreeRoot->rightSibling=currNode;
		T->lastTreeRoot=currNode;
		currNode=currNode->rightSibling;
		T->lastTreeRoot->rightSibling=NULL;
	}
	T->firstTreeRoot=ref;           // after reverse the last tree root will become the first
	return T;
}

// run-time complexity: O(logN)
HeapNode *RemoveMin(BinomialHeap *T){
	assert(T->firstTreeRoot!=NULL);
	HeapNode *smallestRoot=removeSmallestRoot(T);
	BinomialHeap *tmpHeap1=newHeap();
	tmpHeap1->firstTreeRoot=smallestRoot->leftMostChild;
	smallestRoot->leftMostChild=NULL;
	HeapNode *lastNode=tmpHeap1->firstTreeRoot;
	while(lastNode!=NULL && lastNode->rightSibling!=NULL){
		lastNode->parentNode=NULL;
		lastNode=lastNode->rightSibling;
	}
	if(lastNode!=NULL){lastNode->parentNode=NULL;}
	tmpHeap1->lastTreeRoot=lastNode;
	tmpHeap1=reverseOrder(tmpHeap1); // after taking away root node, the resulting root list is ordered by desecending degree

	BinomialHeap *tmpHeap2=mergeHeaps(T,tmpHeap1);
	T->firstTreeRoot=tmpHeap2->firstTreeRoot;
	T->lastTreeRoot=tmpHeap2->lastTreeRoot;
	T->size=T->size-=1;
	return smallestRoot;	
}

// returns the smallest key without modifying the heap
// run-time complexity: log(N)
int Min(BinomialHeap *T){
	assert(T->firstTreeRoot!=NULL);
	HeapNode *head=T->firstTreeRoot;
	int minKey=head->key;
	while(head!=NULL){
		minKey=minKey>head->key?head->key:minKey;
		head=head->rightSibling;
	}
	return minKey;
}

// run-time complexity: O(N)
void freeHeap_helper(HeapNode *root){
	if(root==NULL){return;}
	freeHeap_helper(root->rightSibling);
	freeHeap_helper(root->leftMostChild);
	free(root);
}

// run-time complexity: O(N)
void freeHeap(BinomialHeap *T){
	freeHeap_helper(T->firstTreeRoot);
	free(T);
}