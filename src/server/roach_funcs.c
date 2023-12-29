/**
 * @file roach_funcs.c
 * @brief Implementation of functions related to RoachClient, including movement, connection handling, and disconnection.
 *
 * This file contains the definitions for functions responsible for handling RoachClient connections, movements,
 * and disconnections in the context of the game. It also includes functions for updating Roach positions and rendering.
 */

#include "roach_funcs.h"

void forceRoachDisconnect(message_t *m, void *socket){
    m->msg_type = MSG_TYPE_ROACHES_DISCONNECT;
    zmq_send(socket, &m, sizeof(*m), 0);
}
void new_position_roaches(RoachClient *roachClient, int id){
    switch (roachClient->roaches[id].direction)
    {
    case UP:
        (roachClient->roaches[id].position.position_x) --;
        if(roachClient->roaches[id].position.position_x == 0)
            roachClient->roaches[id].position.position_x = 2;
        break;
    case DOWN:
        (roachClient->roaches[id].position.position_x) ++;
        if(roachClient->roaches[id].position.position_x == WINDOW_HEIGHT-1)
            roachClient->roaches[id].position.position_x = WINDOW_HEIGHT-3;
        break;
    case LEFT:
        (roachClient->roaches[id].position.position_y) --;
        if(roachClient->roaches[id].position.position_y == 0)
            roachClient->roaches[id].position.position_y = 2;
        break;
    case RIGHT:
        (roachClient->roaches[id].position.position_y) ++;
        if(roachClient->roaches[id].position.position_y == WINDOW_WIDTH-1)
            roachClient->roaches[id].position.position_y = WINDOW_WIDTH-3;
        break;
    default:
        break;
    }
}

void cleanRoach(WINDOW *my_win, RoachClient *roachClient, int id){
    wmove(my_win, roachClient->roaches[id].position.position_x, roachClient->roaches[id].position.position_y);
    waddch(my_win, ' ');
    wrefresh(my_win);
}

void cleanRoachClient(WINDOW *my_win, RoachClient *roachClient){
    for (int i = 0; i < roachClient->num_roaches; i++)
    {
        cleanRoach(my_win, roachClient, i);
    }
}



void handleRoachesConnect(WINDOW *my_win, RoachClient **headRoachList, message_t *m, void *socket, int *NroachesTotal, int id_roach){
    m->index = id_roach;
    *NroachesTotal += m->N_roaches;
    if(*NroachesTotal>=MAX_ROACHES){
        forceRoachDisconnect(m, socket);
    } 
    else
    {
        m->msg_type = MSG_TYPE_ACK;
        addRoachClient(headRoachList, m->score_roaches, m->N_roaches, id_roach);
        RoachClient *roachClient = findRoachClient(headRoachList, id_roach);
        for (int i = 0; i < roachClient->num_roaches; i++)
        {
            wmove(my_win, roachClient->roaches[i].position.position_x, roachClient->roaches[i].position.position_y);
            waddch(my_win, '0' + roachClient->roaches[i].score);
        }
        wrefresh(my_win);
        zmq_send(socket, m, sizeof(*m), 0);
    }
}



void handleRoachMovement(WINDOW *my_win, RoachClient **headRoachList, message_t *m, direction_t direction,void *socket, LizardClient *headLizardList, WaspClient *headWaspList)
{
    int id_roach = m->index;
    int roach = m->roach_index;
    // printRoachList(*headRoachList);
    RoachClient *roachClient = findRoachClient(headRoachList, id_roach);
    if(roachClient == NULL){
        forceRoachDisconnect(m, socket);
    } else {
        roachClient->roaches[roach].direction = direction;
        m->msg_type = MSG_TYPE_ACK;
        if(roachClient->roaches[roach].on_board == 1){
            renderRoach(my_win, roachClient, roach, headLizardList, headWaspList);
        }
        
        zmq_send(socket, m, sizeof(*m), 0);
    }
}

void handleRoachDisconnect(WINDOW *my_win, RoachClient **headRoachList, message_t *m, void *socket, int *NroachesTotal){
    int id_roach = m->index;
    
    // Find the roach client that is disconnecting
    RoachClient *roachClient = findRoachClient(headRoachList, id_roach);
    if(roachClient == NULL){
        forceRoachDisconnect(m, socket);
        return;
    } else {
        cleanRoachClient(my_win, roachClient);
        // Update the total number of roaches
        *NroachesTotal -= roachClient->num_roaches;
        m->msg_type = MSG_TYPE_ACK;
        zmq_send(socket, m, sizeof(*m), 0);
        // Remove the roach client from the list
        removeRoachClient(headRoachList, id_roach);
    }
}


void disconnectAllRoaches(WINDOW *my_win, RoachClient **headRoachList, void *socket){
    RoachClient *currentRoach = *headRoachList;
    while(currentRoach != NULL){
        for (int i = 0; i < currentRoach->num_roaches; i++)
        {
            cleanRoach(my_win, currentRoach, i);
        }
        currentRoach = currentRoach->next;
    }
    freeRoachList(headRoachList);
    message_t m;
    m.msg_type = MSG_TYPE_ROACHES_DISCONNECT;
    zmq_send(socket, &m, sizeof(m), 0);
}

int checkPositionforRoach(RoachClient **headRoachList, position_t position){
    RoachClient *currentRoach = *headRoachList;
    while(currentRoach != NULL){
        for (int i = 0; i < currentRoach->num_roaches; i++)
        {
            if(comparePosition(currentRoach->roaches[i].position, position)){
                return 1;
            }
        }
        currentRoach = currentRoach->next;
    }
    return 0;
}