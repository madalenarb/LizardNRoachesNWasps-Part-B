/**
 * @file roach_list.c
 * @brief Implementation of functions related to RoachClient, including initialization, addition, and freeing of RoachClients.
 *
 * This file contains the definitions for functions responsible for initializing, adding, and freeing RoachClient structures.
 * It also includes functions to find and print RoachClients within the game.
 */

#include "roach_list.h"

#include "utils.h"


RoachClient *initRoachClient(int n_roaches, int *score, int id_roach){
    RoachClient *newRoachClient = (RoachClient *)malloc(sizeof(RoachClient));
    if(newRoachClient == NULL){
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    newRoachClient->id = id_roach;
    newRoachClient->num_roaches = n_roaches;
    newRoachClient->next = NULL;
    for(int i = 0; i < n_roaches; i++){
        newRoachClient->roaches[i].score = score[i];
        newRoachClient->roaches[i].direction = UP;
        newRoachClient->roaches[i].position.position_x = rand() % (WINDOW_WIDTH - 2) + 1;
        newRoachClient->roaches[i].position.position_y = rand() % (WINDOW_HEIGHT - 2) + 1;
        newRoachClient->roaches[i].on_board = 1;
    }
    return newRoachClient;
}

void addRoachClient(int *score, int n_roaches, int id_roach){
    RoachClient *newRoachClient = initRoachClient(n_roaches, score, id_roach);
    if(gameState.headRoachList == NULL){
        gameState.headRoachList = newRoachClient;
    }
    else{
        RoachClient *currentRoachClient = gameState.headRoachList;
        while(currentRoachClient->next != NULL){
            currentRoachClient = currentRoachClient->next;
        }
        currentRoachClient->next = newRoachClient;
    }
}

RoachClient* findRoachClient(int id_roach){
    RoachClient *currentRoachClient = gameState.headRoachList;
    while(currentRoachClient != NULL){
        if(currentRoachClient->id == id_roach){
            break;
        }
        currentRoachClient = currentRoachClient->next;
    }
    return currentRoachClient;
}

void removeRoachClient(int id_roach){
    if(gameState.headRoachList == NULL){
        return;
    }
    
    RoachClient *temp = gameState.headRoachList, *prev = NULL;
    if(temp != NULL && temp->id == id_roach){
        gameState.headRoachList = temp->next;
        free(temp);
        return;
    }

    while(temp != NULL && temp->id != id_roach){
        prev = temp;
        temp = temp->next;
    }

    if(temp == NULL){
        return;
    }

    prev->next = temp->next;
    free(temp);
}

int countRoaches(){
    RoachClient *currentRoachClient = gameState.headRoachList;
    int count = 0;
    while(currentRoachClient != NULL){
        count += currentRoachClient->num_roaches;
        currentRoachClient = currentRoachClient->next;
    }
    return count;
}

void printRoachList(){
    RoachClient *currentRoachClient = gameState.headRoachList;
    while(currentRoachClient != NULL){
        printf("Roach client %d:\n", currentRoachClient->id);
        for(int i = 0; i < currentRoachClient->num_roaches; i++){
            printf("Roach %d: score: %d, position: (%d, %d), direction: %d, on_board: %d\n", i, currentRoachClient->roaches[i].score, currentRoachClient->roaches[i].position.position_x, currentRoachClient->roaches[i].position.position_y, currentRoachClient->roaches[i].direction, currentRoachClient->roaches[i].on_board);
        }
        currentRoachClient = currentRoachClient->next;
    }
}


void freeRoachList(){
    RoachClient *currentRoachClient = gameState.headRoachList;
    RoachClient *nextRoachClient = NULL;
    while(currentRoachClient != NULL){
        nextRoachClient = currentRoachClient->next;
        free(currentRoachClient);
        currentRoachClient = nextRoachClient;
    }
    gameState.headRoachList = NULL;
}