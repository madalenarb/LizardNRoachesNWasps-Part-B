/**
 * @file lizards_funcs.c
 * @brief Implementation of functions related to the movement and interaction of lizards in the game.
 *
 * This file contains the definitions of the functions responsible for calculating and updating positions,
 * tail orientations, movement handling, and lizard disconnection in the context of the game.
 */
#include "lizards_funcs.h"

void new_position(LizardClient* otherLizard){
    int i;
    switch (otherLizard->direction)
    {
    case UP:
        (otherLizard->position.position_x)--;
        for (i=0;i<5;i++) //a cauda fica na mesma direção da cabeça
        {
            otherLizard->cauda_y[i]=otherLizard->position.position_y;
        }
        if(otherLizard->position.position_x ==0){ //caso va contra a parede
            
            for(i=0;i<5;i++)
            {
                otherLizard->cauda_x[i]=i+2;
            }
            (otherLizard->position.position_x)++; //5 da cauda + 2
        }
        else if(otherLizard->position.position_x==WINDOW_WIDTH-2 || otherLizard->position.position_x==WINDOW_HEIGHT-3 || otherLizard->position.position_x==WINDOW_WIDTH-4 || otherLizard->position.position_x==WINDOW_WIDTH-5|| otherLizard->position.position_x==WINDOW_WIDTH-6) //para a cauda ficar dentro da janela
        {
            otherLizard->position.position_x = WINDOW_WIDTH-7;
            for (i=0;i<5;i++)
            {
                otherLizard->cauda_x[i]=WINDOW_WIDTH-2-i;
            }
        }
        
        else
        { 
            for (i=0;i<5;i++)
            {
                otherLizard->cauda_x[i]= (otherLizard->position.position_x)+i+1;
            }
        }
        break;

    case DOWN:
        (otherLizard->position.position_x) ++;
        for (i=0;i<5;i++) //a cauda fica na mesma direção da cabeça
        {
            otherLizard->cauda_y[i]=otherLizard->position.position_y;
        }

        if(otherLizard->position.position_x ==WINDOW_WIDTH-1) //caso va contra a parede
        {
            for(i=0;i<5;i++)
            {
                otherLizard->cauda_x[i]=WINDOW_WIDTH-i-3;
            }
            (otherLizard->position.position_x)--;
        }
        else if (otherLizard->position.position_x >= 1 && otherLizard->position.position_x <= 5)
        {
            otherLizard->position.position_x = 6;
            for (i=0;i<5;i++)
            {
                otherLizard->cauda_x[i]=i+1;
            }
        }
        else
        {
            for (i=0;i<5;i++)
            {
                otherLizard->cauda_x[i]= (otherLizard->position.position_x)-i-1;
            }
        }
        break;
    case LEFT:
        (otherLizard->position.position_y) --;
        for (i=0;i<5;i++) //a cauda fica na mesma direção da cabeça
        {
            otherLizard->cauda_x[i]=otherLizard->position.position_x;
        }
        if(otherLizard->position.position_y ==0) //caso va contra a parede
        {
            for(i=0;i<5;i++)
            {
                otherLizard->cauda_y[i]=i+2;
            }
            (otherLizard->position.position_y)++; //5 da cauda + 2
        }
        else if(otherLizard->position.position_y==WINDOW_HEIGHT-2 || otherLizard->position.position_y==WINDOW_HEIGHT-3 || otherLizard->position.position_y==WINDOW_HEIGHT-4 || otherLizard->position.position_y==WINDOW_HEIGHT-5 || otherLizard->position.position_y==WINDOW_HEIGHT-6 ) //para a cauda ficar dentro da janela
        {
            otherLizard->position.position_y = WINDOW_HEIGHT-7;
            for (i=0;i<5;i++)
            {
                otherLizard->cauda_y[i]=WINDOW_HEIGHT-2-i;
            }
        }
        else
        { 
            for (i=0;i<5;i++)
            {
                otherLizard->cauda_y[i]= (otherLizard->position.position_y)+i+1;
            }
        }
        break;

    case RIGHT:
        (otherLizard->position.position_y) ++;
        for (i=0;i<5;i++) //a cauda fica na mesma direção da cabeça
        {
            otherLizard->cauda_x[i]=otherLizard->position.position_x;
        }

        if(otherLizard->position.position_y ==WINDOW_HEIGHT-1) //caso va contra a parede
        {
            for(i=0;i<5;i++)
            {
                otherLizard->cauda_y[i]=WINDOW_HEIGHT-i-3;
            }
            (otherLizard->position.position_y) --;
        }
        else if (otherLizard->position.position_y==1 || otherLizard->position.position_y==2 || otherLizard->position.position_y==3 || otherLizard->position.position_y==4 || otherLizard->position.position_y==5) //para a cauda ficar dentro da janela
        {
            otherLizard->position.position_y = 6;
            for (i=0;i<5;i++)
            {
                otherLizard->cauda_y[i]=i+1;
            }
        }
        else
        {
            for (i=0;i<5;i++)
            {
                otherLizard->cauda_y[i]= (otherLizard->position.position_y)-i-1;
            }
        }
        break;

    default:
        break;
    }
}

