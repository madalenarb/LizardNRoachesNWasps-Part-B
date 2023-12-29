/**
 * @file Lizard-client.c
 * @brief Main program for the lizard client, responsible for connecting to the server, handling user input, and sending movements.
 *
 * This file contains the main program for the lizard client, which connects to the server, handles user input, and sends lizard movements.
 */

#include "Lizard-funcs.h"

int main()
{
    int password = 0;
    message_t ACK_server;
    char direction_name[4][10] = {"UP", "DOWN", "LEFT", "RIGHT"};
    int score = 0;
    signal(SIGINT, handle_signal);
    // Initialize the ZeroMQ context and socket
    void *context = zmq_ctx_new();

    void *socket = zmq_socket(context, ZMQ_REQ);
    zmq_connect(socket, "tcp://localhost:5555");


    char ch;
    do{
        printf("what is your character(a..z)?: ");
        ch = getchar();
        ch = tolower(ch);  
    } while (!isalpha(ch));

    // send connection message
    message_t m;
    m.msg_type = MSG_TYPE_LIZARD_CONNECT;
    m.ch = ch;
    m.direction = -1;
    zmq_send(socket, &m, sizeof(message_t), 0);
    zmq_recv(socket, &ACK_server, sizeof(message_t), 0);
    password = ACK_server.password;
    mvprintw(0,0,"score: %d", score);
    if(ACK_server.msg_type == MSG_TYPE_DISCONNECT || ACK_server.msg_type != MSG_TYPE_ACK){
        zmq_close(socket);
        zmq_close(context);
        printf("You have been disconnected\n");
        printf("Bye\n");
        exit(1);
    }

	initscr();			/* Start curses mode 		*/
    // cbreak();				/* Line buffering disabled	*/
	keypad(stdscr, TRUE);		/* We get F1, F2 etc..		*/
	// noecho();			/* Don't echo() while we do getch */
    mvprintw(0,0,"score: %d", score);
    int n = 0;

    //TODO_9

    // prepare the movement message
    m.msg_type = MSG_TYPE_LIZARD_MOVEMENT;
    int key;
    do
    { 
        m.password = password;
    	key = getch();	
        
        select_direction(key, &m);

        if (key != 'q' && key != ERR){
            n++;
            m.msg_type = MSG_TYPE_LIZARD_MOVEMENT;
            zmq_send(socket, &m, sizeof(message_t), 0);
            zmq_recv(socket, &ACK_server, sizeof(message_t), 0);
            if(ACK_server.msg_type == MSG_TYPE_DISCONNECT || ACK_server.msg_type != MSG_TYPE_ACK){
                endwin();
                zmq_close(socket);
                zmq_ctx_destroy(context);
                printf("You have been disconnected\n");
                printf("Bye\n");
                exit(1);
            }
            if(ACK_server.ch == ch){
                score = ACK_server.score_lizard;
            }
            
            mvprintw(0,0,"%d: Score: %d, %s    ", n, score, direction_name[m.direction]);
        }
        refresh();			/* Print it on to the real screen */
    }while(key != 'q' && !flag_exit);
  	endwin();			/* End curses mode		  */
    m.msg_type = MSG_TYPE_DISCONNECT;
    zmq_send(socket, &m, sizeof(m), 0);
    zmq_close(socket);
    zmq_ctx_destroy(context);
	return 0;
} 