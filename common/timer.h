/**
 * Timer module
 * 
 * @brief This module is used to keep track of the time elapsed since the program started
*/


#ifndef TIMER_H
#define TIMER_H

#include <time.h>
#include <stdlib.h>


// Declare the global variables as extern
extern time_t start_time;
extern double elapsed_time; // Should be double to store time difference

void initializeTimer();
void updateTimer();

#endif /* TIMER_H */
