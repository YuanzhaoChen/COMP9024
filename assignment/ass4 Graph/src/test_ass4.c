// Author: Yuanzhao Chen
// Student ID: 5041686
// Platform: (select one of Windows, Unix, Mac): Mac(macOS Mojave)
#include<stdio.h>
#include<string.h>
#include"MyGraph.h"
#include"MyHeap.h"
#define PATH_LENGTH 100

void generatePath(char filePath[],char dataDir[],char srcFile[]){
    strcpy(filePath,dataDir);
    strcat(filePath,srcFile);
}

void runtest(){
    char dataDir[]="../data/";
    char BusStopsFile[]="BusStops.txt"; 
    char BusNamesFile[]="BusNames.txt";
    char BusRouteFile[]="BusRoute.txt";
    char DistanceFile[]="Distance.txt";

    char BusStopsFilePath[PATH_LENGTH];
    generatePath(BusStopsFilePath,dataDir,BusStopsFile);
    char BusNamesFilePath[PATH_LENGTH];
    generatePath(BusNamesFilePath,dataDir,BusNamesFile);
    char BusRouteFilePath[PATH_LENGTH];
    generatePath(BusRouteFilePath,dataDir,BusRouteFile);
    char DistanceFilePath[PATH_LENGTH];
    generatePath(DistanceFilePath,dataDir,DistanceFile);

    // test strongly connectivity
    printf("-----------------------------------------------------------------------------------\n");
    printf("|                             TEST STRONGLY CONNECTIVITY                          |\n");
    printf("-----------------------------------------------------------------------------------\n");
    if(StronglyConnectivity(BusStopsFilePath, BusNamesFilePath, BusRouteFilePath, DistanceFilePath)==0){
        printf("This graph is not strongly connected.\n");
    }else{
        printf("This graph is strongly connected.\n");
    }

    // test strongly connected components
    printf("-----------------------------------------------------------------------------------\n");
    printf("|                            TEST STRONGLY CONNECTED COMPONENT                    |\n");
    printf("-----------------------------------------------------------------------------------\n");
    maximalStronlyComponents(BusStopsFilePath, BusNamesFilePath, BusRouteFilePath, DistanceFilePath);
    
    // test reachable stops
    printf("-----------------------------------------------------------------------------------\n");
    printf("|                                 TEST REACHABLE STOPS                            |\n");
    printf("-----------------------------------------------------------------------------------\n");
    printf("When source bus stop is Albion Street, reachable stops are (BFS order):\n");
    reachableStops("Albion Street",BusStopsFilePath, BusNamesFilePath, BusRouteFilePath, DistanceFilePath);

    printf("\nWhen source bus stop is Broadway, reachable stops are (BFS order):\n");
    reachableStops("Broadway",BusStopsFilePath, BusNamesFilePath, BusRouteFilePath, DistanceFilePath);

    printf("\nWhen source bus stop is Harris Street, reachable stops are (BFS order):\n");
    reachableStops("Five Ways",BusStopsFilePath, BusNamesFilePath, BusRouteFilePath, DistanceFilePath);

    // test test travel routes
    printf("-----------------------------------------------------------------------------------\n");
    printf("|                                  TEST TRAVEL ROUTE                              |\n");
    printf("-----------------------------------------------------------------------------------\n");
    TravelRoute("Albion Street","Anzac Parade", BusStopsFilePath, BusNamesFilePath, BusRouteFilePath, DistanceFilePath);
    printf("\n");

    TravelRoute("Bridge Street","Blacktown",BusStopsFilePath, BusNamesFilePath, BusRouteFilePath, DistanceFilePath); 
    printf("\n");

    TravelRoute("Five Ways","Martin Place",BusStopsFilePath, BusNamesFilePath, BusRouteFilePath, DistanceFilePath); 
    printf("\n");

    TravelRoute("Goulburn","Hunter Street",BusStopsFilePath, BusNamesFilePath, BusRouteFilePath, DistanceFilePath); 
    printf("\n");

    TravelRoute("Bridge Street","Bondi Road",BusStopsFilePath, BusNamesFilePath, BusRouteFilePath, DistanceFilePath); 
    printf("\n");

    TravelRoute("Albion Street","Bridge Street",BusStopsFilePath, BusNamesFilePath, BusRouteFilePath, DistanceFilePath);
    printf("\n");

    TravelRoute("Albion Street","George Street",BusStopsFilePath, BusNamesFilePath, BusRouteFilePath, DistanceFilePath);
    printf("\n");
    
}

int main(){
    runtest();
    return 0;
}