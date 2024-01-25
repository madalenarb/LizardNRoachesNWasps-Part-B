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
 * @param currentLizard Pointer to the current lizard client.
 */
int lizardHitsLizard(LizardClient *currentLizard);

/**
 * @brief Handles the event where a lizard eats a roach.
 *
 * @param currentLizard Pointer to the current lizard client.
 */
void lizardEatsRoach(LizardClient *currentLizard);

/**
 * @brief Handles the event where a wasp stings a lizard.
 *
 * @param currentLizard Pointer to the current lizard client.
 * @param currentWasp Pointer to the current wasp client.
 */
int WaspStingsLizard(LizardClient *currentLizard, WaspClient *currentWasp);

#endif // EVENTS_H
