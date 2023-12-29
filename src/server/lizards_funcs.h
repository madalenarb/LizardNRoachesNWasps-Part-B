/**
 * @file lizards_funcs.h
 * @brief Prototypes for lizard movement and interaction functionalities.
 *
 * Declares functions for managing lizard movement, tail orientation, connection, disconnection,
 * and interaction with other game elements. These functions are essential for controlling lizard 
 * behavior in the LizardsNRoaches game.
 */

#ifndef LIZARD_FUNCS_H
#define LIZARD_FUNCS_H

#include "../../common/protocol.h"
#include "../../common/constants.h"
#include "Lizard_list.h"
#include "roach_list.h"
#include "render_funcs.h"

/**
 * @brief Determines the orientation of a lizard's tail based on its current direction.
 * 
 * This function updates the tail position of a lizard client based on the current direction of movement. 
 * The tail is aligned with the direction to simulate a realistic lizard movement.
 * 
 * @param currentClient A pointer to the LizardClient structure whose tail orientation needs to be updated.
 */
void lizardTailOrientation(LizardClient *currentClient);

/**
 * @brief Processes keyboard input to determine the direction of lizard movement.
 *
 * This function updates the position of a lizard client based on the input direction. 
 * It adjusts the lizard's position in the game environment, considering game boundaries 
 * and other game-specific rules.
 *
 * @param otherLizard A pointer to the LizardClient structure whose position is to be updated.
 */
void new_position(LizardClient* otherLizard);

/**
 * @brief Handles the connection of a new lizard to the game.
 *
 * This function processes a new lizard client's connection request, adding it to the game 
 * if conditions allow. It ensures proper game state and client management upon a new lizard joining.
 *
 * @param my_win Pointer to the game window for rendering.
 * @param headLizardList Pointer to the head of the linked list of lizard clients.
 * @param m Pointer to the received message structure.
 * @param socket Pointer to the communication socket.
 * @param nClients Pointer to the total number of lizard clients connected.
 */
void handleLizardConnect(WINDOW *my_win, LizardClient **headLizardList, message_t *m, void *socket, int *nClients);

/**
 * @brief Manages the movement of a lizard in the game.
 *
 * This function updates the position of a lizard based on movement requests, handling interactions 
 * with roaches and other lizards. It ensures the lizard's movement adheres to game rules.
 *
 * @param my_win Pointer to the game window for rendering.
 * @param headLizardList Pointer to the head of the linked list of lizard clients.
 * @param headRoachList Pointer to the head of the linked list of roach clients.
 * @param m Pointer to the message containing movement information.
 * @param socket Pointer to the communication socket.
 */
void handleLizardMovement(WINDOW *my_win, LizardClient **headLizardList, RoachClient **headRoachList, message_t *m, void *socket);

/**
 * @brief Manages the disconnection of a lizard from the game.
 *
 * Handles the disconnection of a lizard client, removing it from the game environment and 
 * updating the game state accordingly.
 *
 * @param my_win Pointer to the game window for rendering.
 * @param headLizardList Pointer to the head of the linked list of lizard clients.
 * @param m Pointer to the message structure indicating disconnection.
 * @param socket Pointer to the communication socket.
 * @param nClients Pointer to the total number of lizard clients connected.
 */
void handleLizardDisconnect(WINDOW *my_win, LizardClient **headLizardList, message_t *m, void *socket, int *nClients);

/**
 * @brief Disconnects all lizard clients at the end of the game or during shutdown.
 *
 * Ensures a clean and orderly disconnection of all lizard clients from the game, 
 * typically used when shutting down the server or ending the game.
 *
 * @param headLizardList Pointer to the head of the linked list of lizard clients.
 * @param socket Pointer to the communication socket.
 */
void disconnectAllLizards(LizardClient **headLizardList, void *socket);

#endif // LIZARD_FUNCS_H
