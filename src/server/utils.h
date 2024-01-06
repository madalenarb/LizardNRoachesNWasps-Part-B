#ifndef UTILS_H
#define UTILS_H

#include "../../common/protocol.h"
#include "../../common/constants.h"

#include "Lizard_list.h"
#include "roach_list.h"
#include "wasp_list.h"

#include "render_funcs.h"

pthread_mutex_t lock;
pthread_mutex_t lizard_lock;
pthread_mutex_t sharedGameState;
//pthread_mutex_t roach_lock;
//pthread_mutex_t wasp_lock;

typedef struct GameState{
    WINDOW *my_win;
    LizardClient* headLizardList;
    RoachClient* headRoachList;
    WaspClient* headWaspList;
    int numLizardsClients;
    int numRoaches;
    int numRoachesClients;
    int numWasps;
    int numWaspsClients;
} GameState;

GameState gameState;

// Structure to pass multiple arguments to proxy thread function
typedef struct {
    void *frontend;
    void *backend;
} proxy_args;


position_t auxNextPosition(position_t position, direction_t direction);
int isPositionValid(position_t position);
int comparePosition(position_t position1, position_t position2);
void updateGameState(GameState* gameState, LizardClient* headLizardList, RoachClient* headRoachList, WaspClient* headWaspList);
void initGameState(GameState* gameState);
void printGameState();
#endif // UTILS_H