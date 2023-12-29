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



void handleWaspMovement(WINDOW *my_win, WaspClient **headWaspList, message_t *m, direction_t direction, void *socket, LizardClient *headLizardList) {
    int id_wasp = m->index;  // Identificação do cliente vespa
    int wasp = m->wasp_index;  // Index da vespa específica

    WaspClient *waspClient = findWaspClient(headWaspList, id_wasp);  // Localiza o cliente vespa na lista

    if(waspClient == NULL){
        forceWaspDisconnect(m, socket);  // Se o cliente não for encontrado, desconecta
    } else {
        // Define a direção do movimento da vespa
        waspClient->wasps[wasp].direction = direction;
        m->msg_type = MSG_TYPE_WASPS_MOVEMENT;  // Atualiza o tipo de mensagem para movimento de vespa
        
        // Verifica se a vespa está no tabuleiro antes de tentar movê-la
        if(waspClient->wasps[wasp].on_board == 1){
            renderWasp(my_win, waspClient, wasp, headLizardList);  // Renderiza o movimento da vespa
        }
        
        zmq_send(socket, m, sizeof(*m), 0);  // Envia a mensagem de movimento para o servidor
    }
}




