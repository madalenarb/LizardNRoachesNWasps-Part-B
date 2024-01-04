#include "threadsHandler.h"

void *lizardThread(void *arg){
    SharedResources *resources = (SharedResources *) arg;

    void *context = zmq_ctx_new();
    assert(context != NULL);

    void *responder = zmq_socket(context, ZMQ_REP);
    assert(responder != NULL);

    int rc = zmq_connect(responder, "inproc://backend");
    assert(rc == 0);

    message_t message;
    while(!flag_exit){
        zmq_recv(responder, &message, sizeof(message_t), 0);
        pthread_mutex_lock(resources->mutex); // Lock the mutex

        // Handle different message types
        switch(message.msg_type){
            case MSG_TYPE_LIZARD_CONNECT:
                handleLizardConnect(resources->my_win, &resources->headLizardList, &message, resources->socket, &resources->nClients);
                break;

            case MSG_TYPE_LIZARD_MOVEMENT:
                if(resources->headLizardList != NULL)
                    handleLizardMovement(resources->my_win, &resources->headLizardList, &resources->headRoachList, &message, resources->socket, &resources->headWaspList);
                break;

            case MSG_TYPE_LIZARD_DISCONNECT:
                handleLizardDisconnect(resources->my_win, &resources->headLizardList, &message, resources->socket, &resources->nClients);
                break;
            
            default:
                break;
        }

        // Unlock the mutex
        pthread_mutex_unlock(resources->mutex);
        // Send a response back to the client
        zmq_send(responder, &message, sizeof(message_t), 0);
    }

    zmq_close(responder);
    zmq_ctx_destroy(context);
    return NULL;
}

void *roachWaspThread(void *arg){
    SharedResources *resources = (SharedResources *) arg;

    void *context = zmq_ctx_new();
    assert(context != NULL);

    void *responder = zmq_socket(context, ZMQ_REP);
    assert(responder != NULL);

    int rc = zmq_connect(responder, "inproc://backend");
    assert(rc == 0);

    message_t message;
    while(!flag_exit){
        zmq_recv(responder, &message, sizeof(message_t), 0);
        pthread_mutex_lock(resources->mutex);
        // Handle different message types
        switch(message.msg_type){
            case MSG_TYPE_ROACHES_CONNECT:
                resources->id_roach++;
                handleRoachesConnect(resources->my_win, &resources->headRoachList, &message, resources->socket, &resources->NroachesTotal, resources->id_roach);
                break;

            case MSG_TYPE_ROACHES_MOVEMENT:
                if(resources->headRoachList != NULL)
                    handleRoachMovement(resources->my_win, &resources->headRoachList, &message, resources->direction, resources->socket, resources->headLizardList, resources->headWaspList);
                break;

            case MSG_TYPE_ROACHES_DISCONNECT:
                handleRoachDisconnect(resources->my_win, &resources->headRoachList, &message, resources->socket, &resources->NroachesTotal);  
                break;

            case MSG_TYPE_WASPS_CONNECT:
                resources->id_wasp++;
                handleWaspsConnect(resources->my_win, &resources->headWaspList, &message, resources->socket, &resources->NwaspsTotal, resources->id_wasp);
                break;

            case MSG_TYPE_WASPS_MOVEMENT:
                if(resources->headWaspList != NULL)
                    handleWaspMovement(resources->my_win, &resources->headWaspList, &message, resources->direction, resources->socket, &resources->headRoachList, resources->headLizardList);
                break;

            case MSG_TYPE_WASPS_DISCONNECT:
                handleWaspDisconnect(resources->my_win, &resources->headWaspList, &message, resources->socket, &resources->NwaspsTotal);
                break;

            default:
                break;
        }

        // Unlock the mutex
        pthread_mutex_unlock(resources->mutex);

        // Send a response back to the client
        zmq_send(responder, &message, sizeof(message_t), 0);
    }

    zmq_close(responder);
    zmq_ctx_destroy(context);
    return NULL;
}