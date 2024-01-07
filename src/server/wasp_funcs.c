/**
 * @file wasp_funcs.c
 * @brief Implementation of functions related to WaspClient, including movement, connection handling, and disconnection.
 *
 * This file contains the definitions for functions responsible for handling WaspClientt connections, movements,
 * and disconnections in the context of the game. It also includes functions for updating Roach positions and rendering.
 */

#include "wasp_funcs.h"

void forceWaspDisconnect(LizardsNroachestypes__GameMessage *m, void *socket){
    m->msg_type = LIZARDS_NROACHESTYPES__MESSAGE_TYPE__WASPS_DISCONNECT;
    size_t len = lizards_nroachestypes__game_message__get_packed_size(m);
    void *buf = malloc(len);
    lizards_nroachestypes__game_message__pack(m, buf);
    zmq_send(socket, buf, len, 0);
    free(buf);
}

void new_position_wasps(WaspClient *waspClient, int id){
    switch (waspClient->wasps[id].direction)
    {
    case UP:
        (waspClient->wasps[id].position.position_x) --;
        if(waspClient->wasps[id].position.position_x == 0)
            waspClient->wasps[id].position.position_x = 2;
        break;
    case DOWN:
        (waspClient->wasps[id].position.position_x) ++;
        if(waspClient->wasps[id].position.position_x == WINDOW_HEIGHT-1)
            waspClient->wasps[id].position.position_x = WINDOW_HEIGHT-3;
        break;
    case LEFT:
        (waspClient->wasps[id].position.position_y) --;
        if(waspClient->wasps[id].position.position_y == 0)
            waspClient->wasps[id].position.position_y = 2;
        break;
    case RIGHT:
        (waspClient->wasps[id].position.position_y) ++;
        if(waspClient->wasps[id].position.position_y == WINDOW_WIDTH-1)
            waspClient->wasps[id].position.position_y = WINDOW_WIDTH-3;
        break;
    default:
        break;
    }
}

void cleanWasp(WaspClient *waspClient, int id){
    wmove(gameState.my_win, waspClient->wasps[id].position.position_x, waspClient->wasps[id].position.position_y);
    waddch(gameState.my_win, ' ');
    wrefresh(gameState.my_win);
}

void cleanWaspClient(WaspClient *waspClient){
    for (int i = 0; i < waspClient->num_wasps; i++)
    {
        cleanWasp(waspClient, i);
    }
}

void handleWaspsConnect(LizardsNroachestypes__GameMessage *m, void *socket, int id_wasp){
    pthread_mutex_lock(&sharedGameState);
    m->index = id_wasp; 
    gameState.numWasps += m->n_wasps;  // Update the total number of wasps
    if(gameState.numWasps > MAX_WASPS){
        forceWaspDisconnect(m, socket);  // If the total number of wasps exceeds the maximum, disconnect the wasp client
        pthread_mutex_unlock(&sharedGameState);
    } else {
        m->msg_type = LIZARDS_NROACHESTYPES__MESSAGE_TYPE__ACK;
        addWaspClient(m->n_wasps, id_wasp);  // Add the wasp client to the list
        pthread_mutex_lock(&roach_wasps_lock);
        WaspClient *waspClient = findWaspClient(id_wasp);  // Change to findWaspClient

        // Add the wasps to the game
        for (int i = 0; i < waspClient->num_wasps; i++)  // Altere para num_wasps
        {
            wmove(gameState.my_win, waspClient->wasps[i].position.position_x, waspClient->wasps[i].position.position_y);
            waddch(gameState.my_win, '#');  // Representa vespas com '#'
        }
        pthread_mutex_unlock(&roach_wasps_lock);

        wrefresh(gameState.my_win);
        pthread_mutex_unlock(&sharedGameState);

        // Serialize the message
        LizardsNroachestypes__GameMessage ack_msg;
        lizards_nroachestypes__game_message__init(&ack_msg);
        ack_msg.msg_type = LIZARDS_NROACHESTYPES__MESSAGE_TYPE__ACK;
        ack_msg.has_index = true;
        ack_msg.index = id_wasp;
        size_t len = lizards_nroachestypes__game_message__get_packed_size(&ack_msg);
        void *buf = malloc(len);
        lizards_nroachestypes__game_message__pack(&ack_msg, buf);
        zmq_send(socket, buf, len, 0);
        free(buf);
    }
}

