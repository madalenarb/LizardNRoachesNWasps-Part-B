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
    message_t ACK_server, disconnect_msg;

    int n = 0;
    void *context = zmq_ctx_new();
    void *socket = zmq_socket(context, ZMQ_REQ);
    zmq_connect(socket, "tcp://localhost:5555");

    LizardsNroachestypes__GameMessage m = LIZARDS_NROACHESTYPES__GAME_MESSAGE__INIT;
    m.msg_type = LIZARDS_NROACHESTYPES__MESSAGE_TYPE__WASPS_CONNECT;
    m.n_wasps = rand() % 10 + 1;  // Random number of wasps between 1 and 10

    // Serialize the message
    size_t len = lizards_nroachestypes__game_message__get_packed_size(&m);
    void *buf = malloc(len);
    lizards_nroachestypes__game_message__pack(&m, buf);

    // Send serialized message to server
    zmq_send(socket, buf, len, 0);
    printf("Sent %d wasps\n", m.n_wasps);
    free(buf);

    zmq_recv(socket, &ACK_server, sizeof(message_t), 0);
    printf("ACK_server.msg_type: %d\n", ACK_server.msg_type);
    printf("client index: %d\n", ACK_server.index);
    if(ACK_server.msg_type == MSG_TYPE_WASPS_DISCONNECT){
        zmq_close(socket);
        zmq_ctx_destroy(context);
        printf("Bye\n");
        exit(1);
    }

    m.wasp_index = ACK_server.wasp_index;   
    direction_t direction;
    m.index = ACK_server.index;

    do {
        // Preencher os movimentos aleatórios para cada barata
        for (int i = 0; i < m.n_wasps; i++) {
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

            m.index = ACK_server.index;
            m.wasp_index = i;
            m.msg_type =MSG_TYPE_WASPS_MOVEMENT;
            m.direction=direction;
            printf("wasp client %d\n", m.index);
            printf("wasp %d moving to %d\n", m.wasp_index, m.direction);
            zmq_send(socket, &m, sizeof(message_t), 0);
            zmq_recv(socket, &ACK_server, 10, 0);  
            
            if(ACK_server.msg_type == MSG_TYPE_LIZARD_DISCONNECT || flag_exit == 1){
                disconnect_msg.msg_type = MSG_TYPE_WASPS_DISCONNECT;
                disconnect_msg.index = ACK_server.index;
                zmq_send(socket, &disconnect_msg, sizeof(message_t), 0);
                zmq_recv(socket, &ACK_server, sizeof(ACK_server), 0);
                zmq_close(socket);
                printf("Bye\n");
                break;
            }      
        }
    } while (!flag_exit && ACK_server.msg_type != MSG_TYPE_WASPS_DISCONNECT);
   
   // Adicione uma pequena pausa antes de encerrar

    endwin();			/* End curses mode		  */
    zmq_close(socket);
    zmq_ctx_destroy(context);

    return 0;
}

