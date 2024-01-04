/**
 * @file wasp_funcs.c
 * @brief Implementation of functions related to WaspClient, including movement, connection handling, and disconnection.
 *
 * This file contains the definitions for functions responsible for handling WaspClientt connections, movements,
 * and disconnections in the context of the game. It also includes functions for updating Roach positions and rendering.
 */

#include "wasp_funcs.h"

void forceWaspDisconnect(LizardsNroachestypes__GameMessage *m, void *socket){
    m->msg_type = MSG_TYPE_WASPS_DISCONNECT;
    size_t len = lizards_nroachestypes__game_message__pack(m, NULL);
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

void cleanWasp(WINDOW *my_win, WaspClient *waspClient, int id){
    wmove(my_win, waspClient->wasps[id].position.position_x, waspClient->wasps[id].position.position_y);
    waddch(my_win, ' ');
    wrefresh(my_win);
}

void cleanWaspClient(WINDOW *my_win, WaspClient *waspClient){
    for (int i = 0; i < waspClient->num_wasps; i++)
    {
        cleanWasp(my_win, waspClient, i);
    }
}

void handleWaspsConnect(WINDOW *my_win, WaspClient **headWaspList, LizardsNroachestypes__GameMessage *m, void *socket, int *NWaspsTotal, int id_wasp){
    m->index = id_wasp; 
    *NWaspsTotal += m->n_wasps;  // Update the total number of wasps
    if(*NWaspsTotal > MAX_WASPS){
        forceWaspDisconnect(m, socket);  // If the total number of wasps exceeds the maximum, disconnect the wasp client
    } else {
        m->msg_type = LIZARDS_NROACHESTYPES__MESSAGE_TYPE__ACK;
        addWaspClient(headWaspList, m->n_wasps, id_wasp);  // Altere para addWaspClient e ajuste os parâmetros
        WaspClient *waspClient = findWaspClient(headWaspList, id_wasp);  // Altere para findWaspClient

        // Adicione todas as vespas ao campo de jogo
        for (int i = 0; i < waspClient->num_wasps; i++)  // Altere para num_wasps
        {
            wmove(my_win, waspClient->wasps[i].position.position_x, waspClient->wasps[i].position.position_y);
            waddch(my_win, '#');  // Representa vespas com '#'
        }
        wrefresh(my_win);

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

void handleWaspMovement(WINDOW *my_win, WaspClient **headWaspList, LizardsNroachestypes__GameMessage *m, void *socket, RoachClient **headRoachList, LizardClient *headLizardList){
    int id_wasp = m->index;  // Identification of the wasp client
    int wasp = m->wasp_index;  // Index of the wasp within the WaspClient
    int stingOccurred = 0;  // Flag to indicate if a sting occurred

    WaspClient *waspClient = findWaspClient(headWaspList, id_wasp);  // Localize the wasp client

    if(waspClient == NULL){
        forceWaspDisconnect(m, socket);  // If the wasp client is not found, disconnect it
    } else {        
        // Define the direction of the wasp
        stingOccurred = WaspStingsLizard(my_win, &headLizardList, NULL, headWaspList, waspClient);

        waspClient->wasps[wasp].direction = m->direction;  // Update the direction of the wasp
        m->msg_type = MSG_TYPE_WASPS_MOVEMENT;  // Update the message type
        
        // Verify if the next position is occupied by a lizard
        if(waspClient->wasps[wasp].on_board == 1 && stingOccurred == 0){
            renderWasp(my_win, waspClient, wasp, headRoachList, headLizardList);  // Render the wasp in the next position
        }

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

void disconnectAllWasps(WINDOW *my_win, WaspClient **headWaspList, int *NwaspsTotal){
    WaspClient *currentWaspClient = *headWaspList;
    while(currentWaspClient != NULL){
        for (int i = 0; i < currentWaspClient->num_wasps; i++)
        {
            cleanWasp(my_win, currentWaspClient, i);
        }
        currentWaspClient = currentWaspClient->next;
    }
    freeWaspList(headWaspList);
    *NwaspsTotal = 0;
}

void handleWaspDisconnect(WINDOW *my_win, WaspClient **headWaspList, LizardsNroachestypes__GameMessage *m, void *socket, int *NwaspsTotal){
    int id_wasp = m->index;
    WaspClient *waspClient = findWaspClient(headWaspList, id_wasp);
    if(waspClient == NULL){
        forceWaspDisconnect(m, socket);
        return;
    } else {
        // Update the total number of wasps
        *NwaspsTotal -= waspClient->num_wasps;
        LizardsNroachestypes__GameMessage ack_msg;
        lizards_nroachestypes__game_message__init(&ack_msg);
        ack_msg.msg_type = LIZARDS_NROACHESTYPES__MESSAGE_TYPE__ACK;
        
        size_t len = lizards_nroachestypes__game_message__get_packed_size(&ack_msg);
        void *buf = malloc(len);
        lizards_nroachestypes__game_message__pack(&ack_msg, buf);
        zmq_send(socket, buf, len, 0);
        free(buf);
        cleanWaspClient(my_win, waspClient);
        // Remove the wasp client from the list
        removeWaspClient(headWaspList, id_wasp);
    }
}

int checkPositionforWasp(WaspClient *headWaspList, position_t position){
    WaspClient *currentWaspClient = headWaspList;
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