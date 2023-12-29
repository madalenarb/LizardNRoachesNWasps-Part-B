/**
 * @file lizard_list.c
 * @brief Implementation of functions related to managing a linked list of LizardClient structures.
 *
 * This file contains the definitions for functions responsible for creating, adding, printing, disconnecting,
 * freeing, and finding lizard clients within the context of the game.
 */
#include "Lizard_list.h"

// Create a new LizardClient
LizardClient* initLizardClient(char id, int password){
    LizardClient* newClient = malloc(sizeof(LizardClient));
    if(newClient == NULL){
        perror("malloc lizardClient");
        exit(EXIT_FAILURE);
    }
    
    newClient->id = id;
    newClient->direction = rand() % 4;
    newClient->password = password;

    // Set the initial position of the LizardClient, ensuring that it is not out of bounds
    int minDistance = 6;

    newClient->position.position_x = rand() % (WINDOW_WIDTH - 2*minDistance) + minDistance;
    newClient->position.position_y = rand() % (WINDOW_HEIGHT - 2*minDistance) + minDistance;

    // Set the initial position of the LizardClient's tail
    for(int i = 0; i < TAIL_LENGTH; i++){
        switch(newClient->direction){
            case UP:
                newClient->cauda_x[i] = newClient->position.position_x + i + 1;
                newClient->cauda_y[i] = newClient->position.position_y;
                break;
            case DOWN:
                newClient->cauda_x[i] = newClient->position.position_x - i - 1;
                newClient->cauda_y[i] = newClient->position.position_y;
                break;
            case LEFT:
                newClient->cauda_x[i] = newClient->position.position_x;
                newClient->cauda_y[i] = newClient->position.position_y + i + 1;
                break;
            case RIGHT:
                newClient->cauda_x[i] = newClient->position.position_x - i - 1;
                newClient->cauda_y[i] = newClient->position.position_y;
                break;
        }
    }
    newClient->score = 0;
    newClient->next = NULL;
    return newClient;
}


// Add a new LizardClient to the end of the list
void addLizardClient(LizardClient** headLizardList, char id, int password){
    LizardClient* lizardClient = initLizardClient(id, password);
    if(*headLizardList == NULL){
        *headLizardList = lizardClient;
    } else {
        LizardClient* current = *headLizardList;
        while(current->next != NULL){
            current = current->next;
        }
        current->next = lizardClient;
    }
}

// Print the list of LizardClient
void printList(LizardClient* headLizardList){
    LizardClient* current = headLizardList;
    while(current != NULL){
        printf("id: %c, position: %d, %d, score: %d\n", current->id, current->position.position_x, current->position.position_y, current->score);
        for(int i=0; i<5; i++)
        {
            printf("cauda_x[%d]=%d cauda_y[%d]=%d\n", i, current->cauda_x[i], i, current->cauda_y[i]);
        }
        current = current->next;
    }
}

int countLizards(LizardClient* headLizardList){
    LizardClient* current = headLizardList;
    int count = 0;
    while(current != NULL){
        count++;
        current = current->next;
    }
    return count;
}

// Disconect a LizardClient from the list
void disconnectLizardClient(LizardClient** headLizardList, char id){
    LizardClient* current = *headLizardList, *prev;
    if(current != NULL && current->id == id){
        *headLizardList = current->next;
        free(current);
        return;
    }
    while(current != NULL && current->id != id){
        prev = current;
        current = current->next;
    }
    if(current == NULL) return;
    prev->next = current->next;
    free(current);
}

// Free the list of LizardClient
void freeList(LizardClient** headLizardList){
    LizardClient* current;
    while(*headLizardList != NULL){
        current = *headLizardList;
        *headLizardList = (*headLizardList)->next;
        free(current);
    }
}

// Get the LizardClient with the id
LizardClient* findLizardClient(LizardClient* headLizardList, char id){
    LizardClient* current = headLizardList;
    while(current != NULL){
        if(current->id == id){
            return current;
        }
        current = current->next;
    }
    return NULL;
}

