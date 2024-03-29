// Author: Yuanzhao Chen
// Student ID: 5041686
// Platform: (select one of Windows, Unix, Mac): Mac(macOS Mojave)
#ifndef MYGRAPH_H
#define MYGRAPH_H
#include "MyList.h"
#include "MyHeap.h"
typedef struct bus_stop{
    char name[20];              // bus name is no more than 20 characters
    int id;                     // serial number of a bus stop
}BusStop;

struct vertex;
typedef struct edge{ 
    struct vertex *dst;         // destination vertex (source vertex stored in super class)?????????
    int distance;               // the length of the edge
}Edge;

// Store both vertex and its edges
typedef struct vertex{ 
    BusStop *thisBusStop;
    struct list *edgesQueue;
    struct list *reverse_edgesQueue;
    struct vertex *previousVertexInsShortestPath;
    int distanceFromStartVertex;
    int positionInHeap;         // the position of the vertex in binary heap
    int isMarked;
}Vertex;

typedef struct graphRep{
    Vertex **vertices;          // store an array of vertex pointer
    int size;                   // max number of vertices that can be stored in graph
    int nV;                     // number of vertices currently stored in graph
    int nE;                     // number of edges currently stored in graph
}GraphRep;
typedef GraphRep *Graph;
struct binary_heap;
//----------------------------- functions implemented in MyGraph.c -----------------------
BusStop *newBusStop(char busStopName[],int busStopId);
Edge *newEdge(Vertex *v,int distance);
Vertex *newVertex(char busStopName[],int busStopId);
Graph newGraph(int n);
void insertVertex(Graph g, Vertex *v);
void insertEdge(Graph g, int FromBusId, int ToBusId, int distance);
Graph makeGraphFromFiles(const char *busStops,const char *BusNames,const char *BusRoutes,const char *Distance);
int findLineNum(const char fileName[]);
void uploadVertex(Graph g,const char fileName[]);
void uploadEdges(Graph g,const char fileName[]);
void unmarkAllVertices(Graph g);
void dfs(Vertex *v);
void reverse_dfs(Vertex *v);
void bfs(Vertex *v);
int findBusStop(Graph g, const char*busName);
void relax(struct binary_heap *routeHeap,Vertex *v);
void maximalStronlyComponents_helper_1(Vertex *v,struct list *L);
void maximalStronlyComponents_helper_2(Vertex *v);
void freeGraph(Graph g);
void freeVertex(Vertex *v);
void freeEdge(Edge *e);

int StronglyConnectivity(const char *busStops,const char *BusNames,const char *BusRoutes,const char *Distance);
void maximalStronlyComponents(const char *busStops,const char *BusNames,const char *BusRoutes,const char *Distance);
void reachableStops(const char *sourceStop,const char *busStops,const char *BusNames,const char *BusRoutes,const char *Distance);
void TravelRoute(const char *sourceStop,const char *destinationStop,const char *busStops,const char *BusNames,const char *BusRoutes,const char *Distance);
#endif
