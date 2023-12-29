/**
 * @file roach_funcs.h
 * @brief Prototypes for managing roach movements and interactions in the LizardsNRoaches game.
 *
 * Declares functions for handling roach client movement, connection, disconnection, 
 * and interaction within the game. These functions manage the behavior of roaches in the game world.
 */

#ifndef ROACH_FUNCS_H
#define ROACH_FUNCS_H

#include "../../common/protocol.h"
#include "../../common/constants.h"

#include "roach_list.h"
#include "utils.h"
#include "lizards_funcs.h"

/**
 * @brief Forces the disconnection of a roach client.
 *
 * Sends a disconnect message to a roach client, effectively removing it from the game.
 *
 * @param m Pointer to the message structure to be sent.
 * @param socket Pointer to the ZeroMQ socket used for communication.
 */
void forceRoachDisconnect(message_t *m, void *socket);

/**
 * @brief Updates the position of a roach based on its direction.
 *
 * Moves a roach in the specified direction, ensuring it remains within the game window boundaries.
 *
 * @param roachClient Pointer to the RoachClient structure.
 * @param id Identifier of the roach within the RoachClient.
 */
void new_position_roaches(RoachClient *roachClient, int id);

/**
 * @brief Cleans the previous position of a roach on the game window.
 *
 * Erases the roach from its previous position on the game window, preventing ghost images.
 *
 * @param my_win Pointer to the game window.
 * @param roachClient Pointer to the RoachClient structure.
 * @param id Identifier of the roach within the RoachClient.
 */
void cleanRoach(WINDOW *my_win, RoachClient *roachClient, int id);

/**
 * @brief Handles the connection of a new roach client to the server.
 *
 * Processes a roach connection request, initializing and adding the roach client to the game.
 *
 * @param my_win Pointer to the game window.
 * @param headRoachList Pointer to the head of the linked list of roach clients.
 * @param m Pointer to the message structure received from the roach client.
 * @param socket Pointer to the ZeroMQ socket used for communication.
 * @param NroachesTotal Pointer to the total number of roaches in the game.
 * @param id_roach Identifier of the new roach client.
 */
void handleRoachesConnect(WINDOW *my_win, RoachClient **headRoachList, message_t *m, void *socket, int *NroachesTotal, int id_roach);

/**
 * @brief Manages the movement of a roach client in the game.
 *
 * Processes a movement message from a roach client, updating its position and rendering it on the game window.
 *
 * @param my_win Pointer to the game window.
 * @param headRoachList Pointer to the head of the linked list of roach clients.
 * @param m Pointer to the message structure containing movement information.
 * @param direction Direction in which the roach is moving.
 * @param socket Pointer to the ZeroMQ socket used for communication.
 * @param headLizardList Pointer to the head of the linked list of lizard clients.
 */
void handleRoachMovement(WINDOW *my_win, RoachClient **headRoachList, message_t *m, direction_t direction, void *socket, LizardClient *headLizardList);

#endif // ROACH_FUNCS_H