/**
 * Timer module
 * 
 * @param initializeTimer This module is used to keep track of the time elapsed since the program started
 * @param updateTimer updates the timer
*/

#include "timer.h"

// Define the global variables
time_t start_time;       // Stores the start time of the program
double elapsed_time;     // Stores the elapsed time since the program started

void initializeTimer() {
    srand(time(NULL)); // Initialize random seed
    start_time = time(NULL); // Initialize start time
}

void updateTimer() {
    time_t current_time = time(NULL);
    elapsed_time = difftime(current_time, start_time); // Calculate elapsed time
}
