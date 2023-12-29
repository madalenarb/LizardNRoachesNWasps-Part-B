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
    message_t ACK_server;

    int n = 0;
    void *context = zmq_ctx_new();
    void *socket = zmq_socket(context, ZMQ_REQ);
    zmq_connect(socket, "tcp://localhost:5555");

    message_t m;

    m.msg_type = MSG_TYPE_ROACHES_CONNECT;

    m.N_roaches= rand() % 10 + 1;  // Número aleatório de roaches entre 1 e 10

    for (int i = 0; i < m.N_roaches; i++) {
       m.score_roaches[i] = rand() % 5 + 1;  // Score aleatório entre 1 e 5
       
    }

    zmq_send(socket, &m, sizeof(message_t), 0);
    zmq_recv(socket, &ACK_server, sizeof(message_t), 0);
    printf("client index: %d\n", ACK_server.index);
    if(ACK_server.msg_type == MSG_TYPE_DISCONNECT){
        zmq_close(socket);
        zmq_close(context);
        printf("Bye\n");
        exit(1);
    }

    m.roach_index = ACK_server.roach_index;   
    //roach_msg.msg_type =MSG_TYPE_ROACHES_MOVEMENT;

    //int sleep_delay;
    direction_t direction;
    m.index = ACK_server.index;

    do {
        // Preencher os movimentos aleatórios para cada barata
        for (int i = 0; i < m.N_roaches; i++) {
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
            m.roach_index = i;
            m.msg_type =MSG_TYPE_ROACHES_MOVEMENT;
            m.direction=direction;
            printf("roach client %d\n", m.index);
            printf("roach %d moving to %d\n", m.roach_index, m.direction);
            zmq_send(socket, &m, sizeof(message_t), 0);
            zmq_recv(socket, &ACK_server, 10, 0);  
            
            if(ACK_server.msg_type == MSG_TYPE_DISCONNECT){
                printf("You have been disconnected\n");
                zmq_close(socket);
                zmq_close(context);
                printf("Bye\n");
                exit(1);
            }      

        }
            
    }while (!flag_exit);
   
   // Adicione uma pequena pausa antes de encerrar
    sleep(1);

    endwin();			/* End curses mode		  */

    zmq_send(socket, &m, sizeof(m), 0);
    zmq_close(socket);
    zmq_ctx_destroy(context);

    return 0;
}

