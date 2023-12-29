 /**
 * @file events.c
 * @brief Implementation of functions related to handling events in the server.
 *
 * This file contains the definitions for functions responsible for handling events in the server, such as interactions between lizards and roaches.
 */

#include "events.h"

void lizardHitsLizard(WINDOW *my_win, LizardClient **headLizardList, LizardClient *currentLizard){
    LizardClient *otherLizard = *headLizardList;
    position_t nextPosition1 = auxNextPosition(currentLizard->position, currentLizard->direction);
    int new_score = 0;
    int flag = 0;
    while(otherLizard != NULL){
        if(otherLizard->id != currentLizard->id){
            if(comparePosition(nextPosition1, otherLizard->position)){
                new_score = (otherLizard->score + currentLizard->score)/2;
                cleanLizard(my_win, currentLizard);
                lizardTailOrientation(currentLizard);
                renderLizardTail(my_win, currentLizard);
                otherLizard->score = new_score;
                currentLizard->score = new_score;

    
                flag = 1;
            }
        }
        otherLizard = otherLizard->next;
    }
    if(flag == 0){
        updateAndRenderOneLizard(my_win, currentLizard);
    }
}


void lizardEatsRoach(WINDOW *my_win, RoachClient **headRoachList, LizardClient *currentLizard){
    RoachClient *roachClient = *headRoachList;
    while(roachClient != NULL){
        for(int i = 0; i < roachClient->num_roaches; i++){
            if(comparePosition(currentLizard->position, roachClient->roaches[i].position)){
                currentLizard->score += roachClient->roaches[i].score;

                cleanRoach(my_win, roachClient, i);
                roachClient->roaches[i].on_board = 0;
                roachClient->roaches[i].eaten_time = elapsed_time;
            }
        }
        roachClient = roachClient->next;
    }
}