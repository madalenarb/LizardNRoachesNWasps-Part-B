/**
 * @file Lizard-client.c
 * @brief Main program for the lizard client, responsible for connecting to the server, handling user input, and sending movements.
 *
 * This file contains the main program for the lizard client, which connects to the server, handles user input, and sends lizard movements.
 */

#include "Lizard-funcs.h"

int main (){

    char direction_name[4][10] = {"UP", "DOWN", "LEFT", "RIGHT"};
    int password = 0;
    void *context = zmq_ctx_new ();
    signal(SIGINT, handle_signal);
    int score = 0;
    void *requester = zmq_socket (context, ZMQ_REQ);
    zmq_connect (requester, "tcp://localhost:5556");

    void *subscriber = zmq_socket (context, ZMQ_SUB);
    zmq_connect (subscriber, "tcp://localhost:5557");
    zmq_setsockopt (subscriber, ZMQ_SUBSCRIBE, "", 0);

    message_t msg;
    msg.msg_type = MSG_TYPE_LIZARD_CONNECT;
    char ch;

    do{
        printf("what is your character(a..z)?: ");
        ch = getchar();
        ch = tolower(ch);  
    } while (!isalpha(ch));

    msg.ch = ch;
    msg.direction = -1;
    zmq_send(requester, &msg, sizeof(msg), 0);
    message_t ack_msg, resp;
    ack_msg.msg_type = MSG_TYPE_ACK;
    zmq_recv(requester, &ack_msg, sizeof(ack_msg), 0);
    password = ack_msg.password;

    int key;
    initscr();			/* Start curses mode 		*/
    cbreak();				/* Line buffering disabled	*/
	keypad(stdscr, TRUE);		/* We get F1, F2 etc..		*/
	noecho();			/* Don't echo() while we do getch */
    printf("Press q to exit\n");
    int loss;
    do{
        loss = 0;
        // n = rand();
        // Asks for a number
        msg.msg_type = MSG_TYPE_LIZARD_MOVEMENT;
        msg.password = password;
        
        if(score < 0){
            loss = lizardsLoses(&msg);
        }
        
        key = getch();
        select_direction(key, &msg);

        zmq_send(requester, &msg, sizeof(msg), 0);      
        zmq_recv(requester, &resp, sizeof(resp), 0);

        if(key == 'q' || flag_exit || loss)
            break;
            
        score = resp.score_lizard;  

        refresh();
        mvprintw(0, 0, "You pressed %s     score: %d   ", direction_name[msg.direction], score);
        
    } while (key != 'q' && !flag_exit && !loss);
    endwin();			/* End curses mode		  */
    msg.msg_type = MSG_TYPE_LIZARD_DISCONNECT;
    zmq_send(requester, &msg, sizeof(msg), 0);
    zmq_recv(requester, &ack_msg, sizeof(ack_msg), 0);
    zmq_close(requester);
    zmq_ctx_destroy(context);
    return 0;
}