/**
 * @file roach_funcs.c
 * @brief Implementation of functions related to RoachClient, including movement, connection handling, and disconnection.
 *
 * This file contains the definitions for functions responsible for handling RoachClient connections, movements,
 * and disconnections in the context of the game. It also includes functions for updating Roach positions and rendering.
 */

#include "roach_funcs.h"
#include "utils.h"

void forceRoachDisconnect(LizardsNroachestypes__GameMessage *m, void *socket){
    m->msg_type = LIZARDS_NROACHESTYPES__MESSAGE_TYPE__ROACHES_DISCONNECT;
    size_t len = lizards_nroachestypes__game_message__get_packed_size(m);
    void *buf = malloc(len);
    lizards_nroachestypes__game_message__pack(m, buf);
    zmq_send(socket, buf, len, 0);
    free(buf);

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

void cleanRoach(RoachClient *roachClient, int id){
    wmove(gameState.my_win, roachClient->roaches[id].position.position_x, roachClient->roaches[id].position.position_y);
    waddch(gameState.my_win, ' ');
    wrefresh(gameState.my_win);
}

void cleanRoachClient(RoachClient *roachClient){
    for (int i = 0; i < roachClient->num_roaches; i++)
    {
        cleanRoach(roachClient, i);
    }
}



void handleRoachesConnect(LizardsNroachestypes__GameMessage *m, int id_roach, void *socket){
    pthread_mutex_lock(&sharedGameState);
    gameState.numRoaches += m->n_roaches;
    if(gameState.numRoaches>=MAX_ROACHES){
        // Create and send disconnect message using protobuff
        LizardsNroachestypes__GameMessage disconnect_msg;
        lizards_nroachestypes__game_message__init(&disconnect_msg);
        disconnect_msg.msg_type = LIZARDS_NROACHESTYPES__MESSAGE_TYPE__ROACHES_DISCONNECT;
        forceRoachDisconnect(&disconnect_msg, socket);
    } else {
        addRoachClient(m->score_roaches, m->n_roaches, id_roach);
        RoachClient *roachClient = findRoachClient(id_roach);
        renderRoaches(roachClient);
        // Serielize the message
        LizardsNroachestypes__GameMessage response_msg;
        lizards_nroachestypes__game_message__init(&response_msg);
        response_msg.msg_type = LIZARDS_NROACHESTYPES__MESSAGE_TYPE__ACK;
        response_msg.has_index = true;
        response_msg.index = id_roach;
        int size_msg = lizards_nroachestypes__game_message__get_packed_size(&response_msg);
        void *buf = malloc(size_msg);
        lizards_nroachestypes__game_message__pack(&response_msg, buf);
        zmq_send(socket, buf, size_msg, 0);
        free(buf);
    }
    pthread_mutex_unlock(&sharedGameState);
}



void handleRoachMovement(LizardsNroachestypes__GameMessage *m, void *socket)
{
    int id_roach = m->index;
    int roach = m->roach_index;
    // printRoachList(*headRoachList);

    pthread_mutex_lock(&sharedGameState);
    RoachClient *roachClient = findRoachClient(id_roach);
    

    if(roachClient == NULL){
        pthread_mutex_unlock(&sharedGameState);
        LizardsNroachestypes__GameMessage disconnect_msg = LIZARDS_NROACHESTYPES__GAME_MESSAGE__INIT;
        forceRoachDisconnect(&disconnect_msg, socket);
    } else {
        // Update roachClient data based on protobuf message
        pthread_mutex_lock(&roach_wasps_lock);
        roachClient->roaches[roach].direction = m->direction;
        if(roachClient->roaches[roach].on_board == 1){
            renderRoach(roach, roachClient);
        }
        pthread_mutex_unlock(&roach_wasps_lock);
        
        // Prepare ACK message
        LizardsNroachestypes__GameMessage ACK_msg = LIZARDS_NROACHESTYPES__GAME_MESSAGE__INIT;
        ACK_msg.msg_type = LIZARDS_NROACHESTYPES__MESSAGE_TYPE__ACK;
        // Serialize and send ACK message
        size_t len = lizards_nroachestypes__game_message__get_packed_size(&ACK_msg);
        void *buf = malloc(len);
        lizards_nroachestypes__game_message__pack(&ACK_msg, buf);
        zmq_send(socket, buf, len, 0);
        free(buf);
    }
    pthread_mutex_unlock(&sharedGameState);
}

void handleRoachDisconnect(LizardsNroachestypes__GameMessage *m, void *socket){
    int id_roach = m->index;
    
    // Find the roach client that is disconnecting
    pthread_mutex_lock(&sharedGameState);
    pthread_mutex_lock(&roach_wasps_lock);
    RoachClient *roachClient = findRoachClient(id_roach);
    if(roachClient == NULL){
        pthread_mutex_unlock(&roach_wasps_lock);
        pthread_mutex_unlock(&sharedGameState);
        LizardsNroachestypes__GameMessage disconnect_msg = LIZARDS_NROACHESTYPES__GAME_MESSAGE__INIT;
        disconnect_msg.msg_type = LIZARDS_NROACHESTYPES__MESSAGE_TYPE__ROACHES_DISCONNECT;
        forceRoachDisconnect(&disconnect_msg, socket);
        return;
    } else {
        cleanRoachClient(roachClient);
        // Update the total number of roaches
        gameState.numRoaches -= roachClient->num_roaches;
        
        //Acknowledge the disconnection
        LizardsNroachestypes__GameMessage ack_msg = LIZARDS_NROACHESTYPES__GAME_MESSAGE__INIT;
        ack_msg.msg_type = LIZARDS_NROACHESTYPES__MESSAGE_TYPE__ACK;

        //Serialize and send the ACK message
        size_t len = lizards_nroachestypes__game_message__get_packed_size(&ack_msg);
        void *buff = malloc(len);
        lizards_nroachestypes__game_message__pack(&ack_msg, buff);
        zmq_send(socket, buff, len, 0);
        free(buff);
        // Remove the roach client from the list
        removeRoachClient(id_roach);
    }
    pthread_mutex_unlock(&roach_wasps_lock);
    pthread_mutex_unlock(&sharedGameState);
}


void disconnectAllRoaches(void *socket){
    pthread_mutex_lock(&sharedGameState);
    RoachClient *currentRoach = gameState.headRoachList;
    pthread_mutex_lock(&roach_wasps_lock);
    while(currentRoach != NULL){
        pthread_mutex_lock(&sharedGameState);
        for (int i = 0; i < currentRoach->num_roaches; i++)
        {
            cleanRoach(currentRoach, i);
        }
        currentRoach = currentRoach->next;
    }
    pthread_mutex_unlock(&roach_wasps_lock);
    freeRoachList();
    pthread_mutex_unlock(&sharedGameState);
    LizardsNroachestypes__GameMessage disconnect_msg;
    lizards_nroachestypes__game_message__init(&disconnect_msg);
    disconnect_msg.msg_type = LIZARDS_NROACHESTYPES__MESSAGE_TYPE__ROACHES_DISCONNECT;
    
    // Serialize and send the protobuf message
    size_t len = lizards_nroachestypes__game_message__get_packed_size(&disconnect_msg);
    void *buf = malloc(len);
    lizards_nroachestypes__game_message__pack(&disconnect_msg, buf);
    printf("Sending disconnect message disconnect_msg: %d\n", disconnect_msg.msg_type);
    zmq_send(socket, buf, len, 0);
    free(buf);
}

int checkPositionforRoach(position_t position){
    RoachClient *currentRoach = gameState.headRoachList;
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