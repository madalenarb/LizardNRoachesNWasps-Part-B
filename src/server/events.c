 /**
 * @file events.c
 * @brief Implementation of functions related to handling events in the server.
 *
 * This file contains the definitions for functions responsible for handling events in the server, such as interactions between lizards and roaches.
 */

#include "events.h"

int lizardHitsLizard(LizardClient *currentLizard){
    LizardClient *otherLizard = gameState.headLizardList;
    position_t nextPosition1 = auxNextPosition(currentLizard->position, currentLizard->direction);
    int new_score = 0;
    int flag = 0;
    while(otherLizard != NULL){
        if(otherLizard->id != currentLizard->id){
            if(comparePosition(nextPosition1, otherLizard->position)){
                new_score = (otherLizard->score + currentLizard->score)/2;
                cleanLizard(gameState.my_win, currentLizard);
                lizardTailOrientation(currentLizard);
                renderLizardTail(gameState.my_win, currentLizard);
                otherLizard->score = new_score;
                currentLizard->score = new_score;

    
                flag = 1;
            }
        }
        otherLizard = otherLizard->next;
    }
    return flag;
}


void lizardEatsRoach(LizardClient *currentLizard){
    RoachClient *roachClient = gameState.headRoachList;
    while(roachClient != NULL){
        for(int i = 0; i < roachClient->num_roaches; i++){
            if(comparePosition(currentLizard->position, roachClient->roaches[i].position)){
                currentLizard->score += roachClient->roaches[i].score;

                cleanRoach(gameState.my_win, roachClient, i);
                roachClient->roaches[i].on_board = 0;
                roachClient->roaches[i].eaten_time = elapsed_time;
            }
        }
        roachClient = roachClient->next;
    }
}

int WaspStingsLizard(LizardClient *currentLizard, WaspClient *currentWasp){
    int stingOccurred = 0;

    // Wasp moved, check if it stings a lizard
    if(currentWasp != NULL){
        for(int i = 0; i < currentWasp->num_wasps; i++){
            position_t nextPositionWasp = auxNextPosition(currentWasp->wasps[i].position, currentWasp->wasps[i].direction);
            LizardClient *lizardClient = gameState.headLizardList;
            while(lizardClient != NULL){
                if(comparePosition(nextPositionWasp, lizardClient->position)){
                    lizardClient->score -= 10;
                    stingOccurred = 1;
                    printf("Lizard %c stung by wasp %d\n", lizardClient->id, i);
                }
                lizardClient = lizardClient->next;
            }
        }
    } else if(currentLizard != NULL){
        // Lizard moved, check if it stings a lizard
        position_t nextPositionLizard = auxNextPosition(currentLizard->position, currentLizard->direction);
        WaspClient *waspClient = gameState.headWaspList;
        while(waspClient != NULL){
            for(int i = 0; i < waspClient->num_wasps; i++){
                if(comparePosition(nextPositionLizard, waspClient->wasps[i].position)){
                    cleanLizard(gameState.my_win, currentLizard);
                    lizardTailOrientation(currentLizard);
                    renderLizardhead(gameState.my_win, currentLizard);
                    currentLizard->score -= 10;
                    stingOccurred = 1;
                }
            }
            waspClient = waspClient->next;
        }
    }
    return stingOccurred;
}