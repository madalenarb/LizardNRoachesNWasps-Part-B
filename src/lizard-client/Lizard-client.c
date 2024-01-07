#include "Lizard-funcs.h"

// Global Variables
WINDOW *input_win, *display_win;
volatile int score = 0;
volatile int exit_flag = 0; // Flag to indicate when to exit the program
volatile char ch;
void *context;

pthread_mutex_t lock;
pthread_mutex_t display_lock; // Mutex for display management

// Display Thread Function
void *display_thread_func(void *arg) {
    void *subscriber = zmq_socket(context, ZMQ_SUB);
    zmq_connect(subscriber, "tcp://localhost:5557");
    zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, "", 0);

    pthread_mutex_lock(&display_lock);
    box(display_win, 0, 0);
    wrefresh(display_win);
    pthread_mutex_unlock(&display_lock);
    while (!exit_flag) {
        pthread_mutex_lock(&lock);
        pthread_mutex_lock(&display_lock);
        updateDisplay(display_win, subscriber);
        pthread_mutex_unlock(&display_lock);
        pthread_mutex_unlock(&lock);
        usleep(100); // Adjust the sleep duration as needed
    }

    zmq_close(subscriber);
    pthread_mutex_lock(&display_lock);
    delwin(display_win);
    pthread_mutex_unlock(&display_lock);
    return NULL;
}

// Input Thread Function
void *input_thread_func(void *arg) {
    void *requester = zmq_socket(context, ZMQ_REQ);
    int rc =zmq_connect(requester, "tcp://localhost:5556");
    assert(rc == 0);

    char direction_name[4][10] = {"UP", "DOWN", "LEFT", "RIGHT"};

    message_t msg;
    int password =0;
    msg.msg_type = MSG_TYPE_LIZARD_CONNECT;

    msg.ch = ch;
    msg.direction = -1;
    zmq_send(requester, &msg, sizeof(msg), 0);
    message_t ack_msg, resp;
    ack_msg.msg_type = MSG_TYPE_ACK;
    pthread_mutex_lock(&lock);
    score = 0;
    pthread_mutex_unlock(&lock);
    zmq_recv(requester, &ack_msg, sizeof(ack_msg), 0);
    if(ack_msg.msg_type == MSG_TYPE_LIZARD_DISCONNECT){
        exit_flag = 1;
        return NULL;
    }
    password = ack_msg.password;

    int key;

    int loss;
    keypad(stdscr, TRUE);		/* We get F1, F2 etc..		*/

    while (!exit_flag) {
        msg.msg_type = MSG_TYPE_LIZARD_MOVEMENT;
        msg.password = password;
        pthread_mutex_lock(&lock);
        if(score < 0){
            loss = lizardsLoses(&msg);
        }
        pthread_mutex_unlock(&lock);
        
        key = getch();
        
        select_direction(key, &msg);

        zmq_send(requester, &msg, sizeof(msg), 0);      
        zmq_recv(requester, &ack_msg, sizeof(ack_msg), 0);

        if(key == 'q' || loss || ack_msg.msg_type == MSG_TYPE_LIZARD_DISCONNECT){
            exit_flag = 1;
            break;
        }
            
        score = ack_msg.score_lizard;  

        refresh();
        pthread_mutex_lock(&display_lock);
        wclear(input_win);
        mvprintw(0, 0, "You pressed %s     Score: %d   ", direction_name[msg.direction], score);
        wrefresh(input_win);    
        pthread_mutex_unlock(&display_lock);
    }

    // Send disconnect message
    msg.msg_type = MSG_TYPE_LIZARD_DISCONNECT;
    zmq_send(requester, &msg, sizeof(msg), 0);

    zmq_close(requester);
    return NULL;
}

// Main Function
int main() {

    do{
        printf("what is your character(a..z)?: ");
        ch = getchar();
        ch = tolower(ch);  
    } while (!isalpha(ch));

    // Initialize ncurses
    initscr();
    cbreak();
    noecho();
    curs_set(0); // Make the cursor invisible

    box(display_win, 0, 0);
    box(input_win, 0, 0);
    wrefresh(input_win);

    // Initialize windows
    display_win = newwin(WINDOW_HEIGHT, WINDOW_WIDTH, 3, 0); // Large window for display
    input_win = newwin(3, WINDOW_WIDTH, 0, 0); // Small window for input

    pthread_mutex_init(&lock, NULL);
    pthread_mutex_init(&display_lock, NULL); // Initialize display mutex
    // Initialize ZeroMQ context
    context = zmq_ctx_new();

    // Create threads
    pthread_t display_thread, input_thread;

    pthread_create(&display_thread, NULL, display_thread_func, display_win);
    pthread_create(&input_thread, NULL, input_thread_func, input_win);

    // Wait for threads to finish
    pthread_join(display_thread, NULL);
    pthread_join(input_thread, NULL);

    // Cleanup
    zmq_ctx_destroy(context);
    endwin();
    return 0;
}

