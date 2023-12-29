/**
 * @file roach_list.h
 * @brief Prototypes for managing roach clients in the LizardsNRoaches game.
 *
 * Declares structures and functions for handling roach clients. These include initialization, addition, 
 * finding, and freeing of roach clients, which are critical for managing the roach population in the game.
 */


#ifndef ROACHE_LIST_H
#define ROACHE_LIST_H

#include "../../common/protocol.h"
#include "../../common/constants.h"

/**
 * @brief Structure representing a roach in the game.
*/
typedef struct Roach{
    int score;
    direction_t direction;
    position_t position;
    int on_board; // 1 if the roach is on the board, 0 otherwise
    time_t eaten_time;
} Roach;

/**
 * @brief Structure representing a roach client in the game.
*/
typedef struct RoachClient {
    int id; // ID of the roach client
    int num_roaches;
    Roach roaches[MAX_ROACHES_PER_CLIENT];
    
    struct RoachClient *next; // Pointer to next roach in the list
} RoachClient;

/**
 * @brief Adds a new roach client to the linked list.
 *
 * Initializes a new roach client and adds it to the existing linked list of roach clients.
 *
 * @param headRoachList Pointer to the head of the linked list of roach clients.
 * @param score Array of scores for each roach.
 * @param n_roaches Number of roaches in the client.
 * @param id_roach Identifier for the new roach client.
 */
void addRoachClient(RoachClient **headRoachList, int *score, int n_roaches, int id_roach);

/**
 * @brief Finds a roach client in the linked list.
 *
 * Searches for a roach client in the linked list based on its identifier.
 *
 * @param headRoachList Pointer to the head of the linked list of roach clients.
 * @param id_roach Identifier of the roach client to find.
 * @return Pointer to the found RoachClient, or NULL if not found.
 */
RoachClient* findRoachClient(RoachClient **headRoachList, int id_roach);

int countRoaches(RoachClient *headRoachList);

/**
 * @brief Frees the memory allocated for the linked list of roach clients.
 *
 * Iterates through the linked list of roach clients and frees each node, effectively clearing the list.
 *
 * @param headRoachList Pointer to the head of the linked list of roach clients.
 */
void freeRoachList(RoachClient **headRoachList);

/**
 * @brief Prints the details of all roach clients in the linked list.
 *
 * Iterates through the linked list of roach clients, printing details of each client for debugging or information purposes.
 *
 * @param headRoachList Pointer to the head of the linked list of roach clients.
 */
void printRoachList(RoachClient *headRoachList);

#endif /* ROACHE_LIST_H */