void handleWaspMovement(LizardsNroachestypes__GameMessage *m, void *socket){
    int id_wasp = m->index;  // Identification of the wasp client
    int wasp = m->wasp_index;  // Index of the wasp within the WaspClient
    int stingOccurred = 0;  // Flag to indicate if a sting occurred

    pthread_mutex_lock(&sharedGameState);

    WaspClient *waspClient = findWaspClient(id_wasp);  // Localize the wasp client

    if(waspClient == NULL){
        pthread_mutex_unlock(&sharedGameState);

        forceWaspDisconnect(m, socket);  // If the wasp client is not found, disconnect it
    } else {        
        // Define the direction of the wasp
        stingOccurred = WaspStingsLizard(NULL, waspClient);

        waspClient->wasps[wasp].direction = m->direction;  // Update the direction of the wasp
        m->msg_type = LIZARDS_NROACHESTYPES__MESSAGE_TYPE__WASPS_MOVEMENT;
        
        // Verify if the next position is occupied by a lizard
        if(waspClient->wasps[wasp].on_board == 1 && stingOccurred == 0){
            pthread_mutex_lock(&roach_wasps_lock);
            renderWasp(waspClient, wasp);  // Render the wasp in the next position
            pthread_mutex_unlock(&roach_wasps_lock);
        }
        pthread_mutex_unlock(&sharedGameState);

        // Serialize the message
        LizardsNroachestypes__GameMessage ack_msg;  
        lizards_nroachestypes__game_message__init(&ack_msg);
        ack_msg.msg_type = LIZARDS_NROACHESTYPES__MESSAGE_TYPE__ACK;
        size_t len = lizards_nroachestypes__game_message__get_packed_size(&ack_msg);
        void *buf = malloc(len);
        lizards_nroachestypes__game_message__pack(&ack_msg, buf);
        zmq_send(socket, buf, len, 0);
        free(buf);        
    }
}

void disconnectAllWasps(){
    pthread_mutex_lock(&sharedGameState);
    WaspClient *currentWaspClient = gameState.headWaspList;
    pthread_mutex_lock(&roach_wasps_lock);
    while(currentWaspClient != NULL){
        for (int i = 0; i < currentWaspClient->num_wasps; i++)
        {
            cleanWasp(currentWaspClient, i);
        }
        currentWaspClient = currentWaspClient->next;
    }
    pthread_mutex_unlock(&roach_wasps_lock);
    freeWaspList();
    gameState.numWasps = 0;
    pthread_mutex_unlock(&sharedGameState);
}

void handleWaspDisconnect(LizardsNroachestypes__GameMessage *m, void *socket){
    int id_wasp = m->index;
    WaspClient *waspClient = findWaspClient(id_wasp);
    if(waspClient == NULL){
        forceWaspDisconnect(m, socket);
        return;
    } else {
        // Update the total number of wasps
        gameState.numWasps -= waspClient->num_wasps;
        LizardsNroachestypes__GameMessage ack_msg;
        lizards_nroachestypes__game_message__init(&ack_msg);
        ack_msg.msg_type = LIZARDS_NROACHESTYPES__MESSAGE_TYPE__ACK;
        size_t len = lizards_nroachestypes__game_message__get_packed_size(&ack_msg);
        void *buf = malloc(len);
        lizards_nroachestypes__game_message__pack(&ack_msg, buf);
        zmq_send(socket, buf, len, 0);
        free(buf);
        cleanWaspClient(waspClient);
        // Remove the wasp client from the list
        removeWaspClient(id_wasp);
    }
}

int checkPositionforWasp(position_t position){
    WaspClient *currentWaspClient = gameState.headWaspList;
    while(currentWaspClient != NULL){
        for(int i = 0; i < currentWaspClient->num_wasps; i++){
            if(comparePosition(currentWaspClient->wasps[i].position, position)){
                return 1;
            }
        }
        currentWaspClient = currentWaspClient->next;
    }
    return 0;
}