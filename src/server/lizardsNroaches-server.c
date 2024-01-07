/**
 * @file lizardsNroaches-server.c
 * @brief Main server program managing the game state, handling lizard and roach connections, movements, and display updates.
 *
 * This file contains the main program for the server, responsible for managing the game state, handling lizard and roach
 * connections, movements, and updating the display. It utilizes functions from various files to achieve these tasks.
 */

#include "../../common/protocol.h"
#include "../../common/constants.h"

#include "render_funcs.h"
#include "display.h"

#include "utils.h"
#include "lizards_funcs.h"
#include "wasp_funcs.h"

#include "Lizard_list.h"

void *context;

// Function for running the proxy in a separate thread
void *run_proxy(void *args) {
    proxy_args *proxyArgs = (proxy_args *)args;
    zmq_proxy(proxyArgs->frontend, proxyArgs->backend, NULL);
    return NULL;
}


void *handleLizardMessage(){
    // long int thread_number = (long int)ptr;

    int id_lizardClient = 0;

    void *responder = zmq_socket (context, ZMQ_REP);
    int rc = zmq_connect (responder, "inproc://back-end-lizard");
    assert (rc == 0);



    while (1) {
        message_t lizards_msg;
        zmq_recv(responder, &lizards_msg, sizeof(lizards_msg), 0);
        LizardClient *lizardClient = findLizardClient(lizards_msg.ch);
        // Process the message based on its type
        switch (lizards_msg.msg_type)
        {
        case MSG_TYPE_LIZARD_CONNECT:
            id_lizardClient++;
            // Handle lizard connect
            handleLizardConnect(&lizards_msg, responder);

            // printGameState();
            break;

        case MSG_TYPE_LIZARD_MOVEMENT:
            // Handle lizard movement
            handleLizardMovement(&lizards_msg, responder);
            // printGameState();
            break;

        case MSG_TYPE_LIZARD_DISCONNECT:
            // Handle lizard disconnect
            handleLizardDisconnect(&lizards_msg, responder);
            // printGameState();
            break;

        case MSG_TYPE_LIZARD_RECONNECT:
            lizardClient->score = 0;
            renderLizardhead(lizardClient);
            renderLizardTail(lizardClient);
            break;
        }
        zmq_send(responder, &lizards_msg, sizeof(lizards_msg), 0);
        updateAndRenderEverything();
    }
    zmq_close (responder);
    return 0;

}

void* publishDisplayUpdates(void *publisher){
    while(1){
        handleDisplayUpdate(publisher);
        usleep(100);
    }
    return NULL;
}

void *handleRoachWaspMessage(){
    // long int thread_number = (long int)ptr;
    int id_roach=0;
    int id_wasp=0;

    void *socket_roach_wasp = zmq_socket(context, ZMQ_REP);
    int rc = zmq_connect(socket_roach_wasp, "inproc://back-end-roach-wasp");
    assert (rc == 0);
    while(1){
        // printWaspList();
        zmq_msg_t zmq_msg;
        zmq_msg_init(&zmq_msg);
        int msg_len = zmq_recvmsg(socket_roach_wasp, &zmq_msg, 0);
        LizardsNroachestypes__GameMessage *received_msg = lizards_nroachestypes__game_message__unpack(NULL, msg_len, zmq_msg_data(&zmq_msg));
        if(received_msg == NULL){
            printf("Error unpacking message\n");
            exit(1);
        }
        // Handling different message types
        int msg_type = received_msg->msg_type;
        switch (msg_type) {
            case LIZARDS_NROACHESTYPES__MESSAGE_TYPE__ROACHES_CONNECT:
                id_roach++;
                // Handle roaches connect
                handleRoachesConnect(received_msg, id_roach, socket_roach_wasp);
                // printRoachList();
                break;
            
            case LIZARDS_NROACHESTYPES__MESSAGE_TYPE__ROACHES_MOVEMENT:
                // Handle roaches movement
                handleRoachMovement(received_msg, socket_roach_wasp);
                // printRoachList();
                break;

            case LIZARDS_NROACHESTYPES__MESSAGE_TYPE__ROACHES_DISCONNECT:
                // Handle roaches disconnect
                handleRoachDisconnect(received_msg, socket_roach_wasp);
                break;

            case LIZARDS_NROACHESTYPES__MESSAGE_TYPE__WASPS_CONNECT:
                // Handle wasps connect
                id_wasp++;
                handleWaspsConnect(received_msg, socket_roach_wasp, id_wasp);
                break;

            case LIZARDS_NROACHESTYPES__MESSAGE_TYPE__WASPS_MOVEMENT:
                // Handle wasps movement
                handleWaspMovement(received_msg, socket_roach_wasp);
                break;

            case LIZARDS_NROACHESTYPES__MESSAGE_TYPE__WASPS_DISCONNECT:
                // Handle wasps disconnect
                handleWaspDisconnect(received_msg, socket_roach_wasp);
                break;

            default:
                printf("Unknown message type received.\n");
        }
        //Acknowledge the disconnection
        LizardsNroachestypes__GameMessage ack_msg = LIZARDS_NROACHESTYPES__GAME_MESSAGE__INIT;
        ack_msg.msg_type = LIZARDS_NROACHESTYPES__MESSAGE_TYPE__ACK;
        ack_msg.has_index = true;
        ack_msg.index = id_roach;
        size_t len = lizards_nroachestypes__game_message__get_packed_size(&ack_msg);
        void *buff = malloc(len);
        lizards_nroachestypes__game_message__pack(&ack_msg, buff);
        zmq_send(socket_roach_wasp, buff, len, 0);

        free(buff);
        
        updateAndRenderEverything();

        lizards_nroachestypes__game_message__free_unpacked(received_msg, NULL);
        zmq_msg_close(&zmq_msg);
    }
    return 0;
}


