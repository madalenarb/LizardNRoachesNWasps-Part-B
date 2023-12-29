#include "display-app.h"

position_t* DeriveTailPosition(position_t head_position, direction_t direction){
    static position_t tail_position[TAIL_LENGTH];
    for(int i = 0; i < TAIL_LENGTH; i++){
        switch(direction){
            case UP:
                tail_position[i].position_x = head_position.position_x + i + 1;
                tail_position[i].position_y = head_position.position_y;
                break;
            case DOWN:
                tail_position[i].position_x = head_position.position_x - i - 1;
                tail_position[i].position_y = head_position.position_y;
                break;
            case LEFT:
                tail_position[i].position_x = head_position.position_x;
                tail_position[i].position_y = head_position.position_y + i + 1;
                break;
            case RIGHT:
                tail_position[i].position_x = head_position.position_x;
                tail_position[i].position_y = head_position.position_y - i - 1;
                break;
        }
    }
    return tail_position;
}

void renderLizard(WINDOW *display_win, display_update_t game_update){
    mvwaddch(display_win, game_update.position.position_x, game_update.position.position_y, game_update.ch);
    //Draw the tail
    position_t* tailPositions = DeriveTailPosition(game_update.position, game_update.direction);
    for(int i = 0; i < TAIL_LENGTH; i++){
        mvwaddch(display_win, tailPositions[i].position_x, tailPositions[i].position_y, (game_update.score < 50) ? '.' : '*');
    }
}

void renderRoach(WINDOW *display_win, display_update_t game_update){
    for(int i = 0; i < game_update.roaches_num; i++){
        if(game_update.on_board[i] == 1)
            mvwaddch(display_win, game_update.roach_positions[i].position_x, game_update.roach_positions[i].position_y, '0' + game_update.roach_score[i]);
    }
}

void updateDisplay(WINDOW *display_win, void *subscriber){
    int totalEntities = 0;
    zmq_recv(subscriber, &totalEntities, sizeof(int), 0);

    display_update_t *gameState = (display_update_t *)malloc(sizeof(display_update_t) * totalEntities);
    zmq_recv(subscriber, gameState, sizeof(display_update_t) * totalEntities, 0);

    //Clear the window
    werase(display_win);
    box(display_win, 0, 0);

    //Render the entities
    for(int i = 0; i < totalEntities; i++){
        if(gameState[i].entity_type == LIZARD){
            renderLizard(display_win, gameState[i]);
        } else if(gameState[i].entity_type == ROACH){
            renderRoach(display_win, gameState[i]);
        }
    }
    wrefresh(display_win);

    free(gameState);
}

int main()
{
    void *context = zmq_ctx_new();
    void *subscriber = zmq_socket(context, ZMQ_SUB);
    zmq_connect(subscriber, "tcp://localhost:5556");
    zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, "", 0);

    initscr();
    cbreak();
    noecho();

    //Set up the window to draw the game in
    WINDOW *display_win = newwin(WINDOW_HEIGHT, WINDOW_WIDTH, 0, 0);
    box(display_win, 0, 0);
    wrefresh(display_win);

    while(1){
        updateDisplay(display_win, subscriber);
    }
    return 0;
}

