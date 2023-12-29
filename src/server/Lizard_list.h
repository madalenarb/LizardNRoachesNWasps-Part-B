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
    int password; ///< Connection status (1 if connected, 0 if disconnected).
    struct LizardClient *next; ///< Pointer to the next lizard client in the list.
} LizardClient;

void addLizardClient(LizardClient** headLizardList, char id, int password);
void printList(LizardClient* headLizardList);
void disconnectLizardClient(LizardClient** headLizardList, char id);
void freeList(LizardClient** headLizardList);
LizardClient* findLizardClient(LizardClient* headLizardList, char id);
int countLizards(LizardClient* headLizardList);

#endif /* LIZARD_LIST_H */
