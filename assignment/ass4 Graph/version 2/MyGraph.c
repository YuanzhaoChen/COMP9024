// Author: Yuanzhao Chen
// Student ID: 5041686
// Platform: (select one of Windows, Unix, Mac): Mac(macOS Mojave)
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>
#include<math.h>
#define INF_DISTANCE 10000000
#define LINE_LENGTH 100

typedef struct bus_stop{
    char name[20];              // bus name is no more than 20 characters
    int id;                     // serial number of a bus stop
}BusStop;

struct vertex;
typedef struct edge{ 
    struct vertex *dst;         // destination vertex (source vertex stored in super class)?????????
    int distance;               // the length of the edge
}Edge;

typedef struct HeapNode { 
	int key;                    // key is the distance of Edge 
	Edge *thisEdge;
	struct HeapNode *parentNode;
	struct HeapNode *rightSibling;
	struct HeapNode *leftMostChild;
	int degree;
} HeapNode;

typedef struct BinomialHeap{    
	int  size;	                // count of items in the heap
	HeapNode *firstTreeRoot;    // lowest degree bin-tree in heap
	HeapNode *lastTreeRoot;     // highest degree bin-tree in heap 
} BinomialHeap;

// Store both vertex and its edges
typedef struct vertex{ 
    BusStop *thisBusStop;
    BinomialHeap *edgesQueue;
    BinomialHeap *reverse_edgesQueue;
    int distanceFromStartVertex;
    int isMarked;
}Vertex;

typedef struct graphRep{
    Vertex **vertices;          // store an array of vertex pointer
    int size;                   // max number of vertices that can be stored in graph
    int nV;                     // number of vertices currently stored in graph
    int nE;                     // number of edges currently stored in graph
}GraphRep;
typedef GraphRep *Graph;

//---------------------------------------------------------------------------------------------------------------------

// Return a new busStop
BusStop *newBustStop(char busStopName[],int busStopId){
    BusStop *thisBusStop=malloc(sizeof(BusStop));
    assert(thisBusStop!=NULL);
    strcpy(thisBusStop->name,busStopName);
    thisBusStop->id=busStopId;
    return thisBusStop;
}

// Return a new Edge
Edge *newEdge(Vertex *thisVertex,int distance){
    Edge *thisEdge=malloc(sizeof(Edge));
    assert(thisEdge!=NULL);
    thisEdge->dst=thisVertex;
    thisEdge->distance=distance;
    return thisEdge;
}

