/**
 * @file wasp_funcs.h
 * @brief Prototypes for managing roach movements and interactions in the LizardsNRoaches game.
 *
 * Declares functions for handling roach client movement, connection, disconnection, 
 * and interaction within the game. These functions manage the behavior of roaches in the game world.
 */

#ifndef WASP_FUNCS_H
#define WASP_FUNCS_H

#include "../../common/protocol.h"
#include "../../common/constants.h"


#include "utils.h"
#include "wasp_list.h"
#include "lizards_funcs.h"
#include "events.h"
#include "../../protos/c_out/protocol.pb-c.h"


/**
 * @brief Forces the disconnection of a wasp client.
 *
 * Sends a disconnect message to a wasp client, effectively removing it from the game.
 *
 * @param m Pointer to the message structure to be sent.
 * @param socket Pointer to the ZeroMQ socket used for communication.
 */
void forceWaspDisconnect(LizardsNroachestypes__GameMessage *m, void *socket);

/**
 * @brief Updates the position of a wasp based on its direction.
 *
 * Moves a wasp in the specified direction, ensuring it remains within the game window boundaries.
 *
 * @param waspClient Pointer to the WaspClient structure.
 * @param id Identifier of the wasp within the RoachClient.
 */
void new_position_wasps(WaspClient *waspClient, int id);

/**
 * @brief Cleans the previous position of a roach on the game window.
 *
 * Erases the wasp from its previous position on the game window, preventing ghost images.
 *
 * @param my_win Pointer to the game window.
 * @param waspClient Pointer to the WaspClient structure.
 * @param id Identifier of the wasp within the RoachClient.
 */
void cleanWasp(WaspClient *waspClient, int id);

/**
 * @brief Handles the connection of a new roach client to the server.
 *
 * Processes a roach connection request, initializing and adding the roach client to the game.
 *
 * @param my_win Pointer to the game window.
 * @param headWaspList Pointer to the head of the linked list of wasp clients.
 * @param m Pointer to the message structure received from the wasp client.
 * @param socket Pointer to the ZeroMQ socket used for communication.
 * @param NroachesTotal Pointer to the total number of roaches in the game.
 * @param id_roach Identifier of the new wasp client.
 */
void handleWaspsConnect(LizardsNroachestypes__GameMessage *m, void *socket, int id_wasp);

/**
 * @brief Manages the movement of a roach client in the game.
 *
 * Processes a movement message from a roach client, updating its position and rendering it on the game window.
 *
 * @param my_win Pointer to the game window.
 * @param headWaspList Pointer to the head of the linked list of wasp clients.
 * @param m Pointer to the message structure containing movement information.
 * @param direction Direction in which the wasp is moving.
 * @param socket Pointer to the ZeroMQ socket used for communication.
 * @param headLizardList Pointer to the head of the linked list of lizard clients.
 */
void handleWaspMovement(LizardsNroachestypes__GameMessage *m, void *socket);

/**
 * @brief Handles the disconnection of a wasp client from the server.
 *
 * Processes a disconnection message from a wasp client, removing it from the game.
 *
 * @param my_win Pointer to the game window.
 * @param headWaspList Pointer to the head of the linked list of wasp clients.
 * @param m Pointer to the message structure received from the wasp client.
 * @param socket Pointer to the ZeroMQ socket used for communication.
 * @param NwaspsTotal Pointer to the total number of wasps in the game.
 */
void handleWaspDisconnect(LizardsNroachestypes__GameMessage *m, void *socket);

/**
 * @brief Disconnects all wasp clients at the end of the game or during shutdown.
 * 
 * Ensures a clean and orderly disconnection of all wasp clients from the game,
 * typically used when shutting down the server or ending the game.
 *
 * @param my_win Pointer to the game window.
 * @param headWaspList Pointer to the head of the linked list of wasp clients.
 * @param NwaspsTotal Pointer to the total number of wasps in the game.
 */
void disconnectAllWasps();

/**
 * @brief Checks if a wasp is in a given position.
 *
 * Iterates through the linked list of wasp clients and checks if any wasp is in the given position.
 *
 * @param headWaspList Pointer to the head of the linked list of wasp clients.
 * @param position Position to check.
 * @return 1 if a wasp is in the given position, 0 otherwise.
 */
int checkPositionforWasp(position_t position);

#endif // ROACH_FUNCS_H