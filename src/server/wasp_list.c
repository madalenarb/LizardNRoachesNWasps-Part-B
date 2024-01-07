/**
 * @file wasp_list.c
 * @brief Implementation of functions related to WaspClient, including initialization, addition, and freeing of WaspClients.
 *
 * This file contains the definitions for functions responsible for initializing, adding, and freeing WaspClient structures.
 * It also includes functions to find and print WaspClients within the game.
 */

#include "wasp_list.h"

#include "utils.h"

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

void addWaspClient(int n_wasps, int id_wasp){
    WaspClient *newWaspClient = initWaspClient(n_wasps, id_wasp);
    if(gameState.headWaspList == NULL){
        gameState.headWaspList = newWaspClient;
    }
    else{
        WaspClient *currentWaspClient = gameState.headWaspList;
        while(currentWaspClient->next != NULL){
            currentWaspClient = currentWaspClient->next;
        }
        currentWaspClient->next = newWaspClient;
    }
}

WaspClient* findWaspClient(int id_wasp){
    WaspClient *currentWaspClient = gameState.headWaspList;
    while(currentWaspClient != NULL){
        if(currentWaspClient->id == id_wasp){
            break;
        }
        currentWaspClient = currentWaspClient->next;
    }
    return currentWaspClient;
}

int countWasps(){
    WaspClient *currentWaspClient = gameState.headWaspList;
    int count = 0;
    while(currentWaspClient != NULL){
        count += currentWaspClient->num_wasps;
        currentWaspClient = currentWaspClient->next;
    }
    return count;
}

void removeWaspClient(int id_wasp){
    if(gameState.headWaspList == NULL){
        return;
    }
    
    WaspClient *temp = gameState.headWaspList, *prev = NULL;
    if(temp != NULL && temp->id == id_wasp){
        gameState.headWaspList = temp->next;
        free(temp);
        return;
    }
    while(temp != NULL && temp->id != id_wasp){
        prev = temp;
        temp = temp->next;
    }
    if(temp == NULL){
        return;
    }
    prev->next = temp->next;
    free(temp);
}

void printWaspList(){
    if(gameState.headWaspList == NULL){
        printf("No wasps in the list.\n");
        return;
    }
    WaspClient *currentWaspClient = gameState.headWaspList;
    while(currentWaspClient != NULL){
        printf("WaspClient id: %d\n", currentWaspClient->id);
        for(int i = 0; i < currentWaspClient->num_wasps; i++){
            printf("Wasp %d: position (%d, %d), direction %d, on_board %d\n", i, currentWaspClient->wasps[i].position.position_x, currentWaspClient->wasps[i].position.position_y, currentWaspClient->wasps[i].direction, currentWaspClient->wasps[i].on_board);
        }
        currentWaspClient = currentWaspClient->next;
    }
}

void freeWaspList(){
    WaspClient *currentWaspClient = gameState.headWaspList;
    WaspClient *nextWaspClient = NULL;
    while(currentWaspClient != NULL){
        nextWaspClient = currentWaspClient->next;
        free(currentWaspClient);
        currentWaspClient = nextWaspClient;
    }
    gameState.headWaspList = NULL;
}