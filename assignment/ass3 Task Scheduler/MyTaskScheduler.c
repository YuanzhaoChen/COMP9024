// Author: Yuanzhao Chen
// Student ID: 5041686
// Platform: (select one of Windows, Unix, Mac): Mac(macOS Mojave)
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

typedef struct HeapNode { 
	// each node stores the priority (key), name, execution time,
	// release time and deadline of one task
	int key; //key of this task
	int TaskName;  // task name 
	int Etime; // execution time of this task
	int Rtime; // release time of this task
	int Dline; // deadline of this task
	// add additional fields below
	struct HeapNode *parentNode;
	struct HeapNode *rightSibling;
	struct HeapNode *leftMostChild;
	int degree;
} HeapNode;

// data type for a priority queue (heap) 
typedef struct BinomialHeap{ // this is heap header
	int  size;	// count of items in the heap
	// add additional fields below
	HeapNode *firstTreeRoot; // lowest degree bin-tree in heap
	HeapNode *lastTreeRoot; // highest degree bin-tree in heap 
} BinomialHeap;

//--------------------- function declaration --------------------
HeapNode *newHeapNode(int k, int n, int c, int r, int d);
BinomialHeap *newHeap();
int compareDegree(HeapNode *root1,HeapNode *root2);
void appendToHeap(BinomialHeap *T,HeapNode*root);
BinomialHeap *concatHeaps(BinomialHeap *T1, BinomialHeap *T2);
BinomialHeap *mergeHeaps(BinomialHeap *T1, BinomialHeap *T2);
void linkBinTrees(HeapNode *root1,HeapNode *root2);
void Insert(BinomialHeap *T, int k, int n, int c, int r, int d);
HeapNode *findSamllestRoot(BinomialHeap *T);
HeapNode *removeSmallestRoot(BinomialHeap *T);
HeapNode *RemoveMin(BinomialHeap *T);
void printBinTree(HeapNode *root);
void printBinHeaps(BinomialHeap *T);
void freeHeap(BinomialHeap *T);
//---------------------------------------------------------------

HeapNode *newHeapNode(int k, int n, int c, int r, int d){
	// k:key, n:task name, c: execution time, r:release time, d:deadline
	HeapNode *new;
	new=malloc(sizeof(HeapNode));
	assert(new != NULL);
	new->key=k;
	new->TaskName=n;
	new->Etime=c;
	new->Rtime=r; 
	new->Dline=d;
	// initialise other fields 
	new->parentNode=NULL;
	new->rightSibling=NULL;
	new->leftMostChild=NULL;
	new->degree=0;
	return new;
}

BinomialHeap *newHeap(){ 
	BinomialHeap *T;
	T = malloc(sizeof(BinomialHeap));
	assert (T != NULL);
	// initialise all the fields below
	T->firstTreeRoot=NULL;
	T->lastTreeRoot=NULL;
	T->size=0;
	return T;
}

int compareDegree(HeapNode *root1,HeapNode *root2){
	return root1->degree-root2->degree;
}

