#include "display.h"
#include "utils.h"

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

void populateWaspUpdate(WaspClient *waspClient, display_update_t *gameState)
{
    gameState->entity_type = WASP;
    for(int i = 0; i < waspClient->num_wasps; i++){
        gameState->wasp_positions[i] = waspClient->wasps[i].position;
        gameState->wasps_num = waspClient->num_wasps;
    }
}


void handleDisplayUpdate(void *socket_display)
{   
    LizardClient *currentLizardClient = NULL;
    RoachClient *currentRoachClient = NULL;
    WaspClient *currentWaspClient = NULL;

    int totalEntities = countLizards() + countRoaches() + countWasps();
    int i = 0;

    display_update_t *gameStates = (display_update_t *)malloc(sizeof(display_update_t) * totalEntities);
    for(int i = 0; i < totalEntities; i++){
        gameStates[i].entity_type = NOTHING;
        gameStates[i].roaches_num = 0;
        for(int j = 0; j < MAX_ROACHES_PER_CLIENT; j++){
            gameStates[i].roach_score[j] = 0;
        }
    }
    
    if(gameState.headLizardList == NULL && gameState.headRoachList == NULL && gameState.headWaspList == NULL){
        zmq_send(socket_display, &totalEntities, sizeof(int), 0);
        return;
    }

    if(gameState.headLizardList != NULL)
        currentLizardClient = gameState.headLizardList;
    
    while(currentLizardClient != NULL){
        populateLizardUpdate(currentLizardClient, &gameStates[i++]);
        currentLizardClient = currentLizardClient->next;
    }

    if(gameState.headRoachList != NULL)
        currentRoachClient = gameState.headRoachList;

    while(currentRoachClient != NULL){
        populateRoachUpdate(currentRoachClient, &gameStates[i++]);        
        currentRoachClient = currentRoachClient->next;
    }

    if(gameState.headWaspList != NULL)
        currentWaspClient = gameState.headWaspList;

    while(currentWaspClient != NULL){
        populateWaspUpdate(currentWaspClient, &gameStates[i++]);        
        currentWaspClient = currentWaspClient->next;
    }

    zmq_send(socket_display, &totalEntities, sizeof(int), 0);
    //Send full game state to display

    zmq_send(socket_display, gameStates, sizeof(display_update_t) * totalEntities, 0);


    free(gameStates);
}
