/**
 * @file roach_list.c
 * @brief Implementation of functions related to RoachClient, including initialization, addition, and freeing of RoachClients.
 *
 * This file contains the definitions for functions responsible for initializing, adding, and freeing RoachClient structures.
 * It also includes functions to find and print RoachClients within the game.
 */

#include "roach_list.h"


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

void addRoachClient(RoachClient **headRoachList, int *score, int n_roaches, int id_roach){
    RoachClient *newRoachClient = initRoachClient(n_roaches, score, id_roach);
    if(*headRoachList == NULL){
        *headRoachList = newRoachClient;
    }
    else{
        RoachClient *currentRoachClient = *headRoachList;
        while(currentRoachClient->next != NULL){
            currentRoachClient = currentRoachClient->next;
        }
        currentRoachClient->next = newRoachClient;
    }
}

RoachClient* findRoachClient(RoachClient **headRoachList, int id_roach){
    RoachClient *currentRoachClient = *headRoachList;
    while(currentRoachClient != NULL){
        if(currentRoachClient->id == id_roach){
            break;
        }
        currentRoachClient = currentRoachClient->next;
    }
    return currentRoachClient;
}

int countRoaches(RoachClient *headRoachList){
    RoachClient *currentRoachClient = headRoachList;
    int count = 0;
    while(currentRoachClient != NULL){
        count += currentRoachClient->num_roaches;
        currentRoachClient = currentRoachClient->next;
    }
    return count;
}

void printRoachList(RoachClient *headRoachList){
    RoachClient *currentRoachClient = headRoachList;
    while(currentRoachClient != NULL){
        printf("RoachClient id: %d\n", currentRoachClient->id);
        currentRoachClient = currentRoachClient->next;
    }
}


void freeRoachList(RoachClient **headRoachList){
    RoachClient *currentRoachClient = *headRoachList;
    RoachClient *nextRoachClient = NULL;
    while(currentRoachClient != NULL){
        nextRoachClient = currentRoachClient->next;
        free(currentRoachClient);
        currentRoachClient = nextRoachClient;
    }
    *headRoachList = NULL;
}