int main (void)
{
    // Initialize mutex and context
    pthread_mutex_init(&lizard_lock, NULL);
    pthread_mutex_init(&sharedGameState, NULL);
    pthread_mutex_init(&roach_wasps_lock, NULL);
    context = zmq_ctx_new ();

    // Socket for frontend for the roaches and wasps
    void *frontend = zmq_socket (context, ZMQ_ROUTER);
    int rc = zmq_bind (frontend, "tcp://*:5555");
    assert (rc == 0);

    // Socket for frontend for the lizards
    void *frontend_lizard = zmq_socket (context, ZMQ_ROUTER);
    rc = zmq_bind (frontend_lizard, "tcp://*:5556");
    assert (rc == 0);

    // Socket for backend_roaches_wasps 1
    void *backend_roaches_wasps = zmq_socket (context, ZMQ_DEALER);
    rc = zmq_bind (backend_roaches_wasps, "inproc://back-end-roach-wasp");
    assert (rc == 0);

    // Socket for backend_roaches_wasps 2
    void *backend_lizard = zmq_socket (context, ZMQ_DEALER);
    rc = zmq_bind (backend_lizard, "inproc://back-end-lizard");
    assert (rc == 0);

    // Socket for publisher
    void *publisher = zmq_socket(context, ZMQ_PUB);
    rc = zmq_bind(publisher, "tcp://*:5557");
    assert (rc == 0);

    
    proxy_args args1 = {frontend, backend_roaches_wasps};
    proxy_args args2 = {frontend_lizard, backend_lizard};

    // Start worker threads
    long int worker_nbr;
    pthread_t proxy1, proxy2;
    pthread_create(&proxy1, NULL, run_proxy, (void *)&args1);
    pthread_create(&proxy2, NULL, run_proxy, (void *)&args2);

    initGameState();

    pthread_t worker2[4];
    for (worker_nbr = 0; worker_nbr < 4; worker_nbr++) {
        pthread_create(&worker2[worker_nbr], NULL, handleLizardMessage, (void *) worker_nbr);
    }
    pthread_t worker;
    pthread_create(&worker, NULL, handleRoachWaspMessage, (void *) 5);
    pthread_t publisher_thread;
    pthread_create(&publisher_thread, NULL, publishDisplayUpdates, (void *) publisher);

    // Wait for proxy to finish
    pthread_join(proxy1, NULL);
    pthread_join(proxy2, NULL);

    for(int i = 0; i < 4; i++){
        pthread_join(worker2[i], NULL);
    }
    pthread_join(worker, NULL);
    pthread_join(publisher_thread, NULL);


    // Cleanup
    zmq_ctx_destroy(context);
    pthread_mutex_destroy(&lizard_lock);
    pthread_mutex_destroy(&sharedGameState);
    pthread_mutex_destroy(&roach_wasps_lock);
    zmq_ctx_destroy(context);
    zmq_close(frontend);
    zmq_close(backend_roaches_wasps);
    zmq_close(frontend_lizard);
    return 0;
}