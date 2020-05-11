// Author: Yuanzhao Chen
// Student ID: 5041686
// Platform: (select one of Windows, Unix, Mac): Mac(macOS Mojave)
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>
#include<math.h>
#include "MyGraph.h"
#include "MyList.h"
#include "MyHeap.h"
#define INF_DISTANCE 10000000
#define LINE_LENGTH 100


// Return a new busStop
BusStop *newBusStop(char busStopName[],int busStopId){
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

// Return a new vertex
Vertex *newVertex(char busStopName[],int busStopId){
    BusStop *thisBusStop=newBusStop(busStopName,busStopId);
    Vertex *v=malloc(sizeof(Vertex));
    assert(v!=NULL);
    v->thisBusStop=thisBusStop;
    v->edgesQueue=newList();                // each vertex has a unique priority queue to store neighbour vertices
    v->reverse_edgesQueue=newList();        // store the predecessor of the new vertex, useful in strong connectivity algorithm
    v->previousVertexInsShortestPath=NULL;  // record it's previous vertx in the shortest path, so we can backtrace the route after Dijkstra
    v->distanceFromStartVertex=INF_DISTANCE;// measure the distance between this vertex and start point, useful in Dijkstra
    v->isMarked=0;
    return v;
}

// Return a graph which can store n vertices
Graph newGraph(int n){
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
    appendList(src->edgesQueue,newEdge(dst,distance));    // insert the edge to the corresponding priority queue
    appendList(dst->reverse_edgesQueue,newEdge(src,distance));
    g->nE+=1;
}

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

// Mark vertices that can be visited from thisVertex
void dfs(Vertex *thisVertex){
    assert(thisVertex!=NULL && thisVertex->edgesQueue!=NULL);
	if(thisVertex->isMarked==1){return;}
    thisVertex->isMarked=1; // if this vertex is called by dfs() then it is mark as visted 
	ListNode *curr=thisVertex->edgesQueue->firstNode;
    while(curr!=NULL){
		dfs(curr->thisEdge->dst);
		curr=curr->next;
    }
}

// Mark vertices that can be visited from thisVertex, through reverse edge
void reverse_dfs(Vertex *thisVertex){
    assert(thisVertex!=NULL && thisVertex->reverse_edgesQueue!=NULL);
	if(thisVertex->isMarked==1){return;}
	thisVertex->isMarked=1;
	ListNode *curr=thisVertex->reverse_edgesQueue->firstNode;
    while(curr!=NULL){
		reverse_dfs(curr->thisEdge->dst);
		curr=curr->next;
    }
}

// Print the names of all the bus stops reachable from the bus stop in breadth-first-search order
void bfs(Vertex *thisVertex){
	assert(thisVertex!=NULL);
	List *seq=newList();
	thisVertex->isMarked=1;
	appendList(seq,newEdge(thisVertex,0));	// in order to make use of appendList to store vertex, we have to create a edge
											// distance is meaningless here so can be any integer, here we set to 0
	while(seq->size!=0){
		ListNode *thisNode=popList(seq);
		Vertex *thatVertex=thisNode->thisEdge->dst;
		printf("=> [%s]\n",thatVertex->thisBusStop->name);
		ListNode*curr=thatVertex->edgesQueue->firstNode;
		int size=thatVertex->edgesQueue->size;
		for(int i=0;i<size;i+=1){
			if(curr->thisEdge->dst->isMarked==0){
				curr->thisEdge->dst->isMarked=1;
				appendList(seq,curr->thisEdge);
			}
			curr=curr->next;
		}
	}
	freeList(seq);
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
	printf("Number of vertices can be visited through forward propagation: %d\n",nVisited);
    if(nVisited!=g->nV){return 0;}
    nVisited=0;
    unmarkAllVertices(g);
    reverse_dfs(g->vertices[0]);
    for(int i=0;i<g->nV;i+=1){
        if(g->vertices[i]->isMarked==1){ nVisited+=1; }
    }
	printf("Number of vertices can be visited through backwards propagation: %d\n",nVisited);
    if(nVisited!=g->nV){return 0;}
    return 1;
}

// explore vertex in dfs, also push vertex on stack when it can't go to anywhere else
void maximalStronlyComponents_helper_1(Vertex *thisVertex,List *L){
    assert(thisVertex!=NULL && thisVertex->edgesQueue!=NULL);
	if(thisVertex->isMarked==1){return;}
    thisVertex->isMarked=1; // if this vertex is called by dfs() then it is mark as visted 
	ListNode *curr=thisVertex->edgesQueue->firstNode;
    while(curr!=NULL){
		maximalStronlyComponents_helper_1(curr->thisEdge->dst,L);
		curr=curr->next;
    }    
    pushList(L,newEdge(thisVertex,0));
}

// explore vertex reversely
void maximalStronlyComponents_helper_2(Vertex*thisVertex){
    assert(thisVertex!=NULL && thisVertex->reverse_edgesQueue!=NULL);
	if(thisVertex->isMarked==1){return;}
	thisVertex->isMarked=1;
    printf("=> [%s]\n",thisVertex->thisBusStop->name);
	ListNode *curr=thisVertex->reverse_edgesQueue->firstNode;
    while(curr!=NULL){
		maximalStronlyComponents_helper_2(curr->thisEdge->dst);
		curr=curr->next;
    }
}

// Find strongly connected componets by Kosaraju’s algorithm
void maximalStronlyComponents(const char *busStops,const char *BusNames,const char *BusRoutes,const char *Distance){
    Graph g=makeGraphFromFiles(busStops,BusNames,BusRoutes,Distance);
    unmarkAllVertices(g);
    List *L=newList();  // use List as a stack, since push and pop are implmented this is feasible 
    for(int i=0;i<g->nV;i+=1){
        if(g->vertices[i]->isMarked==1){
            continue;
        }
        maximalStronlyComponents_helper_1(g->vertices[i],L);
    }
    unmarkAllVertices(g);
    int component_num=0;
    while(L->size!=0){
        ListNode *curr=popList(L);
        if(curr->thisEdge->dst->isMarked==1){
            continue;
        }
        printf("\nStrongly connected component %d:\n",component_num);
        maximalStronlyComponents_helper_2(curr->thisEdge->dst);
        component_num+=1;
    }
    printf("\nIn total there are %d strongly connected components in the bus network\n",component_num);
    freeList(L);
}

// Return the bus stop id (serial number) of the bus
int findBusStop(Graph g, const char*busName){
	int busId=-1;		// by defalut all bus stop serial numbers are non negative
	for(int i=0;i<g->nV;i+=1){
		if(strcmp(g->vertices[i]->thisBusStop->name,busName)==0){
			busId=i;	// find corresponding vetex of the sourceStop
			break;
		}
	}
	return busId;
}

void reachableStops(const char *sourceStop,const char *busStops,const char *BusNames,const char *BusRoutes,const char *Distance){
	Graph g=makeGraphFromFiles(busStops,BusNames,BusRoutes,Distance);
    unmarkAllVertices(g);
	int busStopId=findBusStop(g,sourceStop);
	if(busStopId==-1){
		printf("This bus stop is not in graph!\n");
		return;
	}
	bfs(g->vertices[busStopId]);
}

void relax(struct binary_heap *routeHeap,Vertex *thisVertex){
    ListNode *curr=thisVertex->edgesQueue->firstNode;                       // explore each edge of the vertex to see whether we can relax
    while(curr!=NULL){
        if(curr->thisEdge->dst->isMarked==1){                               // if this vertex is something that has been taken out of heap, 
            curr=curr->next;                                                // then don't look at it (shortest path already found)
            continue;
        }
        if(thisVertex->distanceFromStartVertex+curr->thisEdge->distance\
          <curr->thisEdge->dst->distanceFromStartVertex){
            BinaryHeap_changeNode(routeHeap,curr->thisEdge->dst->positionInHeap,thisVertex->distanceFromStartVertex + curr->thisEdge->distance);
            curr->thisEdge->dst->previousVertexInsShortestPath=thisVertex;  // now we know one thing: the shortest path to curr->thisEdge->dst 
        }                                                                   // must pass thisVertex beforehand
        curr=curr->next;
    }
}

void printRoute(Graph g,int srcStopId, int dstStopID){
    printf("Shortest route from [%s] to [%s]:\n",g->vertices[srcStopId]->thisBusStop->name,g->vertices[dstStopID]->thisBusStop->name);
    Vertex *currV=g->vertices[dstStopID];
    if(currV->distanceFromStartVertex==INF_DISTANCE){
        printf("No bus route is currently available, consider take a taxi?\n");
        return;
    }
    List *L=newList();
    while(currV!=g->vertices[srcStopId]){
        pushList(L,newEdge(currV,0));   // the is just to save a vertex to a list, distance is useless here so can be an arbitray integer
        currV=currV->previousVertexInsShortestPath;
    }
    pushList(L,newEdge(currV,0));
    ListNode *currL=L->firstNode;
    while(currL!=NULL){
        printf("=> [%s]\n",currL->thisEdge->dst->thisBusStop->name);
        currL=currL->next;
    }
    printf("Total distance is %d\n",g->vertices[dstStopID]->distanceFromStartVertex);
    freeList(L);
}

void TravelRoute(const char *sourceStop,const char *destinationStop,const char *busStops,\
const char *BusNames,const char *BusRoutes,const char *Distance){
	Graph g=makeGraphFromFiles(busStops,BusNames,BusRoutes,Distance);
    unmarkAllVertices(g);
	int srcStopId=findBusStop(g,sourceStop);
	if(srcStopId==-1){
		printf("Source stop is not in graph!\n");
		return;
	}
    g->vertices[srcStopId]->distanceFromStartVertex=0;
	int dstStopID=findBusStop(g,destinationStop);
	if(dstStopID==-1){
		printf("Destination stop is not in graph!\n");
		return;
	}
    
    struct binary_heap *routeHeap=newBinaryHeap(g->nV);
    
    for(int id=0;id<g->nV;id+=1){
        BinaryHeap_addNode(routeHeap,g->vertices[id]);
    }
    
    while(routeHeap->n>1){ // binary heap's size>=1
        Vertex *v=BinaryHeap_removeMin(routeHeap);
        assert(v!=NULL);
        relax(routeHeap,v);
        v->isMarked=1;
    }
    
    printRoute(g,srcStopId,dstStopID);
}


