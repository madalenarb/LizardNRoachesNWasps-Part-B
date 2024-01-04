#include "../../common/protocol.h"
#include "../../common/constants.h"
#include "lizards_funcs.h"

#include "lizards_funcs.h"
#include "roach_funcs.h"
#include "wasp_funcs.h"

typedef struct SharedResources{
    LizardClient* headLizardList;
    RoachClient* headRoachList;
    WaspClient* headWaspList;
    int id_wasp;
    int NwaspsTotal;
    int id_roach;
    int NroachesTotal;
    int nClients;
    direction_t direction;
    void *socket;
    WINDOW *my_win;
    pthread_mutex_t *mutex;
} SharedResources;

void *lizardThread(void *arg);
void *roachWaspThread(void *arg);