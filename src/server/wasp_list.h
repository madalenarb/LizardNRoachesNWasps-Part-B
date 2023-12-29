/**
 * @file wasp_list.h
 * @brief Prototypes for managing wasp clients in the game.
 *
 * Declares structures and functions for handling wasp clients. These include initialization, addition, 
 * finding, and freeing of wasp clients, which are critical for managing the wasp population in the game.
 */


#ifndef WASP_LIST_H
#define WASP_LIST_H

#include "../../common/protocol.h"
#include "../../common/constants.h"

/**
 * @brief Structure representing a wasp in the game.
*/
typedef struct Wasp{
    direction_t direction;
    position_t position;
    int on_board; // 1 if the roach is on the board, 0 otherwise
} Wasp;

/**
 *  @brief Structure representing a wasp client in the game.
*/
typedef struct WaspClient {
    int id; // ID of the wasp client
    int num_wasps;
    Wasp wasps[10];
    
    struct WaspClient *next; // Pointer to next wasp in the list
} WaspClient;

/**
 * @brief Adds a new wasp client to the linked list.
 *
 * Initializes a new wasp client and adds it to the existing linked list of wasp clients.
 *
 * @param headWaspList Pointer to the head of the linked list of wasp clients.
 * @param n_wasps Number of wasps in the client.
 * @param id_wasp Identifier for the new wasp client.
 */
void addWaspClient(WaspClient **headWaspList, int n_wasps, int id_wasp);

/**
 * @brief Finds a wasp client in the linked list.
 *
 * Searches for a wasp client in the linked list based on its identifier.
 *
 * @param headWaspList Pointer to the head of the linked list of wasp clients.
 * @param id_wasp Identifier of the wasp client to find.
 * @return Pointer to the found WaspClient, or NULL if not found.
 */
WaspClient* findWaspClient(WaspClient **headWaspList, int id_wasp);

/**
 * @brief Counts the total number of wasps in all clients.
 *
 * Iterates through the linked list of wasp clients and counts the total number of wasps.
 *
 * @param headWaspList Pointer to the head of the linked list of wasp clients.
 * @return Total number of wasps.
 */
int countWasps(WaspClient *headWaspList);

/**
 * @brief Frees the memory allocated for the linked list of wasp clients.
 *
 * Iterates through the linked list of wasp clients and frees each node, effectively clearing the list.
 *
 * @param headWaspList Pointer to the head of the linked list of wasp clients.
 */
void freeWaspList(WaspClient **headWaspList);

/**
 * @brief Prints the details of all wasp clients in the linked list.
 *
 * Iterates through the linked list of wasp clients, printing details of each client for debugging or information purposes.
 *
 * @param headWaspList Pointer to the head of the linked list of wasp clients.
 */
void printWaspList(WaspClient *headWaspList);

#endif /*  WASP_LIST_H */
