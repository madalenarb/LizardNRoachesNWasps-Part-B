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

/*
#include <ncurses.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>  
#include <stdlib.h>
#include <assert.h>
#include <zmq.h>
*/

int main()
{
    initializeTimer();
    signal(SIGINT, handle_signal);
    message_t m;
    int id_roach=0;
    int direction = 0;
    int NroachesTotal=0; 
    int nClients=0;
    // int id = 0;
	
    void *context = zmq_ctx_new();
    void *socket = zmq_socket(context, ZMQ_REP);
    int rc = zmq_bind(socket, "tcp://*:5555");
    assert(rc == 0);

    //acrescentei agora esta parte
    void *socket_display = zmq_socket(context, ZMQ_PUB);
    int rc_display = zmq_bind(socket_display, "tcp://*:5556");
    assert(rc_display == 0);

    //Linked list to manage Lizard clients
    LizardClient* headLizardList = NULL;

    //Linked list to manage Roach clients
    RoachClient* headRoachList = NULL;


    WINDOW *my_win;
    setupWindows(&my_win);
 
    do
    {
        updateTimer();
        zmq_recv(socket, &m, sizeof(message_t), 0);
        switch (m.msg_type)
        {
        case MSG_TYPE_LIZARD_CONNECT:
            handleLizardConnect(my_win, &headLizardList, &m, socket, &nClients);
            break;
        
        case MSG_TYPE_LIZARD_MOVEMENT:
            if(headLizardList != NULL)
                handleLizardMovement(my_win, &headLizardList, &headRoachList, &m, socket);
            break;
        
        case MSG_TYPE_DISCONNECT:
            handleLizardDisconnect(my_win, &headLizardList, &m, socket, &nClients);
            break;

        case MSG_TYPE_ROACHES_CONNECT:
            id_roach++;
            handleRoachesConnect(my_win, &headRoachList, &m, socket, &NroachesTotal, id_roach);
            break;

        case MSG_TYPE_ROACHES_MOVEMENT:
            direction = m.direction;
            handleRoachMovement(my_win, &headRoachList, &m, direction, socket, headLizardList);
            break;

        default:
            break;
        }
        if(m.msg_type != MSG_TYPE_DISCONNECT){
            m.msg_type = MSG_TYPE_ACK;
            updateRoachesVisibility(&headRoachList, id_roach);
            updateAndRenderLizardsTails(my_win, headLizardList);
            updateAndRenderLizardsHeads(my_win, headLizardList);
            updateAndRenderRoaches(my_win, headRoachList);
        } else {
            zmq_send(socket, &m, sizeof(m), 0);
        }
	    handleDisplayUpdate(socket_display, headLizardList, headRoachList);
    } while (!flag_exit);
  	endwin();			/* End curses mode		  */
    printf("Bye\n");
    disconnectAllLizards(&headLizardList, socket);
    zmq_close(socket_display);
    zmq_close(socket);
    zmq_ctx_destroy(context);
	return 0;
}