// run-time complexity: O(1)
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
	//printf("---------start heap concat---------\n");
	BinomialHeap *T3=newHeap();
	HeapNode *node1=T1->firstTreeRoot;
	//printf("T1:\n");
	//printBinHeaps(T1);
	HeapNode *node2=T2->firstTreeRoot;
	//printf("T2:\n");
	//printBinHeaps(T2);
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
	//printf("---------concat result-------\n");
	//printBinHeaps(T3);
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
void Insert(BinomialHeap *T, int k, int n, int c, int r, int d){
	// k: key, n: task name, c: execution time, r: release time, d:deadline 
	// You don't need to check if this task already exists in T 
	assert(T!=NULL);
	HeapNode *new=newHeapNode(k,n,c,r,d);
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
	T->size+=1;
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
	T->firstTreeRoot=ref;// after reverse the last tree root will become the first
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
	tmpHeap1=reverseOrder(tmpHeap1); //after taking away root node, the resulting root list is ordered by desecending degree

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

// run-time complexity: O(N), where N is the number of root nodes
void printBinTree(HeapNode *root){
	if(root==NULL){return;}
	if(root->parentNode==NULL){
		printf("\nAt Binomial tree degree %d:\n",root->degree);
	}
	printf("key: %d task: %d\n",root->key,root->TaskName);
	printBinTree(root->leftMostChild);
	printBinTree(root->rightSibling);
}

// run-time complexity: O(N), where N is the number of root nodes
void printBinHeaps(BinomialHeap *T){
	assert(T!=NULL);
	if(T->firstTreeRoot==NULL){printf("Nothing to print in heap.\n");}
	printBinTree(T->firstTreeRoot);
	printf("\n");
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

// run-time complexity of Insert(), Min(), RemoveMin(): O(logN)
// there are totally N task in a file, hence
// total run-time complexity: O(NlogN) 
int TaskScheduler(char *f1, char *f2, int m ){
	FILE *fp1=fopen(f1,"r");	// read integer from input file
	FILE *fp2=fopen(f2,"w");	// open output file with write mode to clear existing content
	fclose(fp2);
	fp2=fopen(f2,"a");	// open output file with append mode
	if(fp1==NULL){
		printf("file 1 does not exist\n");
		return 0;
	}
	BinomialHeap *releaseTime_PQ=newHeap();	// stores all the tasks where the key of each task is its release time
	assert(releaseTime_PQ!=NULL);
	BinomialHeap *deadLine_PQ=newHeap();	// stores all the ready tasks where the key of each task is its deadline
	assert(deadLine_PQ!=NULL);
	BinomialHeap *schedulingPoint_PQ=newHeap();	// each core is stored as a single heapNode in this PQ
	assert(schedulingPoint_PQ!=NULL);
	int n,c,r,d;	// n: task name, c: execution time, r: release time, d:deadline 
	int retval;
	for(int i=m;i>0;i-=1){Insert(schedulingPoint_PQ,0,i,0,0,0);} // initialize cores, set available time to 0
	//printBinHeaps(schedulingPoint_PQ);
	int endTime=0;	// max deadline in f1, after that we can stop simulation
	while((retval=fscanf(fp1,"%d %d %d %d",&n,&c,&r,&d))==4){	// record the release time of all tasks
		Insert(releaseTime_PQ,r,n,c,r,d);
		if(endTime<d){endTime=d;}
		//printf("tsk:%d, etime:%d, rtime:%d, dline:%d\n",n,c,r,d);
	}
	if(retval!=EOF){printf("Input error %d\n",retval); return 0;}
	for(int t=0;t<=endTime;t+=1){
		while(releaseTime_PQ->firstTreeRoot!=NULL && Min(releaseTime_PQ)<=t){	// there's a new task available, add to deadline PQ
			HeapNode *minNode=RemoveMin(releaseTime_PQ);
			Insert(deadLine_PQ,minNode->Dline,minNode->TaskName,minNode->Etime,minNode->Rtime,minNode->Dline);
		}
		while(schedulingPoint_PQ->firstTreeRoot!=NULL && Min(schedulingPoint_PQ)<=t){	// there's an idle core, assign earliest deadline 
			if(deadLine_PQ->firstTreeRoot!=NULL){										// task to the core, update next available time (key)
				HeapNode *task=RemoveMin(deadLine_PQ);
				HeapNode *idleCore=RemoveMin(schedulingPoint_PQ);
				if(task->Etime+t>task->Dline){	//miss deadline
					//printf("At time %d,task%d with deadline %d is missed\n",t,task->TaskName,task->Dline);
					return 0;
				}
				idleCore->key=t+task->Etime;
				Insert(schedulingPoint_PQ,idleCore->key,idleCore->TaskName,idleCore->Etime,idleCore->Rtime,idleCore->Dline);
				fprintf(fp2,"(task%d)-core%d-(rt:%d) ",task->TaskName,idleCore->TaskName,t);
			}else{
				break;
			}
		}		
		if(deadLine_PQ->firstTreeRoot!=NULL && Min(deadLine_PQ)<=t){	//miss deadline
			HeapNode *missedTask=RemoveMin(deadLine_PQ);
			printf("At time %d, task%d with deadline %d is missed\n",t,missedTask->TaskName,missedTask->Dline);
			return 0;
		}
	}
	freeHeap(releaseTime_PQ);
	freeHeap(deadLine_PQ);
	freeHeap(schedulingPoint_PQ);
	fclose(fp1);
	fclose(fp2);
	return 1;
}

//sample main for testing 
int main(){ 
	int i;
	printf("-------------samplefile1 with 4 cores-------------\n");
	i=TaskScheduler("samplefile1.txt", "feasibleschedule1.txt", 4);
	if(i==0){ printf("No feasible schedule!\n"); }
	else{ printf("samplefile1 with 4 cores is feasible\n"); }
	//There is a feasible schedule on 4 cores

	printf("-------------samplefile1 with 3 cores-------------\n");
	i=TaskScheduler("samplefile1.txt", "feasibleschedule2.txt", 3);
	if(i==0){ printf("No feasible schedule!\n"); }
	else{ printf("samplefile1 with 3 cores is feasible\n"); }
	//There is no feasible schedule on 3 cores
	
	printf("-------------samplefile2 with 5 cores--------------\n");
	i=TaskScheduler("samplefile2.txt", "feasibleschedule3.txt", 5);
	if(i==0){ printf("No feasible schedule!\n"); }
	else{ printf("samplefile2 with 5 cores is feasible\n"); }
	//There is a feasible schedule on 5 cores 
	
	printf("-------------samplefile2 with 4 cores--------------\n");
	i=TaskScheduler("samplefile2.txt", "feasibleschedule4.txt", 4);
	if(i==0){ printf("No feasible schedule!\n"); }
	else{ printf("samplefile2 with 4 cores is feasible\n"); }
	//There is no feasible schedule on 4 cores 

	printf("-------------samplefile3 with 2 cores--------------\n");
	i=TaskScheduler("samplefile3.txt", "feasibleschedule5.txt", 2);
	if(i==0){ printf("No feasible schedule!\n"); }
	else{ printf("samplefile3 with 2 cores is feasible\n"); }
	//There is no feasible schedule on 2 cores

	printf("-------------samplefile4 with 2 cores--------------\n");
	i=TaskScheduler("samplefile4.txt", "feasibleschedule6.txt", 2);
	if(i==0){ printf("No feasible schedule!\n"); }
	else{ printf("samplefile4 with 2 cores is feasible\n"); }
	//There is a feasible schedule on 2 cores

	return 0; 
}
