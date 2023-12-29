/**
 * @file wasp_funcs.c
 * @brief Implementation of functions related to WaspClient, including movement, connection handling, and disconnection.
 *
 * This file contains the definitions for functions responsible for handling WaspClientt connections, movements,
 * and disconnections in the context of the game. It also includes functions for updating Roach positions and rendering.
 */

#include "wasp_funcs.h"

void forceWaspDisconnect(message_t *m, void *socket){
    m->msg_type = MSG_TYPE_DISCONNECT;
    zmq_send(socket, &m, sizeof(*m), 0);
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

void handleWaspsConnect(WINDOW *my_win, WaspClient **headWaspList, message_t *m, void *socket, int *NroachesTotal, int id_wasp){
    m->index = id_wasp;  
    *NroachesTotal += m->N_wasps; //temos que mudar o nome 
    if(*NroachesTotal >= MAX_ROACHES){  // Alterar depois o nome do MAX
        forceWaspDisconnect(m, socket);  // Altere para forceWaspDisconnect
    } 
    else
    {
        m->msg_type = MSG_TYPE_ACK;
        addWaspClient(headWaspList, m->N_wasps, id_wasp);  // Altere para addWaspClient e ajuste os parâmetros
        WaspClient *waspClient = findWaspClient(headWaspList, id_wasp);  // Altere para findWaspClient

        // Adicione todas as vespas ao campo de jogo
        for (int i = 0; i < waspClient->num_wasps; i++)  // Altere para num_wasps
        {
            wmove(my_win, waspClient->wasps[i].position.position_x, waspClient->wasps[i].position.position_y);
            waddch(my_win, '#');  // Representa vespas com '#'
        }
        wrefresh(my_win);
        zmq_send(socket, m, sizeof(*m), 0);
    }
}

void handleWaspMovement(WINDOW *my_win, WaspClient **headWaspList, message_t *m, direction_t direction, void *socket, RoachClient **headRoachList, LizardClient *headLizardList){
    int id_wasp = m->index;  // Identification of the wasp client
    int wasp = m->wasp_index;  // Index of the wasp within the WaspClient
    int stingOccurred = 0;  // Flag to indicate if a sting occurred

    WaspClient *waspClient = findWaspClient(headWaspList, id_wasp);  // Localize the wasp client

    if(waspClient == NULL){
        forceWaspDisconnect(m, socket);  // If the wasp client is not found, disconnect it
    } else {        
        // Define the direction of the wasp
        stingOccurred = WaspStingsLizard(my_win, &headLizardList, NULL, headWaspList, waspClient);  // Check if the wasp stings a lizard

        waspClient->wasps[wasp].direction = direction;
        m->msg_type = MSG_TYPE_WASPS_MOVEMENT;  // Atualiza o tipo de mensagem para movimento de vespa
        
        // Verifica se a vespa está no tabuleiro antes de tentar movê-la
        if(waspClient->wasps[wasp].on_board == 1 && stingOccurred == 0){
            renderWasp(my_win, waspClient, wasp, headRoachList);  // Render the wasp on the game window
        }

        
        zmq_send(socket, m, sizeof(*m), 0);  // Envia a mensagem de movimento para o servidor
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