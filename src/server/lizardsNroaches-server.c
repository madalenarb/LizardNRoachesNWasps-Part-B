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

#define LENGTH_ARRAY 100000
int random_numbers[LENGTH_ARRAY];
int prime_array[LENGTH_ARRAY]; // exercise 5
int prime_count = 0;
int next_random_index = 0;
pthread_mutex_t lock;
pthread_mutex_t sharedNumberLock;
//void *context;
void *context;

typedef struct SharedNumbers {
    int number;
    int isShared;
    int count;
} SharedNumbers;


SharedNumbers sharedNumber = {0, 0, 0};
/**
 * @brief funcao que verifica se um número é primo
 * 
 * @param value valor a verificar se é primo
 * @return int 1 se value for primo 0, se não for primo
 */
int verify_prime( int value){
    int divisor = 2;
    if(value < 4){
        return 1;
    }
    while (value%divisor != 0){
        divisor ++;
    }
    if (value == divisor){
        return 1;
    }else{
        return 0;
    }
}


// Function for running the proxy in a separate thread
void *run_proxy(void *args) {
    printf("Starting proxy...\n");
    proxy_args *proxyArgs = (proxy_args *)args;
    zmq_proxy(proxyArgs->frontend, proxyArgs->backend, NULL);
    return NULL;
}


void *handleLizardMessage( void *ptr ){
    long int thread_number = (long int)ptr;

    int id_lizardClient = 0;

    void *responder = zmq_socket (context, ZMQ_REP);
    int rc = zmq_connect (responder, "inproc://back-end2");
    assert (rc == 0);

    printf("Thread %ld ready\n", thread_number);
    while (1) {
        message_t lizards_msg;
        zmq_recv(responder, &lizards_msg, sizeof(lizards_msg), 0);
        printf("Thread %ld received message type %d\n", thread_number, lizards_msg.msg_type);
        pthread_mutex_lock(&lock);
        // Process the message based on its type
        switch (lizards_msg.msg_type)
        {
        case MSG_TYPE_LIZARD_CONNECT:
            id_lizardClient++;
            printf("Lizard client %d connected\n", id_lizardClient);
            // Handle lizard connect
            handleLizardConnect(&lizards_msg, responder);
            printf("Lizard client %d password: %ld\n", id_lizardClient, lizards_msg.password);

            printGameState();
            break;

        case MSG_TYPE_LIZARD_MOVEMENT:
            printf("Lizard client %c moved %d\n", lizards_msg.ch, lizards_msg.direction);
            // Handle lizard movement
            handleLizardMovement(&lizards_msg, responder);
            printGameState();
            break;

        case MSG_TYPE_LIZARD_DISCONNECT:
            printf("Lizard client %d disconnected\n", id_lizardClient);
            // Handle lizard disconnect
            handleLizardDisconnect(&lizards_msg, responder);
            printGameState();
            break;
        }
        pthread_mutex_unlock(&lock);
        zmq_send(responder, &lizards_msg, sizeof(lizards_msg), 0);
    }
    zmq_close (responder);
    return 0;

}

