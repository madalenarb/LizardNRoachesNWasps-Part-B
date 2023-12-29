/**
 * @file Lizard-funcs.h
 * @brief Prototypes for functions used in the lizard client.
 *
 * This header file declares functions used for handling user input and determining lizard directions in the LizardsNRoaches game client.
 */

#ifndef LIZARD_FUNCS_H
#define LIZARD_FUNCS_H

#include <ncurses.h>
#include "../../common/protocol.h"  

/**
 * @brief Processes keyboard input to determine the direction of lizard movement.
 * 
 * This function maps arrow key inputs to corresponding directions for the lizard in the game.
 * 
 * @param ch The character representing the user's input.
 * @param m Pointer to the message_t structure where the direction will be set.
 */
void select_direction(int ch, message_t *m);

#endif  // LIZARD_FUNCS_H
