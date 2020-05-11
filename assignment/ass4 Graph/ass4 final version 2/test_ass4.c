// Author: Yuanzhao Chen
// Student ID: 5041686
// Platform: (select one of Windows, Unix, Mac): Mac(macOS Mojave)
#include<stdio.h>
#include"MyGraph.h"
#include"MyBinomialHeaps.h"

void runtest(){
    // test strongly connectivity
    printf("-----------------------------------------------------------------------------------\n");
    printf("|                             TEST STRONGLY CONNECTIVITY                          |\n");
    printf("-----------------------------------------------------------------------------------\n");
    if(StronglyConnectivity("BusStops.txt","BustNames.txt","BusRoute.txt","Distance.txt")==0){
        printf("This graph is not strongly connected.\n");
    }else{
        printf("This graph is strongly connected.\n");
    }

    // test strongly connected components
    printf("-----------------------------------------------------------------------------------\n");
    printf("|                            TEST STRONGLY CONNECTED COMPONENT                    |\n");
    printf("-----------------------------------------------------------------------------------\n");
    maximalStronlyComponents("BusStops.txt","BustNames.txt","BusRoute.txt","Distance.txt");
    
    // test reachable stops
    printf("-----------------------------------------------------------------------------------\n");
    printf("|                                 TEST REACHABLE STOPS                            |\n");
    printf("-----------------------------------------------------------------------------------\n");
    printf("When source bus stop is Albion Street, reachable stops are (BFS order):\n");
    reachableStops("Albion Street","BusStops.txt","BusNames.txt","BusRoute.txt","Distance.txt");

    printf("\nWhen source bus stop is Broadway, reachable stops are (BFS order):\n");
    reachableStops("Broadway","BusStops.txt","BusNames.txt","BusRoute.txt","Distance.txt");

    printf("\nWhen source bus stop is Harris Street, reachable stops are (BFS order):\n");
    reachableStops("Harris Street","BusStops.txt","BusNames.txt","BusRoute.txt","Distance.txt");

    // test test travel routes
    printf("-----------------------------------------------------------------------------------\n");
    printf("|                                  TEST TRAVEL ROUTE                              |\n");
    printf("-----------------------------------------------------------------------------------\n");
    TravelRoute("Albion Street","Anzac Parade","BusStops.txt","BusNames.txt","BusRoute.txt","Distance.txt");
    printf("\n");
    TravelRoute("Bridge Street","Blacktown","BusStops.txt","BusNames.txt","BusRoute.txt","Distance.txt"); 
    printf("\n");
    TravelRoute("Great Western","Harris Street","BusStops.txt","BusNames.txt","BusRoute.txt","Distance.txt"); 
    printf("\n");
    TravelRoute("Albion Street","Bridge Street","BusStops.txt","BusNames.txt","BusRoute.txt","Distance.txt");
    printf("\n");
}

int main(){
    runtest();
    return 0;
}