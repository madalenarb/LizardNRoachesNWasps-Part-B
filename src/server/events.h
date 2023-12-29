/**
 * @file events.h
 * @brief Prototypes for functions handling game events in the LizardsNRoaches game.
 *
 * This header file declares functions responsible for handling various game events, such as interactions between lizards, and lizards eating roaches.
 */

#ifndef EVENTS_H
#define EVENTS_H

#include "roach_list.h"
#include "Lizard_list.h"
#include "render_funcs.h"
#include "utils.h"

/**
 * @brief Handles the event where a lizard encounters another lizard.
 *
 * @param my_win The window in which the game is being rendered.
 * @param headLizardList Pointer to the head of the linked list of lizard clients.
 * @param currentLizard Pointer to the current lizard client.
 */
void lizardHitsLizard(WINDOW *my_win, LizardClient **headLizardList, LizardClient *currentLizard);

/**
 * @brief Handles the event where a lizard eats a roach.
 *
 * @param my_win The window in which the game is being rendered.
 * @param headRoachList Pointer to the head of the linked list of roach clients.
 * @param currentLizard Pointer to the current lizard client.
 */
void lizardEatsRoach(WINDOW *my_win, RoachClient **headRoachList, LizardClient *currentLizard);

#endif // EVENTS_H
