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
    int NroachesTotal=0; 
    int nClients=0;

    int msg_type = 0;
    // int id = 0;
	
    void *context = zmq_ctx_new();
    void *socket_lizard = zmq_socket(context, ZMQ_REP);
    int rc = zmq_bind(socket_lizard, "tcp://*:5555");
    assert(rc == 0);

    void *socket_roach_wasp = zmq_socket(context, ZMQ_REP);
    int rc_roach_wasp = zmq_bind(socket_roach_wasp, "tcp://*:5557");
    assert(rc_roach_wasp == 0);

    void *socket_display = zmq_socket(context, ZMQ_PUB);
    int rc_display = zmq_bind(socket_display, "tcp://*:5556");
    assert(rc_display == 0);

    //Linked list to manage Lizard clients
    LizardClient* headLizardList = NULL;

    //Linked list to manage Roach clients
    RoachClient* headRoachList = NULL;

    // Linked List to manage Wasp clients
    WaspClient* headWaspList = NULL;


    WINDOW *my_win;
    setupWindows(&my_win);
 
    zmq_pollitem_t items [] = {
        { socket_lizard, 0, ZMQ_POLLIN, 0 },
        { socket_roach_wasp, 0, ZMQ_POLLIN, 0 }
    };

    do
    {
        updateTimer();

        zmq_poll(items, 2, -1);

        // Process Lizard messages
        if(items[0].revents & ZMQ_POLLIN){
            
            message_t lizard_msg;
            zmq_recv(socket_lizard, &lizard_msg, sizeof(lizard_msg), 0);
            switch (lizard_msg.msg_type)
            {
            case MSG_TYPE_LIZARD_CONNECT:
                handleLizardConnect(my_win, &headLizardList, &lizard_msg, socket_lizard, &nClients);
                break;

            case MSG_TYPE_LIZARD_MOVEMENT:
                if(headLizardList != NULL)
                    handleLizardMovement(my_win, &headLizardList, &headRoachList, &lizard_msg, socket_lizard, &headWaspList);
                break;

            case MSG_TYPE_LIZARD_DISCONNECT:
                handleLizardDisconnect(my_win, &headLizardList, &lizard_msg, socket_lizard, &nClients);
                break;
            }
        }

        // Process Roach and Wasp messages
        if (items[1].revents & ZMQ_POLLIN) {
            zmq_msg_t zmq_msg;
            zmq_msg_init(&zmq_msg);
            int msg_len = zmq_recvmsg(socket_roach_wasp, &zmq_msg, 0);
            LizardsNroachestypes__GameMessage *received_msg = lizards_nroachestypes__game_message__unpack(NULL, msg_len, zmq_msg_data(&zmq_msg));
            // Handling different message types
            msg_type = received_msg->msg_type;
            switch (msg_type) {
                case LIZARDS_NROACHESTYPES__MESSAGE_TYPE__ROACHES_CONNECT:
                    // Handle roaches connect
                    id_roach++;
                    handleRoachesConnect(my_win, &headRoachList, received_msg, socket_roach_wasp, &NroachesTotal, id_roach);
                    break;
                
                case LIZARDS_NROACHESTYPES__MESSAGE_TYPE__ROACHES_MOVEMENT:
                    // Handle roaches movement
                    handleRoachMovement(my_win, &headRoachList, received_msg, socket_roach_wasp, headLizardList, headWaspList);
                    break;

                case LIZARDS_NROACHESTYPES__MESSAGE_TYPE__ROACHES_DISCONNECT:
                    // Handle roaches disconnect
                    handleRoachDisconnect(my_win, &headRoachList, received_msg, socket_roach_wasp, &NroachesTotal);
                    break;

                default:
                    printf("Unknown message type received.\n");
            }

            lizards_nroachestypes__game_message__free_unpacked(received_msg, NULL);
            zmq_msg_close(&zmq_msg);
        }


        // Update display
        if(m.msg_type != MSG_TYPE_LIZARD_DISCONNECT && m.msg_type != MSG_TYPE_ROACHES_DISCONNECT && m.msg_type != MSG_TYPE_WASPS_DISCONNECT){
            m.msg_type = MSG_TYPE_ACK;
            updateRoachesVisibility(&headRoachList, id_roach);
            updateAndRenderLizardsTails(my_win, headLizardList);
            updateAndRenderLizardsHeads(my_win, headLizardList);
            updateAndRenderRoaches(my_win, headRoachList);
            updateAndRenderWasps(my_win, headWaspList);
        } 
	    handleDisplayUpdate(socket_display, headLizardList, headRoachList);
    } while (!flag_exit);
  	endwin();			/* End curses mode		  */
    zmq_close(socket_display);
    zmq_ctx_destroy(context);
    printf("Bye\n");
    disconnectAllLizards(&headLizardList, socket_lizard);
    zmq_close(socket_lizard);
    zmq_close(socket_roach_wasp);

	return 0;
}
