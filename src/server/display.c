#include "display.h"

void populateLizardUpdate(LizardClient *lizardClient, display_update_t *gameState)
{
    gameState->entity_type = LIZARD;
    gameState->position = lizardClient->position;
    gameState->direction = lizardClient->direction;
    gameState->ch = lizardClient->id;
    gameState->score = lizardClient->score;
}

void populateRoachUpdate(RoachClient *roachClient, display_update_t *gameState)
{
    gameState->entity_type = ROACH;
    for(int i = 0; i < roachClient->num_roaches; i++){
        gameState->roach_positions[i] = roachClient->roaches[i].position;
        gameState->roach_score[i] = roachClient->roaches[i].score;
        gameState->roaches_num = roachClient->num_roaches;
        gameState->on_board[i] = roachClient->roaches[i].on_board;
    }
}

/**
 * 
 * @brief Handles the display update
 * 
 * This function handles the display update by sending the display update message to the display application.
 * 
 * @param socket_display The socket to send the display update message to
 * @param headLizardList The head of the lizard client linked list
 * @param headRoachList The head of the roach client linked list
 * 
*/
void handleDisplayUpdate(void *socket_display, LizardClient *headLizardList, RoachClient *headRoachList)
{
    LizardClient *currentLizardClient = NULL;
    RoachClient *currentRoachClient = NULL;

    int totalEntities = countLizards(headLizardList) + countRoaches(headRoachList);
    int i = 0;

    display_update_t *gameState = (display_update_t *)malloc(sizeof(display_update_t) * totalEntities);
    for(int i = 0; i < totalEntities; i++){
        gameState[i].entity_type = NOTHING;
        gameState[i].roaches_num = 0;
        for(int j = 0; j < MAX_ROACHES_PER_CLIENT; j++){
            gameState[i].roach_score[j] = 0;
        }
    }
    
    if(headLizardList == NULL && headRoachList == NULL){
        return;
    }

    if(headLizardList != NULL)
        currentLizardClient = headLizardList;
    
    while(currentLizardClient != NULL){
        populateLizardUpdate(currentLizardClient, &gameState[i++]);
        currentLizardClient = currentLizardClient->next;
    }

    if(headRoachList != NULL)
        currentRoachClient = headRoachList;

    while(currentRoachClient != NULL){
        populateRoachUpdate(currentRoachClient, &gameState[i++]);        
        currentRoachClient = currentRoachClient->next;
    }

    //Send total number of entities to display
    zmq_send(socket_display, &totalEntities, sizeof(int), 0);

    //Send full game state to display
    zmq_send(socket_display, gameState, sizeof(display_update_t) * totalEntities, 0);

    free(gameState);
}
