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
    char name[20];      // bus name is no more than 20 characters
    int id;             // serial number of a bus stop
}BusStop;

struct vertex;
typedef struct edge{ 
    struct vertex *dst; // destination vertex (source vertex stored in super class)?????????
    int distance;       // the length of the edge
}Edge;

typedef struct heap_node{
    int key;            // key is the distance of thisEdge
    Edge *thisEdge;     // that's what we want to store in a node
    int level;          // child node's level = 1 + parent node's level
    struct heap_node *parent;   // trace this node for upheap
    struct heap_node *leftChild;
    struct heap_node *rightChild;
}HeapNode;

typedef struct priority_queue{
    HeapNode *root;
    HeapNode *lastNode; // rightmost node on higest level, useful for removeMin
    int nNode;          // number of heap nodes 
    int height;         // height of the tree, aka max level
}PriorityQueue;

// Store both vertex and its edges
typedef struct vertex{ 
    BusStop *thisBusStop;
    PriorityQueue *edgesQueue;
    int distanceFromStartVertex;
    int isMarked;
}Vertex;

typedef struct graphRep{
    Vertex **vertices;  // store an array of vertex pointer
    int size;           // max number of vertices that can be stored in graph
    int nV;             // number of vertices currently stored in graph
    int nE;             // number of edges currently stored in graph
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
HeapNode *newHeapNode(Edge *thisEdge){
    HeapNode *thisHeapNode=malloc(sizeof(HeapNode));
    assert(thisHeapNode!=NULL);
    thisHeapNode->key=thisEdge->distance;
    thisHeapNode->thisEdge=thisEdge;
    thisHeapNode->level=0;
    thisHeapNode->parent=NULL;
    thisHeapNode->leftChild=NULL;
    thisHeapNode->rightChild=NULL;
    return thisHeapNode;
}

// Return a new PriorityQueue
PriorityQueue *newPriorityQueue(){
    PriorityQueue *pq=malloc(sizeof(PriorityQueue));
    assert(pq!=NULL);
    pq->root=NULL;
    pq->lastNode=NULL;
    pq->nNode=0;
    pq->height=0;
    return pq; 
}

// Helper function of PQ_insert, use in-order traversal to find correct place to insert
void PQ_insert_helper(HeapNode *root, HeapNode *thisNode, int PQ_height){
    if(root==NULL){return;}
    PQ_insert_helper(root->leftChild,thisNode,PQ_height);
    if(root->leftChild==NULL && root->level==PQ_height-1){          // insert to left child   
        root->leftChild=thisNode;
        thisNode->parent=root;
        thisNode->level=root->level+1;
    }else if(root->rightChild==NULL && root->level==PQ_height-1){   //insert to right child
        root->rightChild=thisNode;
        thisNode->parent=root;
        thisNode->level=root->level+1;
    }
    PQ_insert_helper(root->rightChild,thisNode,PQ_height);
}

// Swap two heap nodes, parent-child relationship is also inherited
void swapNode(HeapNode *highNode, HeapNode *lowNode){
    
}

// Bubble up this new node to the correct place
void upHeap(PriorityQueue *pq,HeapNode *thisNode){
    HeapNode *refNode=thisNode;
    int isChanged=0;
    while(refNode->parent!=NULL && refNode->parent->key>refNode->key){
        if(isChanged==0){
            pq->lastNode=refNode->parent;   // if thisNode is bubbled up then lastNode becomes it's parent
            isChanged=1;
        }
        swapNode(refNode,refNode->parent);  // first argument of must be further from root
    }
}

// Add heap node to priority queue
void PQ_insert(PriorityQueue *pq,Vertex *thisVertex,int distance){
    HeapNode *thisNode=newHeapNode(newEdge(thisVertex,distance));
    //printf("before insert, nNoed: %d,capacity: %d\n",pq->nNode,(int)pow(2,pq->height+1)-1);
    if(pq->root==NULL){
        pq->root=thisNode; 
        pq->height=0;
    }else{
        if(pq->nNode==(int)pow(2,pq->height+1)-1){  // full at pq->height
            HeapNode *refNode=pq->root;
            while(refNode->leftChild!=NULL){        // insert to bottom left 
                refNode=refNode->leftChild; 
            }
            refNode->leftChild=thisNode;
            thisNode->parent=refNode;
            thisNode->level=refNode->level+1;
            pq->height+=1;
        }else{                                      // still have a spare spot for current height, height unchanged
            PQ_insert_helper(pq->root,thisNode,pq->height);
        }
    }
    pq->nNode+=1;
    pq->lastNode=thisNode;
    upHeap(pq,thisNode);
    //printf("after insert, nNode: %d, capacity: %d\n\n",pq->nNode,(int)pow(2,pq->height+1)-1);
}

// Remove min heap node from priority queue
HeapNode *PQ_removeMin(PriorityQueue *pq){
    HeapNode *minNode=pq->root;
    //DO SOMETHING
    return minNode;
}

// Return a new vertex
Vertex *newVertex(char busStopName[],int busStopId){
    BusStop *thisBusStop=newBustStop(busStopName,busStopId);
    Vertex *v=malloc(sizeof(Vertex));
    assert(v!=NULL);
    v->thisBusStop=thisBusStop;
    v->edgesQueue=newPriorityQueue();       // each vertex has a unique priority queue to store neighbour vertices
    v->distanceFromStartVertex=INF_DISTANCE;// measure the distance between this vertex and start point, useful in Dijkstra
    v->isMarked=0;
    return v;
}

// Return a graph which can store n vertices
Graph newGraph(int n) {
    assert(n>0);
    Graph g=malloc(sizeof(GraphRep));
    assert(g!=NULL);
    g->size=n;  // this graph can store n vertices at most
    g->nV=0;    // number of vertices, currently no vertex in graph
    g->nE=0;    // number of edges, currently no edge in graph
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
    g->vertices[g->nV]=thisVertex;  // free spot in g, since BusStop.txt serial number is ascending,
    g->nV+=1;                       // g->nV = thisVertex->thisBusStop->id !
}

// Insert existing edges to graph
void insertEdge(Graph g, int FromBusId, int ToBusId, int distance){
    assert(g!=NULL);
    Vertex *src=g->vertices[FromBusId];        // by default the bus id is exacatly the index of where it's vertex is stored in vertices 
    Vertex *dst=g->vertices[ToBusId];
    assert(src!=NULL && dst!=NULL);
    //printf("Add edge %d->%d(%d)\n",FromBusId,ToBusId,distance);
    PQ_insert(src->edgesQueue,dst,distance);    // insert the edge to the corresponding priority queue
    g->nE+=1;
}

//---------------------------------------- Declare functions to be implemented ----------------------------------------
int StronglyConnectivity(const char *busStops,const char *BusNames,const char *BusRoutes,const char *Distance);
void maximalStronlyComponents(const char *busStops,const char *BusNames,const char *BusRoutes,const char *Distance);
void reachableStops(const char *sourceStop,const char *busStops,const char *BusNames,const char *BusRoutes,const char *Distance);
void TravelRoute(const char *sourceStop,const char *destinationStop,const char *busStops,const char *BusNames,const char *BusRoutes,const char *Distance);
//---------------------------------------------------------------------------------------------------------------------
//------------------------------------------- Declare other helper functions ------------------------------------------
Graph makeGraphFromFiles();
int findLineNum(char fileName[]);
void uploadVertex(Graph g,char fileName[]);
void uploadEdges(Graph g,char fileName[]);
void unmarkAllVertices(Graph g);
//---------------------------------------------------------------------------------------------------------------------
// Construct graph from BusStops.txt (where vertices are stored), Distance.txt (where edges are stored)
Graph makeGraphFromFiles(){
    Graph g=newGraph(findLineNum("BusStops.txt"));
    uploadVertex(g,"BusStops.txt");
    uploadEdges(g,"Distance.txt");
    printf("number of vertices in graph: %d\n",g->nV);
    printf("number of edges in graph: %d\n",g->nE);
    return g;
}

// Find how many lines the txt file has
int findLineNum(char fileName[]){
    int n=0;
    FILE*fp=fopen(fileName,"r");
    char line[LINE_LENGTH];
    while(fgets(line,LINE_LENGTH,fp)!=NULL){ n+=1; }
    fclose(fp);
    return n;
}

// Initially there's no vertices in graph, now update vertex information from fileName
void uploadVertex(Graph g,char fileName[]){
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
void uploadEdges(Graph g,char fileName[]){
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

void dfs(Vertex *thisVertex){
}

int StronglyConnectivity(const char *busStops,const char *BusNames,const char *BusRoutes,const char *Distance){
    return 0;
}

void runtest(){
    Graph g=makeGraphFromFiles();
}

int main(){
    runtest();    
    return 0;
}