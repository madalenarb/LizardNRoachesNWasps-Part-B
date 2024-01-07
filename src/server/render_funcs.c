/**
 * @file render_funcs.c
 * @brief Implementation of functions responsible for rendering lizards and managing game windows.
 *
 * This file contains the definitions of functions that handle the rendering of lizard heads and tails,
 * as well as managing the game window and updating lizard positions in the context of the game.
 */

#include "render_funcs.h"   

void updateRoachesVisibility(){
    RoachClient *currentRoachClient = gameState.headRoachList;

    while(currentRoachClient != NULL){
        for(int i = 0; i < currentRoachClient->num_roaches; i++){
            if(currentRoachClient->roaches[i].on_board == 0){
                if(difftime(elapsed_time, currentRoachClient->roaches[i].eaten_time) >= 5){
                    currentRoachClient->roaches[i].on_board = 1;
                    currentRoachClient->roaches[i].position.position_x = rand() % (WINDOW_HEIGHT-2) + 1;
                    currentRoachClient->roaches[i].position.position_y = rand() % (WINDOW_WIDTH-2) + 1;
                }
            }
        }
        currentRoachClient = currentRoachClient->next;
    }
    
}

void setupWindows(){
    initscr();			/* Start curses mode 		*/
    cbreak();
    keypad(stdscr, TRUE);
    noecho();
    gameState.my_win = newwin(WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0);
    box(gameState.my_win, 0 , 0);		/* 0, 0 gives default characters 
                        * for the vertical and horizontal
                        * lines			*/
    curs_set(0); // Make the cursor invisible
    wrefresh(gameState.my_win);
}

void resetWindow(){
    gameState.my_win = newwin(WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0);
    wclear(gameState.my_win);
    box(gameState.my_win, 0 , 0);
}

void renderLizardhead(LizardClient *otherLizard){
    wmove(gameState.my_win, otherLizard->position.position_x, otherLizard->position.position_y);
    //if score is lower than 0 it isnt printed on the screen
    waddch(gameState.my_win, otherLizard->id | A_BOLD);	
    wrefresh(gameState.my_win);
}

// void renderLizardTail(WINDOW *my_win, LizardClient *otherLizard){
//     for(int i = 0; i < TAIL_LENGTH; i++){
//         wmove(my_win, otherLizard->cauda_x[i], otherLizard->cauda_y[i]);
//         waddch(my_win, (otherLizard->score < 50) ? '.' | A_BOLD : '*' | A_BOLD);
//     }
//     wrefresh(my_win);
// }

void renderLizardTail(LizardClient *otherLizard){
    for(int i = 0; i < TAIL_LENGTH; i++){
        wmove(gameState.my_win, otherLizard->cauda_x[i], otherLizard->cauda_y[i]);
        char charToPrint;
        if (otherLizard->score < 0) {
            charToPrint = ' ';
        } else if (otherLizard->score < 50) {
            charToPrint = '.'; 
        } else {
            charToPrint = '*'; 
        }
        waddch(gameState.my_win, charToPrint | A_BOLD);
    }
    wrefresh(gameState.my_win);
}

void cleanLizard(WINDOW *my_win, LizardClient *otherLizard){
    wmove(my_win, otherLizard->position.position_x, otherLizard->position.position_y);
    waddch(my_win, ' ');
    for(int i = 0; i < 5; i++){
        wmove(my_win, otherLizard->cauda_x[i], otherLizard->cauda_y[i]);
        waddch(my_win, ' ');
    }
    wrefresh(my_win);
}

void updateAndRenderLizardsHeads(){
    LizardClient *currentLizard = gameState.headLizardList;
    while(currentLizard != NULL){
        //Update lizard position
        renderLizardhead(currentLizard);
        currentLizard = currentLizard->next;
    }
}

void updateAndRenderLizardsTails(){
    LizardClient *currentLizard = gameState.headLizardList;
    while(currentLizard != NULL){
        //Update lizard position
        renderLizardTail(currentLizard);
        currentLizard = currentLizard->next;
    }
}

void updateAndRenderOneLizard(LizardClient *otherLizard){
    //Update lizard position
    cleanLizard(gameState.my_win, otherLizard);

    new_position(otherLizard);

    //Render updated lizard
    renderLizardhead(otherLizard);
    renderLizardTail(otherLizard);

}

void renderInitRoach(RoachClient *roachClient, int id){
    wmove(gameState.my_win, roachClient->roaches[id].position.position_x, roachClient->roaches[id].position.position_y);
    waddch(gameState.my_win, '0' + roachClient->roaches[id].score);
}

