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

pthread_mutex_t display_lock; // Mutex for display management


/**
 * @brief Processes keyboard input to determine the direction of lizard movement.
 * 
 * This function maps arrow key inputs to corresponding directions for the lizard in the game.
 * 
 * @param ch The character representing the user's input.
 * @param m Pointer to the message_t structure where the direction will be set.
 */
void select_direction(int ch, message_t *m);


int lizardsLoses(message_t *m);
void select_direction(int ch, message_t *m);
int lizardsLoses(message_t *m);
position_t* DeriveTailPosition(position_t head_position, direction_t direction);
void renderLizard(WINDOW *display_win, display_update_t game_update);
void renderRoach(WINDOW *display_win, display_update_t game_update);
void renderWasp(WINDOW *display_win, display_update_t game_update);
void updateDisplay(WINDOW *display_win, void *subscriber);

#endif  // LIZARD_FUNCS_H
