/**
 * @file wasp_list.c
 * @brief Implementation of functions related to WaspClient, including initialization, addition, and freeing of WaspClients.
 *
 * This file contains the definitions for functions responsible for initializing, adding, and freeing WaspClient structures.
 * It also includes functions to find and print WaspClients within the game.
 */

#include "wasp_list.h"

WaspClient *initWaspClient(int n_wasps, int id_wasp){
    WaspClient *newWaspClient = (WaspClient *)malloc(sizeof(WaspClient));
    if(newWaspClient == NULL){
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    newWaspClient->id = id_wasp;
    newWaspClient->num_wasps = n_wasps;
    newWaspClient->next = NULL;
    for(int i = 0; i < n_wasps; i++){
        newWaspClient->wasps[i].direction = UP;
        newWaspClient->wasps[i].position.position_x = rand() % (WINDOW_WIDTH - 2) + 1;
        newWaspClient->wasps[i].position.position_y = rand() % (WINDOW_HEIGHT - 2) + 1;
        newWaspClient->wasps[i].on_board = 1;
    }
    return newWaspClient;
}

void addWaspClient(WaspClient **headWaspList, int n_wasps, int id_wasp){
    WaspClient *newWaspClient = initWaspClient(n_wasps, id_wasp);
    if(*headWaspList == NULL){
        *headWaspList = newWaspClient;
    }
    else{
        WaspClient *currentWaspClient = *headWaspList;
        while(currentWaspClient->next != NULL){
            currentWaspClient = currentWaspClient->next;
        }
        currentWaspClient->next = newWaspClient;
    }
}

WaspClient* findWaspClient(WaspClient **headWaspList, int id_wasp){
    WaspClient *currentWaspClient = *headWaspList;
    while(currentWaspClient != NULL){
        if(currentWaspClient->id == id_wasp){
            break;
        }
        currentWaspClient = currentWaspClient->next;
    }
    return currentWaspClient;
}

int countWasps(WaspClient *headWaspList){
    WaspClient *currentWaspClient = headWaspList;
    int count = 0;
    while(currentWaspClient != NULL){
        count += currentWaspClient->num_wasps;
        currentWaspClient = currentWaspClient->next;
    }
    return count;
}

void printWaspList(WaspClient *headWaspList){
    WaspClient *currentWaspClient = headWaspList;
    while(currentWaspClient != NULL){
        printf("WaspClient id: %d\n", currentWaspClient->id);
        currentWaspClient = currentWaspClient->next;
    }
}

void freeWaspList(WaspClient **headWaspList){
    WaspClient *currentWaspClient = *headWaspList;
    WaspClient *nextWaspClient = NULL;
    while(currentWaspClient != NULL){
        nextWaspClient = currentWaspClient->next;
        free(currentWaspClient);
        currentWaspClient = nextWaspClient;
    }
    *headWaspList = NULL;
}