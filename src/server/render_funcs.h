/**
 * @file render_funcs.h
 * @brief Prototypes for rendering functions in the LizardsNRoaches game.
 *
 * Declares functions for rendering lizards and roaches, setting up game windows, and updating 
 * game elements' visuals. These functions are key to managing the game's graphical representation.
 */

#ifndef RENDER_FUNCS_H
#define RENDER_FUNCS_H

#include "../../common/protocol.h"
#include "../../common/constants.h"

#include "roach_list.h"
#include "display.h"
#include "wasp_list.h"
#include "Lizard_list.h"
#include "lizards_funcs.h"
#include "utils.h"
#include "roach_funcs.h"
#include "wasp_funcs.h"

/**
 * @brief Initializes the game window using ncurses.
 *
 * Sets up the main game window, initializing the curses mode and configuring necessary settings for display.
 *
 * @param my_win Pointer to the window structure to be initialized.
 */
void setupWindows();

/**
 * @brief Updates and renders the heads of all lizard clients.
 *
 * Iterates through the list of lizard clients and renders the head of each lizard on the game window.
 *
 * @param my_win Pointer to the game window.
 * @param headLizardList Pointer to the head of the linked list of lizard clients.
 */
void updateAndRenderLizardsHeads();

/**
 * @brief Updates and renders the tails of all lizard clients.
 *
 * Iterates through the list of lizard clients and renders the tail of each lizard on the game window.
 *
 * @param my_win Pointer to the game window.
 * @param headLizardList Pointer to the head of the linked list of lizard clients.
 */
void updateAndRenderLizardsTails();

/**
 * @brief Updates and renders a single lizard.
 *
 * Cleans the previous position of the lizard on the window, updates the position, and then renders the lizard.
 *
 * @param my_win Pointer to the game window.
 * @param otherLizard Pointer to the LizardClient structure to be updated and rendered.
 */
void updateAndRenderOneLizard(LizardClient *otherLizard);

/**
 * @brief Checks if a position is occupied by a lizard.
 *
 * Compares a given position with the positions of all lizards to determine if it's already occupied.
 *
 * @param headLizardList Pointer to the head of the linked list of lizard clients.
 * @param position The position to be checked.
 * @return 1 if occupied, 0 otherwise.
 */
int checkPositionforLizard(position_t position);

/**
 * @brief Renders a roach on the game window.
 *
 * Displays a roach client at its current position on the game window.
 *
 * @param my_win Pointer to the game window.
 * @param roachClient Pointer to the RoachClient structure.
 * @param id The ID of the roach to be rendered.
 * @param headLizardList Pointer to the head of the linked list of lizard clients.
 */
void renderRoach(int id, RoachClient *roachClient);

/**
 * @brief Updates and renders all roaches on the game window.
 *
 * Iterates through the list of roach clients and renders each roach on the game window.
 *
 * @param my_win Pointer to the game window.
 * @param headRoachList Pointer to the head of the linked list of roach clients.
 */
void updateAndRenderRoaches();

/**
 * @brief Cleans the previous position of a lizard on the game window.
 *
 * Erases the lizard and its tail from its previous position on the game window.
 *
 * @param my_win Pointer to the game window.
 * @param otherLizard Pointer to the LizardClient structure to be cleaned.
 */
void cleanLizard(WINDOW *my_win, LizardClient *otherLizard);

/**
 * @brief Renders the head of a lizard client.
 *
 * Displays the head of a lizard client at its current position on the game window.
 *
 * @param my_win Pointer to the game window.
 * @param otherLizard Pointer to the LizardClient structure.
 */
void renderLizardhead(LizardClient *otherLizard);

/**
 * @brief Renders the tail of a lizard client.
 *
 * Displays the tail of a lizard client at its current position on the game window.
 *
 * @param my_win Pointer to the game window.
 * @param otherLizard Pointer to the LizardClient structure.
 */
void renderLizardTail(LizardClient *otherLizard);


/**
 * @brief Handles interactions when a lizard eats a roach.
 *
 * Manages the scenario where a lizard consumes a roach, adjusting scores and updating game state.
 *
 * @param my_win Pointer to the game window.
 * @param headRoachList Pointer to the head of the linked list of roach clients.
 * @param currentLizard Pointer to the LizardClient structure that eats the roach.
 */
void lizardEatsRoach(LizardClient *currentLizard);

/**
 * @brief Updates the visibility status of roaches.
 *
 * Iterates through all roach clients and updates their visibility on the game window based on certain conditions.
 *
 * @param headRoachList Pointer to the head of the linked list of roach clients.
 * @param id_roach The ID of the roach client to update visibility for.
 */
void updateRoachesVisibility();


/**
 * @brief Renders a roach on the game window.
 *
 * Displays a wasp client at its current position on the game window.
 *
 * @param my_win Pointer to the game window.
 * @param roachClient Pointer to the WaspClient structure.
 * @param id The ID of the wasp to be rendered.
 * @param headLizardList Pointer to the head of the linked list of lizard clients.
 */
void renderWasp(WaspClient *waspClient, int waspIndex);

/**
 * @brief Updates and renders all wasps on the game window.
 *
 * Iterates through the list of roach clients and renders each roach on the game window.
 *
 * @param my_win Pointer to the game window.
 * @param headWaspsList Pointer to the head of the linked list of wasp clients.
 * @param headLizardList Pointer to the head of the linked list of lizard clients.
 */
void updateAndRenderWasps();

/**
 * @brief renders roaches of a roach client
 * 
 * Displays all roaches of a roach client at their current position on the game window.
 * 
 * @param my_win Pointer to the game window.
 * @param roachClient Pointer to the RoachClient structure.
*/
void renderRoaches(RoachClient *roachClient);

void updateAndRenderEverything();

#endif // RENDER_FUNCS_H
