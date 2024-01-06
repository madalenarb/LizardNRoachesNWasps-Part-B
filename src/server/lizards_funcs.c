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
    if(findLizardClient(m->ch) != NULL || gameState.numLizardsClients >= MAX_CLIENTS){
        pthread_mutex_lock(&lizard_lock);
        forceLizardDisconnect(m, socket);
        pthread_mutex_unlock(&lizard_lock);
    } else {
        pthread_mutex_lock(&sharedGameState);
        gameState.numLizardsClients++;
        pthread_mutex_unlock(&sharedGameState);
        m->msg_type = MSG_TYPE_ACK;
        m->score_lizard = 0;
        m->password = rand()+19999299;

        pthread_mutex_lock(&lizard_lock);
        pthread_mutex_lock(&sharedGameState);
        addLizardClient(m->ch, m->password);
        // LizardClient *newLizard = findLizardClient(m->ch);
        // renderLizardhead(my_win, newLizard);    
        // renderLizardTail(my_win, newLizard);
        pthread_mutex_unlock(&sharedGameState);
        pthread_mutex_unlock(&lizard_lock);
    }
}

void handleLizardMovement(message_t *m, void *socket){
    pthread_mutex_lock(&lizard_lock);/**/
    LizardClient *currentLizard = findLizardClient(m->ch);
    int flag = 0;
    int stingOccurred = 0;

    if(m->password != currentLizard->password){
        forceLizardDisconnect(m, socket);
    } else {
        printf("else\n");
        if(currentLizard != NULL){
            currentLizard->direction = m->direction;
            m->msg_type = MSG_TYPE_ACK;
            printf("currentLizard->direction: %d\n", currentLizard->direction);
            pthread_mutex_lock(&sharedGameState);
            stingOccurred = WaspStingsLizard(currentLizard, NULL);
            printf("stingOccurred: %d\n", stingOccurred);
            flag = lizardHitsLizard(currentLizard);
            printf("flag: %d\n", flag);
            if(flag == 0 && stingOccurred == 0){
                // updateAndRenderOneLizard(currentLizard);
                printf("Lizard %c moved to %d, %d\n", currentLizard->id, currentLizard->position.position_x, currentLizard->position.position_y);
                new_position(currentLizard);
            }
            lizardEatsRoach(currentLizard);
            pthread_mutex_unlock(&sharedGameState);

            m->direction = currentLizard->direction;
            m->score_lizard = currentLizard->score;
            m->ch = currentLizard->id;
            // Before sending the message
            zmq_send(socket, m, sizeof(*m), 0);
        } else {
            forceLizardDisconnect(m, socket);
        }
    }
    pthread_mutex_unlock(&lizard_lock); // **Unlock after all shared resources are safely modified
}

///*versão 2(mais segura porque verifica primeiro se currentLizard é NULL e, se for, desconecta o lagarto 
//          e retorna imediatamente. Isso evita qualquer desreferenciação de um ponteiro nulo.)*/

// void handleLizardMovement(message_t *m, void *socket){
//     pthread_mutex_lock(&lizard_lock);
//     LizardClient *currentLizard = findLizardClient(m->ch);


//     if(currentLizard == NULL){
//         // Handle case where lizard is not found
//         forceLizardDisconnect(m, socket);
//         pthread_mutex_unlock(&lizard_lock);
//         return;
//     }

//     if(m->password != currentLizard->password){
//         // Handle password mismatch
//         forceLizardDisconnect(m, socket);
//     } else {
//         // Valid lizard, process movement
//         int flag = 0;
//         int stingOccurred = 0;
//         currentLizard->direction = m->direction;
//         m->msg_type = MSG_TYPE_ACK;

//         pthread_mutex_lock(&sharedGameState);
//         stingOccurred = WaspStingsLizard(my_win, headLizardList, currentLizard, headWaspList, NULL);
//         flag = lizardHitsLizard(my_win, headLizardList, currentLizard);
//         if(flag == 0 && stingOccurred == 0){
//             updateAndRenderOneLizard(my_win, currentLizard);
//         }
//         lizardEatsRoach(my_win, headRoachList, currentLizard);
//         pthread_mutex_unlock(&sharedGameState);

//         m->direction = currentLizard->direction;
//         m->score_lizard = currentLizard->score;
//         m->ch = currentLizard->id;

//         // Send the message
//         zmq_send(socket, m, sizeof(*m), 0);
//     }
//     pthread_mutex_unlock(&lizard_lock);
// }


void handleLizardDisconnect(message_t *m, void *socket){
    printf("handleLizardDisconnect\n");
    pthread_mutex_lock(&lizard_lock);  // **//
    m->msg_type = MSG_TYPE_LIZARD_DISCONNECT;
    printf("handleLizar1111dDisconnect\n");
    LizardClient *otherLizard = findLizardClient(m->ch);
    if(otherLizard != NULL){
        m->msg_type = MSG_TYPE_LIZARD_DISCONNECT;
        zmq_send(socket, &m, sizeof(*m), 0);
        //zmq_send(socket, m, sizeof(*m), 0); //&m ou m?
        pthread_mutex_lock(&sharedGameState);
        cleanLizard(gameState.my_win, otherLizard);
        disconnectLizardClient(m->ch);
        gameState.numLizardsClients--;
        pthread_mutex_unlock(&sharedGameState);
    }
    pthread_mutex_unlock(&lizard_lock);/**/
} 

void disconnectAllLizards(void *socket) {
    pthread_mutex_lock(&lizard_lock); /**/
    LizardClient *currentLizard = gameState.headLizardList;
    while (currentLizard != NULL) {
        //nextLizard = currentLizard->next; // Salve o próximo lagarto antes de potencialmente liberar o atual(recomendam fazer isto)

        // Send a message before disconnecting
        message_t m;
        zmq_recv(socket, &m, sizeof(m), 0); //estas 2 linhas podem causar erros!!


        message_t disconnectMessage;
        disconnectMessage.msg_type = MSG_TYPE_LIZARD_DISCONNECT;
        disconnectMessage.ch = currentLizard->id;

        zmq_send(socket, &disconnectMessage, sizeof(message_t), 0);

        // Disconnect lizard
        // Clean and disconnect
        disconnectLizardClient(currentLizard->id);
        currentLizard = currentLizard->next;
    }

    freeList();

    pthread_mutex_unlock(&lizard_lock); 
}
//isso tinha sido que nao tinha apagado um / mas depis
int checkPositionforLizard(position_t position){
    LizardClient *currentLizard = gameState.headLizardList;
    while(currentLizard != NULL){
        if(comparePosition(currentLizard->position, position)){
            pthread_mutex_unlock(&lizard_lock);
            return 1;
        }
        currentLizard = currentLizard->next;
    }
    return 0;
}