void *handleRoachWaspMessage( void *ptr ){
    long int thread_number = (long int)ptr;
    int id_roach=0;
    int id_wasp=0;

    void *socket_roach_wasp = zmq_socket(context, ZMQ_REP);
    int rc = zmq_connect(socket_roach_wasp, "inproc://back-end");
    assert (rc == 0);
    while(1){
        printWaspList();
        zmq_msg_t zmq_msg;
        zmq_msg_init(&zmq_msg);
        int msg_len = zmq_recvmsg(socket_roach_wasp, &zmq_msg, 0);
        LizardsNroachestypes__GameMessage *received_msg = lizards_nroachestypes__game_message__unpack(NULL, msg_len, zmq_msg_data(&zmq_msg));
        if(received_msg == NULL){
            printf("Error unpacking message\n");
            exit(1);
        }
        printf("Thread %ld received message type %d\n", thread_number, received_msg->msg_type);
        // Handling different message types
        int msg_type = received_msg->msg_type;
        switch (msg_type) {
            case LIZARDS_NROACHESTYPES__MESSAGE_TYPE__ROACHES_CONNECT:
                id_roach++;
                printf("Roach client %d connected\n", id_roach);
                // Handle roaches connect
                handleRoachesConnect(received_msg, id_roach, socket_roach_wasp);
                printRoachList();
                break;
            
            case LIZARDS_NROACHESTYPES__MESSAGE_TYPE__ROACHES_MOVEMENT:
                printf("Roach client %d moved %d\n", received_msg->index, received_msg->direction);
                // Handle roaches movement
                handleRoachMovement(received_msg, socket_roach_wasp);
                printRoachList();
                break;

            case LIZARDS_NROACHESTYPES__MESSAGE_TYPE__ROACHES_DISCONNECT:
                printf("Roach client %d disconnected\n", id_roach);
                // Handle roaches disconnect
                handleRoachDisconnect(received_msg, socket_roach_wasp);
                break;

            case LIZARDS_NROACHESTYPES__MESSAGE_TYPE__WASPS_CONNECT:
                // Handle wasps connect
                id_wasp++;
                printf("Wasp client %d connected\n", id_wasp);
                handleWaspsConnect(received_msg, socket_roach_wasp, id_wasp);
                break;

            case LIZARDS_NROACHESTYPES__MESSAGE_TYPE__WASPS_MOVEMENT:
                printf("Wasp client %d moved %d\n", received_msg->index, received_msg->direction);
                // Handle wasps movement
                handleWaspMovement(received_msg, socket_roach_wasp);
                break;

            case LIZARDS_NROACHESTYPES__MESSAGE_TYPE__WASPS_DISCONNECT:
                printf("Wasp client %d disconnected\n", id_wasp);
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
        
        lizards_nroachestypes__game_message__free_unpacked(received_msg, NULL);
        zmq_msg_close(&zmq_msg);

    }
    return 0;
}


int main (void)
{
    // Initialize mutex and context
    pthread_mutex_init(&lock, NULL);
    pthread_mutex_init(&sharedNumberLock, NULL);
    context = zmq_ctx_new ();

    // Initialize random numbers array
    srand(time(NULL)); 
    for(int i = 0; i < LENGTH_ARRAY; i++){
        random_numbers[i] = rand()/10; 
    }

    // Socket for frontend 1
    void *frontend = zmq_socket (context, ZMQ_ROUTER);
    int rc = zmq_bind (frontend, "tcp://*:5555");
    assert (rc == 0);

    // Socket for frontend 2
    void *frontend2 = zmq_socket (context, ZMQ_ROUTER);
    rc = zmq_bind (frontend2, "tcp://*:5556");
    assert (rc == 0);

    // Socket for backend 1
    void *backend = zmq_socket (context, ZMQ_DEALER);
    rc = zmq_bind (backend, "inproc://back-end");
    assert (rc == 0);

    // Socket for backend 2
    void *backend2 = zmq_socket (context, ZMQ_DEALER);
    rc = zmq_bind (backend2, "inproc://back-end2");
    assert (rc == 0);
    
    proxy_args args1 = {frontend, backend};
    proxy_args args2 = {frontend2, backend2};

    // Start worker threads
    long int worker_nbr;
    pthread_t proxy1, proxy2;
    pthread_create(&proxy1, NULL, run_proxy, (void *)&args1);
    pthread_create(&proxy2, NULL, run_proxy, (void *)&args2);

    for (worker_nbr = 0; worker_nbr < 4; worker_nbr++) {
        pthread_t worker2;
        pthread_create(&worker2, NULL, handleLizardMessage, (void *) worker_nbr);
    }
    pthread_t worker;
    pthread_create(&worker, NULL, handleRoachWaspMessage, (void *) 5);

    // Wait for proxy to finish
    pthread_join(proxy1, NULL);
    pthread_join(proxy2, NULL);

    // Cleanup
    zmq_ctx_destroy(context);
    pthread_mutex_destroy(&lock);
    return 0;
}