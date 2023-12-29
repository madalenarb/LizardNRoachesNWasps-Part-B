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
            m->direction = LEFT;
            break;

        // If the Right arrow key is pressed
        case KEY_RIGHT:
            m->direction = RIGHT;
            break;

        // If the Down arrow key is pressed
        case KEY_DOWN:
            m->direction = DOWN;
            break;

        // If the Up arrow key is pressed
        case KEY_UP:
            m->direction = UP;
            break;

        default:
            break;
    }
}

