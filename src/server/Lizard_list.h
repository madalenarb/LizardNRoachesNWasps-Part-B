/**
 * @file lizard_list.h
 * @brief Prototypes for managing a linked list of LizardClient structures.
 *
 * This header file declares functions and structures for creating, adding, printing, disconnecting,
 * freeing, and finding LizardClient instances in the context of the LizardsNRoaches game.
 */

#ifndef LIZARD_LIST_H
#define LIZARD_LIST_H

#include "../../common/protocol.h"
#include "../../common/constants.h"

/**
 * @brief Structure representing a lizard client in the game.
 */
typedef struct LizardClient {
    char id; ///< ID of the lizard client.
    position_t position; ///< Position of the lizard client.
    direction_t direction; ///< Current direction of the lizard client.
    int cauda_x[5]; ///< X coordinates of the lizard's tail segments.
    int cauda_y[5]; ///< Y coordinates of the lizard's tail segments.
    int score; ///< Score of the lizard client.
    long int password; ///< Connection status (1 if connected, 0 if disconnected).
    struct LizardClient *next; ///< Pointer to the next lizard client in the list.
} LizardClient;

/**
 * @brief Initializes a new LizardClient.
 *
 * @param id ID of the lizard client.
 * @param password Password of the lizard client.
 * @return Pointer to the newly created LizardClient.
 */
void addLizardClient(char id, int password);

/**
 * @brief Prints the list of LizardClients.
 */
void printList();

/**
 * @brief Disconnects a lizard client from the game.
 *
 * @param id ID of the lizard client to disconnect.
 */
void disconnectLizardClient(char id);

/**
 * @brief Frees the list of LizardClients.
 */
void freeList();

/**
 * @brief Finds a lizard client in the list.
 *
 * @param id ID of the lizard client to find.
 * @return Pointer to the lizard client if found, NULL otherwise.
 */
LizardClient* findLizardClient(char id);

/**
 * @brief Counts the number of lizards in the list.
 *
 * @return Number of lizards in the list.
 */
int countLizards();

#endif /* LIZARD_LIST_H */
