/**
 * @file Lizard-funcs.c
 * @brief Implementation of functions related to lizard client functionalities.
 *
 * This file contains the definitions for functions responsible for lizard client functionalities, such as selecting directions based on user input.
 */

#include "Lizard-funcs.h"

void select_direction(int ch, message_t *m){
    switch (ch)
    {
        // If the Left arrow key is pressed
        case KEY_LEFT:
            mvprintw(0, 0, "You pressed LEFT");
            m->direction = LEFT;
            break;

        // If the Right arrow key is pressed
        case KEY_RIGHT:
            mvprintw(0, 0, "You pressed RIGHT");
            m->direction = RIGHT;
            break;

        // If the Down arrow key is pressed
        case KEY_DOWN:
            mvprintw(0, 0, "You pressed DOWN");
            m->direction = DOWN;
            break;

        // If the Up arrow key is pressed
        case KEY_UP:
            mvprintw(0, 0, "You pressed UP");
            m->direction = UP;
            break;

        default:
            break;
    }
}


int lizardsLoses(message_t *m){

    for(int i = 0; i < 10; i++){
        clear();
        printw("You lost the game. Waiting %d seconds to reconnect...\n", 10-i);
        refresh();
        sleep(1);
    }

    clear();
    printw("Press 'r' to reconnect or any other key to exit\n", 0, 0);
    refresh();
    int key = getch();
    if(key == 'r'){
        m->msg_type = MSG_TYPE_LIZARD_RECONNECT;
        clear();
        refresh();
    }else{
        m->msg_type = MSG_TYPE_LIZARD_DISCONNECT;
        return 1;
    }
    return 0;
}

position_t* DeriveTailPosition(position_t head_position, direction_t direction){
    static position_t tail_position[TAIL_LENGTH];
    for(int i = 0; i < TAIL_LENGTH; i++){
        switch(direction){
            case UP:
                tail_position[i].position_x = head_position.position_x + i + 1;
                tail_position[i].position_y = head_position.position_y;
                break;
            case DOWN:
                tail_position[i].position_x = head_position.position_x - i - 1;
                tail_position[i].position_y = head_position.position_y;
                break;
            case LEFT:
                tail_position[i].position_x = head_position.position_x;
                tail_position[i].position_y = head_position.position_y + i + 1;
                break;
            case RIGHT:
                tail_position[i].position_x = head_position.position_x;
                tail_position[i].position_y = head_position.position_y - i - 1;
                break;
        }
    }
    return tail_position;
}

void renderLizard(WINDOW *display_win, display_update_t game_update){
    mvwaddch(display_win, game_update.position.position_x, game_update.position.position_y, game_update.ch);
    //Draw the tail
    position_t* tailPositions = DeriveTailPosition(game_update.position, game_update.direction);
    for(int i = 0; i < TAIL_LENGTH; i++){
        mvwaddch(display_win, tailPositions[i].position_x, tailPositions[i].position_y, (game_update.score < 50) ? '.' : '*');
    }
}

void renderRoach(WINDOW *display_win, display_update_t game_update){
    for(int i = 0; i < game_update.roaches_num; i++){
        if(game_update.on_board[i] == 1)
            mvwaddch(display_win, game_update.roach_positions[i].position_x, game_update.roach_positions[i].position_y, '0' + game_update.roach_score[i]);
    }
}

void renderWasp(WINDOW *display_win, display_update_t game_update){
    for(int i = 0; i < game_update.wasps_num; i++){
        mvwaddch(display_win, game_update.wasp_positions[i].position_x, game_update.wasp_positions[i].position_y, '#');
    }
}

void updateDisplay(WINDOW *display_win, void *subscriber){
    int totalEntities = 0;
    zmq_recv(subscriber, &totalEntities, sizeof(int), 0);
    display_update_t *gameState = (display_update_t *)malloc(sizeof(display_update_t) * totalEntities);
    if(totalEntities > 0){
        zmq_recv(subscriber, gameState, sizeof(display_update_t) * totalEntities, 0);
    } else {
        gameState = NULL;
    }
    
    pthread_mutex_lock(&display_lock);
    werase(display_win);

    box(display_win, 0 , 0);
    pthread_mutex_unlock(&display_lock);

    //Render the entities
    if(totalEntities > 0){
        for(int i = 0; i < totalEntities; i++){
            if(gameState[i].entity_type == LIZARD){
                pthread_mutex_lock(&display_lock);
                renderLizard(display_win, gameState[i]);
                pthread_mutex_unlock(&display_lock);
            } else if(gameState[i].entity_type == ROACH){
                pthread_mutex_lock(&display_lock);
                renderRoach(display_win, gameState[i]);
                pthread_mutex_unlock(&display_lock);
            } else if(gameState[i].entity_type == WASP){
                pthread_mutex_lock(&display_lock);
                renderWasp(display_win, gameState[i]);
                pthread_mutex_unlock(&display_lock);
            }
        }
    }

    pthread_mutex_lock(&display_lock);
    wrefresh(display_win);
    pthread_mutex_unlock(&display_lock);
}