void renderRoaches(RoachClient *roachClient){
    for(int i = 0; i < roachClient->num_roaches; i++){
        if(roachClient->roaches[i].on_board == 1){
            renderInitRoach(roachClient, i);
        }
    }
    wrefresh(gameState.my_win);
}

void renderRoach(int id, RoachClient *roachClient){
    wmove(gameState.my_win, roachClient->roaches[id].position.position_x, roachClient->roaches[id].position.position_y);
    waddch(gameState.my_win, ' ');
    wrefresh(gameState.my_win);
    position_t nextPosition = auxNextPosition(roachClient->roaches[id].position, roachClient->roaches[id].direction);
    
    int occupied_position = 0;
    int occupied_position_lizard = checkPositionforLizard(nextPosition);
    int occupied_position_wasps = checkPositionforWasp(nextPosition);
    if(occupied_position_wasps == 1 || occupied_position_lizard == 1){
        occupied_position = 1;
    }
    // Check if the next position is occupied by a lizard
    while(occupied_position == 1){
        roachClient->roaches[id].direction = (direction_t) (rand() % 4);
        nextPosition = auxNextPosition(roachClient->roaches[id].position, roachClient->roaches[id].direction);
        occupied_position_lizard = checkPositionforLizard(nextPosition);
        occupied_position_wasps = checkPositionforWasp(nextPosition);

        if(occupied_position_wasps == 1 || occupied_position_lizard == 1){
            occupied_position = 1;
        } else if(occupied_position_wasps == 0 && occupied_position_lizard == 0){
            occupied_position = 0;
        }
    }

    // If the next position is not occupied, move the roach to the next position
    if(occupied_position_lizard == 0){
        new_position_roaches(roachClient, id);
    }

    wmove(gameState.my_win, roachClient->roaches[id].position.position_x, roachClient->roaches[id].position.position_y);
    waddch(gameState.my_win,'0' + roachClient->roaches[id].score);
    wrefresh(gameState.my_win);
}


void updateAndRenderRoaches(){
    RoachClient *currentRoachClient = gameState.headRoachList;
    while(currentRoachClient != NULL){
        for(int i = 0; i < currentRoachClient->num_roaches; i++){
            if(currentRoachClient->roaches[i].on_board == 1){
                wmove(gameState.my_win, currentRoachClient->roaches[i].position.position_x, currentRoachClient->roaches[i].position.position_y);
                waddch(gameState.my_win,'0' + currentRoachClient->roaches[i].score);
            }
        }
        currentRoachClient = currentRoachClient->next;
    }
    wrefresh(gameState.my_win);
}

void renderWasp(WaspClient *waspClient, int waspIndex){
    cleanWasp(waspClient, waspIndex);
    // Calculate the next position of the wasp
    position_t nextPosition = auxNextPosition(waspClient->wasps[waspIndex].position, waspClient->wasps[waspIndex].direction);

    // Check if the next position is occupied by a lizard
    int occupied_position = checkPositionforRoach(nextPosition);
    int occupied_position_lizard = checkPositionforLizard(nextPosition);

    if(occupied_position == 0 && occupied_position_lizard == 0){
        new_position_wasps(waspClient, waspIndex);  


        wmove(gameState.my_win, waspClient->wasps[waspIndex].position.position_x, waspClient->wasps[waspIndex].position.position_y);
        waddch(gameState.my_win, '#');  // Representa vespas com '#'
        wrefresh(gameState.my_win);
    }
}

void updateAndRenderWasps(){
    WaspClient *currentWaspClient = gameState.headWaspList;
    while(currentWaspClient != NULL){
        for(int i = 0; i < currentWaspClient->num_wasps; i++){
            if(currentWaspClient->wasps[i].on_board == 1){
                wmove(gameState.my_win, currentWaspClient->wasps[i].position.position_x, currentWaspClient->wasps[i].position.position_y);
                waddch(gameState.my_win, '#'); 
                wrefresh(gameState.my_win);
            }
        }
        currentWaspClient = currentWaspClient->next;
    }
    wrefresh(gameState.my_win);
}


void updateAndRenderEverything(){
    pthread_mutex_lock(&sharedGameState);
    resetWindow();
    updateAndRenderLizardsTails();
    updateRoachesVisibility();
    updateAndRenderRoaches();
    updateAndRenderLizardsHeads();
    updateAndRenderWasps();

    pthread_mutex_unlock(&sharedGameState);
}