void forceLizardDisconnect(message_t *m, void *socket){
    message_t disconnectMessage;
    disconnectMessage.msg_type = MSG_TYPE_DISCONNECT;
    zmq_recv(socket, m, sizeof(message_t), 0);
    zmq_send(socket, &disconnectMessage, sizeof(message_t), 0);
}

void lizardTailOrientation(LizardClient *currentClient){
    int flag = 0;
    for(int i = 0; i < 5; i++){
        position_t newTailPosition;

        switch(currentClient->direction){
            case UP:
                newTailPosition.position_x = currentClient->position.position_x + i + 1;
                newTailPosition.position_y = currentClient->position.position_y;
                break;

            case DOWN:
                newTailPosition.position_x = currentClient->position.position_x - i - 1;
                newTailPosition.position_y = currentClient->position.position_y;
                break;

            case LEFT:
                newTailPosition.position_y = currentClient->position.position_y + i + 1;
                newTailPosition.position_x = currentClient->position.position_x;
                break;

            case RIGHT:
                newTailPosition.position_y = currentClient->position.position_y - i - 1;
                newTailPosition.position_x = currentClient->position.position_x;
                break;

            default:
                break;
        }
        flag = isPositionValid(newTailPosition);
    }
    
    if(flag == 1){
        for(int i = 0; i < 5; i++){
            switch (currentClient->direction){
                case UP:
                    currentClient->cauda_x[i] = currentClient->position.position_x + i + 1;
                    currentClient->cauda_y[i] = currentClient->position.position_y;
                    break;
                case DOWN:
                    currentClient->cauda_x[i] = currentClient->position.position_x - i - 1;
                    currentClient->cauda_y[i] = currentClient->position.position_y;
                    break;
                case LEFT:
                    currentClient->cauda_y[i] = currentClient->position.position_y + i + 1;
                    currentClient->cauda_x[i] = currentClient->position.position_x;
                    break;
                case RIGHT:
                    currentClient->cauda_y[i] = currentClient->position.position_y - i - 1;
                    currentClient->cauda_x[i] = currentClient->position.position_x;
                    break;
                default:
                    break;
            }
        }
    }
}

void handleLizardConnect(WINDOW *my_win, LizardClient **headLizardList, message_t *m, void *socket, int *nClients){
    if(findLizardClient(*headLizardList, m->ch) != NULL || *nClients >= MAX_CLIENTS){
        forceLizardDisconnect(m, socket);
    } else {
        nClients++;
        m->msg_type = MSG_TYPE_ACK;
        m->score_lizard = 0;
        m->password = rand()+19999299;

        zmq_send(socket, m, sizeof(*m), 0);
        addLizardClient(headLizardList, m->ch, m->password);
        LizardClient *newLizard = findLizardClient(*headLizardList, m->ch);
        renderLizardhead(my_win, newLizard);    
        renderLizardTail(my_win, newLizard);
    }
}

void handleLizardMovement(WINDOW *my_win, LizardClient **headLizardList, RoachClient **headRoachList, message_t *m, void *socket){
    LizardClient *currentLizard = findLizardClient(*headLizardList, m->ch);
    if(m->password != currentLizard->password){
        forceLizardDisconnect(m, socket);
    } else {
        if(currentLizard != NULL){
            currentLizard->direction = m->direction;
            m->msg_type = MSG_TYPE_ACK;
            lizardHitsLizard(my_win, headLizardList, currentLizard);
            lizardEatsRoach(my_win, headRoachList, currentLizard);
            m->direction = currentLizard->direction;
            m->score_lizard = currentLizard->score;
            m->ch = currentLizard->id;
            // Before sending the message
            zmq_send(socket, m, sizeof(*m), 0);
        } else {
            forceLizardDisconnect(m, socket);
        }
    }
}

void handleLizardDisconnect(WINDOW *my_win, LizardClient **headLizardList, message_t *m, void *socket, int *nClients){
    LizardClient *otherLizard = findLizardClient(*headLizardList, m->ch);
    if(otherLizard != NULL){
        m->msg_type = MSG_TYPE_DISCONNECT;
        zmq_send(socket, &m, sizeof(*m), 0);
        cleanLizard(my_win, otherLizard);
        disconnectLizardClient(headLizardList, m->ch);
        nClients--;
    }
}

void disconnectAllLizards(LizardClient **headLizardList, void *socket) {
    LizardClient *currentLizard = *headLizardList;
    while (currentLizard != NULL) {
        
        // Send a message before disconnecting
        message_t m;
        zmq_recv(socket, &m, sizeof(m), 0);
        message_t disconnectMessage;
        disconnectMessage.msg_type = MSG_TYPE_DISCONNECT;
        disconnectMessage.ch = currentLizard->id;

        zmq_send(socket, &disconnectMessage, sizeof(message_t), 0);

        // Disconnect lizard
        // Clean and disconnect
        disconnectLizardClient(headLizardList, currentLizard->id);
        currentLizard = currentLizard->next;
    }

    freeList(headLizardList);
}

int checkPositionforLizard(LizardClient *headLizardList, position_t position){
    LizardClient *currentLizard = headLizardList;
    while(currentLizard != NULL){
        if(comparePosition(currentLizard->position, position)){
            return 1;
        }
        currentLizard = currentLizard->next;
    }
    return 0;
}