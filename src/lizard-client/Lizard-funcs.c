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