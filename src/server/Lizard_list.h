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

void addLizardClient(char id, int password);
void printList();
void disconnectLizardClient(char id);
void freeList();
LizardClient* findLizardClient(char id);
int countLizards();

#endif /* LIZARD_LIST_H */
