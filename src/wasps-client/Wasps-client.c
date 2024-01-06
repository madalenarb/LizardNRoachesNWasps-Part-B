/**
 * @file Roaches-client.c
 * @brief Main program for the roaches client, responsible for connecting to the server, generating random movements, and sending them.
 *
 * This file contains the main program for the roaches client, which connects to the server, generates random movements, and sends them.
 */

#include "wasps_function.h"
#include <unistd.h>
#include "../../protos/c_out/protocol.pb-c.h"

int main() {
    // Signal handler for Ctrl+C
    signal(SIGINT, handle_signal);

    int n = 0;
    void *context = zmq_ctx_new();
    void *socket = zmq_socket(context, ZMQ_REQ);
    int rc = zmq_connect(socket, "tcp://localhost:5555");
    if(rc != 0){
        printf("Error connecting to server\n");
        exit(1);
    }

    LizardsNroachestypes__GameMessage wasp_msg;
    lizards_nroachestypes__game_message__init(&wasp_msg);
    wasp_msg.msg_type = LIZARDS_NROACHESTYPES__MESSAGE_TYPE__WASPS_CONNECT;
    wasp_msg.has_n_wasps = true;
    int n_wasps = rand() % 10 + 1;  // Random number of wasps between 1 and 10
    
    wasp_msg.n_wasps = n_wasps;

    // Serialize the message
    size_t size_msg = lizards_nroachestypes__game_message__get_packed_size(&wasp_msg);
    void *buf = malloc(size_msg);
    lizards_nroachestypes__game_message__pack(&wasp_msg, buf);

    // Send serialized message to server
    zmq_send(socket, buf, size_msg, 0);

    zmq_msg_t ack;
    zmq_msg_init(&ack);
    zmq_msg_recv(&ack, socket, 0);
    LizardsNroachestypes__GameMessage *ack_msg = lizards_nroachestypes__game_message__unpack(NULL, zmq_msg_size(&ack), zmq_msg_data(&ack));
    if(ack_msg == NULL){
        printf("Error unpacking message1\n");
        exit(1);
    }
    free(buf);
    if(ack_msg->msg_type == LIZARDS_NROACHESTYPES__MESSAGE_TYPE__WASPS_DISCONNECT){
        lizards_nroachestypes__game_message__free_unpacked(ack_msg, NULL);
        zmq_close(socket);
        zmq_ctx_destroy(context);
        exit(1);
    }

    int index = ack_msg->index;
    printf("Wasp client %d connected\n", index);
    direction_t direction;
    lizards_nroachestypes__game_message__free_unpacked(ack_msg, NULL);

    do {
        // Preencher os movimentos aleatórios para cada barata
        for (int i = 0; i < n_wasps; i++) {
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

            LizardsNroachestypes__GameMessage movement_msg;
            lizards_nroachestypes__game_message__init(&movement_msg);
            movement_msg.has_index = true;
            movement_msg.index = index;
            movement_msg.has_wasp_index = true;
            movement_msg.wasp_index = i;
            movement_msg.msg_type = LIZARDS_NROACHESTYPES__MESSAGE_TYPE__WASPS_MOVEMENT;
            movement_msg.has_direction = true;
            movement_msg.direction = direction;
            size_t size_msg = lizards_nroachestypes__game_message__get_packed_size(&movement_msg);
            void *buf = malloc(size_msg);
            lizards_nroachestypes__game_message__pack(&movement_msg, buf);
            zmq_send(socket, buf, size_msg, 0);
            free(buf);
            zmq_msg_t ack;
            zmq_msg_init(&ack);
            zmq_msg_recv(&ack, socket, 0);
            LizardsNroachestypes__GameMessage *ack_msg = lizards_nroachestypes__game_message__unpack(NULL, zmq_msg_size(&ack), zmq_msg_data(&ack));
            if(ack_msg == NULL){
                printf("Error unpacking message\n");
                exit(1);
            }
            if(ack_msg->msg_type == MSG_TYPE_LIZARD_DISCONNECT || flag_exit == 1){
                LizardsNroachestypes__GameMessage disconnect_msg;
                lizards_nroachestypes__game_message__init(&disconnect_msg);
                disconnect_msg.msg_type = LIZARDS_NROACHESTYPES__MESSAGE_TYPE__WASPS_DISCONNECT;
                disconnect_msg.has_index = true;
                disconnect_msg.index = index;
                size_t len = lizards_nroachestypes__game_message__get_packed_size(&disconnect_msg);
                void *buf = malloc(len);
                lizards_nroachestypes__game_message__pack(&disconnect_msg, buf);
                zmq_send(socket, buf, len, 0);
                free(buf);
                zmq_close(socket);
                printf("Bye\n");
                break;
            }      
        }
    } while (!flag_exit && ack_msg->msg_type != MSG_TYPE_LIZARD_DISCONNECT);
   
   // Adicione uma pequena pausa antes de encerrar

    endwin();			/* End curses mode		  */
    zmq_close(socket);
    zmq_ctx_destroy(context);

    return 0;
}

