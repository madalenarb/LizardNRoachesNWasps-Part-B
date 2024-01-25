/**
 * @brief Handles signals, specifically the interrupt signal (SIGINT).
 * 
 * @param signum The signal number.
 */

#include "protocol.h"

volatile sig_atomic_t flag_exit = 0;

/**
 * Handles signals, specifically the interrupt signal (SIGINT).
*/
void handle_signal(int signum) {
    if (signum == SIGINT) {
        flag_exit = 1;
    }
}



