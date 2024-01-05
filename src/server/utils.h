#ifndef UTILS_H
#define UTILS_H

#include "../../common/protocol.h"
#include "../../common/constants.h"

#include "Lizard_list.h"
#include "roach_list.h"
#include "wasp_list.h"

typedef struct GameState{
    LizardClient* headLizardList;
    RoachClient* headRoachList;
    WaspClient* headWaspList;
    int numLizards;
    int numRoaches;
    int numWasps;
} GameState;

position_t auxNextPosition(position_t position, direction_t direction);
int isPositionValid(position_t position);
int comparePosition(position_t position1, position_t position2);
void updateGameState(GameState* gameState, LizardClient* headLizardList, RoachClient* headRoachList, WaspClient* headWaspList);
void printGameState(GameState* gameState);

#endif // UTILS_H