// Return a new HeapNode
HeapNode *newHeapNode(Edge*thisEdge){
	HeapNode *newNode=malloc(sizeof(HeapNode));
	assert(newNode != NULL);
	newNode->key=thisEdge->distance;
    newNode->thisEdge=thisEdge;
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
void InsertToHeap(BinomialHeap *T, Edge *thisEdge){
	assert(T!=NULL);
	HeapNode *new=newHeapNode(thisEdge);
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

// Return a new vertex
Vertex *newVertex(char busStopName[],int busStopId){
    BusStop *thisBusStop=newBustStop(busStopName,busStopId);
    Vertex *v=malloc(sizeof(Vertex));
    assert(v!=NULL);
    v->thisBusStop=thisBusStop;
    v->edgesQueue=newHeap();                // each vertex has a unique priority queue to store neighbour vertices
    v->reverse_edgesQueue=newHeap();        // store the predecessor of the new vertex, useful in strong connectivity algorithm
    v->distanceFromStartVertex=INF_DISTANCE;// measure the distance between this vertex and start point, useful in Dijkstra
    v->isMarked=0;
    return v;
}

// Return a graph which can store n vertices
Graph newGraph(int n) {
    assert(n>0);
    Graph g=malloc(sizeof(GraphRep));
    assert(g!=NULL);
    g->size=n;                              // this graph can store n vertices at most
    g->nV=0;                                // number of vertices, currently no vertex in graph
    g->nE=0;                                // number of edges, currently no edge in graph
    g->vertices=malloc(n*sizeof(Vertex *)); // a pointer that points to n Vertex pointers 
    assert(g->vertices!=NULL);
    for(int i=0;i<n;i+=1){  
        g->vertices[i]=NULL;   
    }
    return g;
}

// Insert a new vertex to graph
void insertVertex(Graph g, Vertex *thisVertex){
    assert(g!=NULL && g->nV<=g->size && thisVertex!=NULL);
    g->vertices[g->nV]=thisVertex;          // free spot in g, since BusStop.txt serial number is ascending,
    g->nV+=1;                               // g->nV = thisVertex->thisBusStop->id !
}

// Insert existing edges and reverse edges to graph
void insertEdge(Graph g, int FromBusId, int ToBusId, int distance){
    assert(g!=NULL);
    Vertex *src=g->vertices[FromBusId];                     // the bus id is exacatly the index of where it's vertex is stored in vertices 
    Vertex *dst=g->vertices[ToBusId];
    assert(src!=NULL && dst!=NULL);
    InsertToHeap(src->edgesQueue,newEdge(dst,distance));    // insert the edge to the corresponding priority queue
    InsertToHeap(dst->reverse_edgesQueue,newEdge(src,distance));
    g->nE+=1;
}

//---------------------------------------- Declare functions to be implemented ----------------------------------------
int StronglyConnectivity(const char *busStops,const char *BusNames,const char *BusRoutes,const char *Distance);
void maximalStronlyComponents(const char *busStops,const char *BusNames,const char *BusRoutes,const char *Distance);
void reachableStops(const char *sourceStop,const char *busStops,const char *BusNames,const char *BusRoutes,const char *Distance);
void TravelRoute(const char *sourceStop,const char *destinationStop,const char *busStops,const char *BusNames,const char *BusRoutes,const char *Distance);
//---------------------------------------------------------------------------------------------------------------------
//------------------------------------------- Declare other helper functions ------------------------------------------
Graph makeGraphFromFiles(const char *busStops,const char *BusNames,const char *BusRoutes,const char *Distance);
int findLineNum(const char fileName[]);
void uploadVertex(Graph g,const char fileName[]);
void uploadEdges(Graph g,const char fileName[]);
void unmarkAllVertices(Graph g);
//---------------------------------------------------------------------------------------------------------------------
// Construct graph from BusStops.txt (where vertices are stored), Distance.txt (where edges are stored)
Graph makeGraphFromFiles(const char *busStops,const char *BusNames,const char *BusRoutes,const char *Distance){
    Graph g=newGraph(findLineNum(busStops));
    uploadVertex(g,busStops);
    uploadEdges(g,Distance);
    return g;
}

// Find how many lines the txt file has
int findLineNum(const char fileName[]){
    int n=0;
    FILE*fp=fopen(fileName,"r");
    char line[LINE_LENGTH];
    while(fgets(line,LINE_LENGTH,fp)!=NULL){ n+=1; }
    fclose(fp);
    return n;
}

// Initially there's no vertices in graph, now update vertex information from fileName
void uploadVertex(Graph g,const char fileName[]){
    FILE*fp=fopen(fileName,"r");
    assert(fp!=NULL);
    char line[LINE_LENGTH];
    while(fgets(line,LINE_LENGTH,fp)!=NULL){    //decompose each line into name and id
        char busStopName[20];
        int busStopId;
        char *token=strtok(line,":");
        busStopId=atoi(token);
        token=strtok(NULL,":");
        strcpy(busStopName,token);
        if (strlen(busStopName)-1>=0 && busStopName[strlen(busStopName)-1]=='\n'){
            busStopName[strlen(busStopName)-1]='\0'; //do not include '\n' in BusStopName
        }
        insertVertex(g,newVertex(busStopName,busStopId));
    }
    fclose(fp);
}

// After vertices are loaded from file to graph, we can update the edge information of different vertices
void uploadEdges(Graph g,const char fileName[]){
    FILE*fp=fopen(fileName,"r");
    assert(fp!=NULL);
    char line[LINE_LENGTH];
    while(fgets(line,LINE_LENGTH,fp)!=NULL){    // extract busId1, busId2, distance
        if(strcmp(line,"\n")==0){continue;}
        int fromBusId=0,toBusId=0,distance=0;
        int state=0;                            // state 0: reading fromBusId, state 1: reading toBusId, state 3: reading distance
        for(int i=0;i<strlen(line);i+=1){       // read info in each line
            if(line[i]=='-'){
                state=1;
            }else if(line[i]==':'){
                state=2;
            }
            if(line[i]<'0'||line[i]>'9'){continue;}
            if(state==0){
                fromBusId=fromBusId*10+line[i]-'0';
            }else if(state==1){
                toBusId=toBusId*10+line[i]-'0';
            }else if(state==2){
                distance=distance*10+line[i]-'0';
            }
        }
        insertEdge(g,fromBusId,toBusId,distance);
    }
    fclose(fp);
}

// Set all vertices as unmarked
void unmarkAllVertices(Graph g){
    for(int i=0;i<g->nV;i+=1){
        g->vertices[i]->isMarked=0;
    }
}

// mark vertices that can be visited from thisVertex
void dfs(Vertex *thisVertex){
    assert(thisVertex!=NULL && thisVertex->edgesQueue!=NULL);
    thisVertex->isMarked=1; // if this vertex is called by dfs() then it is mark as visted 
    while(thisVertex->edgesQueue->size!=0){
        Vertex *nextStop=RemoveMin(thisVertex->edgesQueue)->thisEdge->dst;
        dfs(nextStop);
    }
}

// mark vertices that can be visited from thisVertex, through reverse edge
void reverse_dfs(Vertex *thisVertex){
    assert(thisVertex!=NULL && thisVertex->reverse_edgesQueue!=NULL);
    while(thisVertex->reverse_edgesQueue->size!=0){
        Vertex *nextStop=RemoveMin(thisVertex->reverse_edgesQueue)->thisEdge->dst;
        reverse_dfs(nextStop);
    }
}

// Compute strong connectivity algorithms (see note week8 page 19)
// Run time complexity: O(M+N), where M is the number of verices and N is the number of edges in graph 
int StronglyConnectivity(const char *busStops,const char *BusNames,const char *BusRoutes,const char *Distance){
    // pick this first vertex as start point for convenience, can be any other vertex
    Graph g=makeGraphFromFiles(busStops,BusNames,BusRoutes,Distance);
    dfs(g->vertices[0]);
    int nVisited=0;
    for(int i=0;i<g->nV;i+=1){
        if(g->vertices[i]->isMarked==1){ nVisited+=1; } // count number of visited vertices  
    }
    if(nVisited!=g->nV){return 0;}
    nVisited=0;
    unmarkAllVertices(g);
    reverse_dfs(g->vertices[0]);
    for(int i=0;i<g->nV;i+=1){
        if(g->vertices[i]->isMarked==1){ nVisited+=1; }
    }
    if(nVisited!=g->nV){return 0;}
    return 1;
}

void maximalStronlyComponents(const char *busStops,const char *BusNames,const char *BusRoutes,const char *Distance){
    Graph g=makeGraphFromFiles(busStops,BusNames,BusRoutes,Distance);
}
void runtest(){
    // test strongly connectivity
    if(StronglyConnectivity("BusStops.txt","BustNames.txt","BusRoute.txt","Distance.txt")==0){
        printf("This graph is not strongly connected, answer is correct.\n");
    }else{
        printf("Answer of strongly connectivity is wrong.\n");
    }
    // test strongly connectivity components
    maximalStronlyComponents("BusStops.txt","BustNames.txt","BusRoute.txt","Distance.txt");
}

int main(){
    runtest();    
    return 0;
}