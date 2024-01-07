#ifndef UTILS_H
#define UTILS_H

#include "../../common/protocol.h"
#include "../../common/constants.h"

#include "Lizard_list.h"
#include "roach_list.h"
#include "wasp_list.h"

#include "render_funcs.h"

// Mutexes
pthread_mutex_t lock; // Mutex to control access to messages
pthread_mutex_t lizard_lock; // Mutex to control access to lizard related variables
pthread_mutex_t sharedGameState; // Mutex to control access to shared game state
pthread_mutex_t roach_wasps_lock; // Mutex to control access to roach and wasp related variables

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
void initGameState();
void printGameState();
#endif // UTILS_H