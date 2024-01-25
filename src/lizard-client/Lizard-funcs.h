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

/**
 * @brief Handles the case where the Lizard loses the game.
 * 
 * This function displays a message to the user indicating that the Lizard has lost the game, 
 * and prompts the user to reconnect or exit after a 10 second delay.
 * 
 * @param m Pointer to the message_t structure where the direction will be set.
 */
int lizardsLoses(message_t *m);

/**
 * @brief Accoring to the key pressed, the function will set the direction of the lizard.
 * 
 * @param display_win The window where the lizard will be printed.
 * @param game_update The struct that contains the information about the lizard.
*/
void select_direction(int ch, message_t *m);

/**
 * @brief Given the direction and the position of the lizard, the function will return the position of the tail so
 * that it can be printed by the lizard Client display.
 * 
 * @param head_position The position of the head of the lizard.
 * @param direction The direction of the lizard.
 * 
 * @return The position of the tail of the lizard.  
 * 
*/
position_t* DeriveTailPosition(position_t head_position, direction_t direction);

/**
 * @brief Renders the lizard on the display window.
 * 
 * This function renders the lizard on the display window, as well as its tail.
 * 
 * @param display_win The window where the lizard will be printed.
 * @param game_update The struct that contains the information about the lizard.
 */
void renderLizard(WINDOW *display_win, display_update_t game_update);

/**
 * @brief Renders the roach on the display window.
 * 
 * This function renders the roach on the display window.
 * 
 * @param display_win The window where the roach will be printed.
 * @param game_update The struct that contains the information about the roach.
 */
void renderRoach(WINDOW *display_win, display_update_t game_update);

/**
 * @brief Renders the wasp on the display window.
 * 
 * This function renders the wasp on the display window.
 * 
 * @param display_win The window where the wasp will be printed.
 * @param game_update The struct that contains the information about the wasp.
 */
void renderWasp(WINDOW *display_win, display_update_t game_update);

/**
 * @brief Renders the lizard on the display window.
 * 
 * This function renders the lizard on the display window, as well as its tail.
 * 
 * @param display_win The window where the lizard will be printed.
 * @param subscriber The struct that contains the information about the lizard.
 */
void updateDisplay(WINDOW *display_win, void *subscriber);

#endif  // LIZARD_FUNCS_H
