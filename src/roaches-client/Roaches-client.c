/**
 * @file Roaches-client.c
 * @brief Main program for the roaches client, responsible for connecting to the server, generating random movements, and sending them.
 *
 * This file contains the main program for the roaches client, which connects to the server, generates random movements, and sends them.
 */

#include "roaches_function.h"
#include <unistd.h>


int main() {
    // Signal handler for Ctrl+C
    signal(SIGINT, handle_signal);

    int n = 0;
    void *context = zmq_ctx_new();
    void *socket = zmq_socket(context, ZMQ_REQ);
    int rc = zmq_connect(socket, "tcp://localhost:5557");
    if(rc != 0){
        printf("Error connecting to server\n");
        exit(1);
    }

    LizardsNroachestypes__GameMessage roach_msg;
    lizards_nroachestypes__game_message__init(&roach_msg);
    roach_msg.msg_type = LIZARDS_NROACHESTYPES__MESSAGE_TYPE__ROACHES_CONNECT;    
    int n_roaches = rand() % 10 + 1;  // Random number of roaches between 1 and 10

    printf("N_roaches: %d\n", n_roaches);
    roach_msg.score_roaches = malloc(roach_msg.n_roaches * sizeof(int));
    roach_msg.n_score_roaches = n_roaches;
    for (int i = 0; i < n_roaches; i++) {
        roach_msg.score_roaches[i] = rand()%5 + 1;
        printf("Score: %d\n", roach_msg.score_roaches[i]);   
    }
    roach_msg.msg_type = LIZARDS_NROACHESTYPES__MESSAGE_TYPE__ROACHES_CONNECT;
    roach_msg.has_n_roaches = true;
    roach_msg.n_roaches = n_roaches;
    size_t size_msg = lizards_nroachestypes__game_message__get_packed_size(&roach_msg);
    void *msg_buf = malloc(size_msg);
    lizards_nroachestypes__game_message__pack(&roach_msg, msg_buf);
    zmq_send(socket, msg_buf, size_msg, 0);

    int msg_len = lizards_nroachestypes__game_message__get_packed_size(&roach_msg);
    msg_buf = malloc(msg_len);
    lizards_nroachestypes__game_message__pack(&roach_msg, msg_buf);   
    zmq_send(socket, msg_buf, msg_len, 0); 
    printf("Sent message: %p\n", msg_buf);
    free(msg_buf);
    free(roach_msg.score_roaches);

    zmq_msg_t ack;
    zmq_msg_init(&ack);
    zmq_msg_recv(&ack, socket, 0);
    msg_buf = zmq_msg_data(&ack);
    LizardsNroachestypes__GameMessage *ack_msg = lizards_nroachestypes__game_message__unpack(NULL, zmq_msg_size(&ack), msg_buf);
    if(ack_msg == NULL){
        printf("Error unpacking message\n");
        exit(1);
    }
    if (ack_msg->msg_type == LIZARDS_NROACHESTYPES__MESSAGE_TYPE__ROACHES_DISCONNECT) {
        printf("Received disconnect from server.\n");
        lizards_nroachestypes__game_message__free_unpacked(ack_msg, NULL);
        zmq_close(socket);
        zmq_ctx_destroy(context);
        printf("Bye\n");
        exit(1);
    }
    
    lizards_nroachestypes__game_message__free_unpacked(ack_msg, NULL);
    printf("index: %d\n", ack_msg->index);
    int index = ack_msg->index;

    //roach_msg.msg_type =MSG_TYPE_ROACHES_MOVEMENT;

    //int sleep_delay;
    direction_t direction;

    do {
        LizardsNroachestypes__GameMessage roach_msg;
        lizards_nroachestypes__game_message__init(&roach_msg);
        // Preencher os movimentos aleatórios para cada barata
        for (int i = 0; i < n_roaches; i++) {
            // Esperar um tempo aleatório antes do movimento de cada barata
           usleep(rand() % 3000000);  // Esperar até 3 segundos

            n++;
            direction = rand() % 4;
            //direction = random()%4;
            switch (direction)
            {
            case LEFT:
            printf("%d Going Left   \n", n);
                break;
            case RIGHT:
                printf("%d Going Right   \n", n);
                 break;
            case DOWN:
                printf("%d Going Down   \n", n);
                break;
            case UP:
                printf("%d Going Up    \n", n);
                break;
            }

            roach_msg.has_index = true;
            roach_msg.index = index;
            roach_msg.has_roach_index = true;
            roach_msg.roach_index = i;
            roach_msg.msg_type = LIZARDS_NROACHESTYPES__MESSAGE_TYPE__ROACHES_MOVEMENT;
            roach_msg.has_direction = true;
            roach_msg.direction = direction;
            size_t size_msg = lizards_nroachestypes__game_message__get_packed_size(&roach_msg);
            void *msg_buf = malloc(size_msg);
            lizards_nroachestypes__game_message__pack(&roach_msg, msg_buf);

            // Send the serialized message
            zmq_send(socket, msg_buf, size_msg, 0);
            free(msg_buf);
            free(roach_msg.score_roaches);

            zmq_msg_t ack;
            zmq_msg_init(&ack);
            zmq_msg_recv(&ack, socket, 0);
            printf("Received message\n");
            LizardsNroachestypes__GameMessage *ack_msg = lizards_nroachestypes__game_message__unpack(NULL, zmq_msg_size(&ack), zmq_msg_data(&ack));
            if(ack_msg == NULL){
                printf("Error unpacking message2\n");
                exit(1);
            }
            if (ack_msg->msg_type == LIZARDS_NROACHESTYPES__MESSAGE_TYPE__ROACHES_DISCONNECT) {
                printf("Received disconnect from server.\n");
                lizards_nroachestypes__game_message__free_unpacked(ack_msg, NULL);
                zmq_close(socket);
                zmq_ctx_destroy(context);
                printf("Bye\n");
                exit(1);
            }
            lizards_nroachestypes__game_message__free_unpacked(ack_msg, NULL);

        }
        if(flag_exit){
            LizardsNroachestypes__GameMessage disconnect_msg = LIZARDS_NROACHESTYPES__GAME_MESSAGE__INIT;
            disconnect_msg.msg_type = LIZARDS_NROACHESTYPES__MESSAGE_TYPE__ROACHES_DISCONNECT;
            disconnect_msg.has_index = true;
            disconnect_msg.index = index;
            
            size_msg = lizards_nroachestypes__game_message__get_packed_size(&disconnect_msg);
            msg_buf = malloc(size_msg);
            lizards_nroachestypes__game_message__pack(&disconnect_msg, msg_buf);
            zmq_send(socket, msg_buf, size_msg, 0);
            free(msg_buf);
            printf("You have been disconnected\n");
            zmq_close(socket);
            zmq_close(context);
            printf("Bye\n");
            exit(1);
        }
    }while (!flag_exit);

    endwin();			/* End curses mode		  */

    LizardsNroachestypes__GameMessage disconnect_msg;
    lizards_nroachestypes__game_message__init(&disconnect_msg);

    size_msg = lizards_nroachestypes__game_message__get_packed_size(&disconnect_msg);
    msg_buf = malloc(size_msg);
    lizards_nroachestypes__game_message__pack(&disconnect_msg, msg_buf);

    zmq_send(socket, msg_buf, size_msg, 0);
    zmq_close(socket);
    zmq_ctx_destroy(context);

    return 0;
}

