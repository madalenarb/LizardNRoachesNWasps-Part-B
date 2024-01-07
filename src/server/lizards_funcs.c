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
    disconnectMessage.msg_type = MSG_TYPE_LIZARD_DISCONNECT;
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

void handleLizardConnect(message_t *m, void *socket){
    pthread_mutex_lock(&lizard_lock);
    if(findLizardClient(m->ch) != NULL || gameState.numLizardsClients >= MAX_CLIENTS){
        forceLizardDisconnect(m, socket);
    } else {
        gameState.numLizardsClients++;
        m->msg_type = MSG_TYPE_ACK;
        m->score_lizard = 0;
        m->password = rand()+19999299;

        addLizardClient(m->ch, m->password);
        LizardClient *newLizard = findLizardClient(m->ch);
        renderLizardhead(newLizard);    
        renderLizardTail(newLizard);
    }
    pthread_mutex_unlock(&lizard_lock);
}

void handleLizardMovement(message_t *m, void *socket){
    pthread_mutex_lock(&lizard_lock);
    LizardClient *currentLizard = findLizardClient(m->ch);
    int flag = 0;
    int stingOccurred = 0;
    if(currentLizard == NULL){
        forceLizardDisconnect(m, socket);
    } else if(m->password != currentLizard->password){
        forceLizardDisconnect(m, socket);
    } else {
        if(currentLizard != NULL){
            currentLizard->direction = m->direction;
            m->msg_type = MSG_TYPE_ACK;
            stingOccurred = WaspStingsLizard(currentLizard, NULL);
            flag = lizardHitsLizard(currentLizard);
            if(flag == 0 && stingOccurred == 0){
                updateAndRenderOneLizard(currentLizard);
            }
            lizardEatsRoach(currentLizard);

            m->direction = currentLizard->direction;
            m->score_lizard = currentLizard->score;
            m->ch = currentLizard->id;
            // Before sending the message
            zmq_send(socket, m, sizeof(*m), 0);
        } else {
            forceLizardDisconnect(m, socket);
        }
    }
    pthread_mutex_unlock(&lizard_lock);
}


void handleLizardDisconnect(message_t *m, void *socket){
    pthread_mutex_lock(&lizard_lock);
    pthread_mutex_lock(&sharedGameState);
    m->msg_type = MSG_TYPE_LIZARD_DISCONNECT;
    LizardClient *otherLizard = findLizardClient(m->ch);
    if(otherLizard != NULL){
        m->msg_type = MSG_TYPE_LIZARD_DISCONNECT;
        zmq_send(socket, &m, sizeof(*m), 0);
        //zmq_send(socket, m, sizeof(*m), 0); //&m ou m?
        cleanLizard(gameState.my_win, otherLizard);
        disconnectLizardClient(m->ch);
        gameState.numLizardsClients--;
    }
    pthread_mutex_unlock(&sharedGameState);
    pthread_mutex_unlock(&lizard_lock);
} 

void disconnectAllLizards(void *socket) {
    pthread_mutex_lock(&sharedGameState);
    LizardClient *currentLizard = gameState.headLizardList;
    pthread_mutex_unlock(&sharedGameState);

    pthread_mutex_lock(&lizard_lock);
    while (currentLizard != NULL) {
        LizardClient *nextLizard = currentLizard->next;

        message_t disconnectMessage;
        disconnectMessage.msg_type = MSG_TYPE_LIZARD_DISCONNECT;
        disconnectMessage.ch = currentLizard->id;

        zmq_send(socket, &disconnectMessage, sizeof(message_t), 0);

        disconnectLizardClient(currentLizard->id);
        currentLizard = nextLizard;
    }
    freeList();
    pthread_mutex_unlock(&lizard_lock);
}

//isso tinha sido que nao tinha apagado um / mas depis
int checkPositionforLizard(position_t position){
    LizardClient *currentLizard = gameState.headLizardList;
    while(currentLizard != NULL){
        if(comparePosition(currentLizard->position, position)){
            return 1;
        }
        currentLizard = currentLizard->next;
    }
    return 0;
}
