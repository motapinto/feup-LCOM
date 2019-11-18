#ifndef STATE_H
#define STATE_H

/** @defgroup state state
 * @{
 *
 */

//MENUS
typedef enum {
	MAIN_MENU, PLAY_GAME, MENU_OPTIONS, GAMEOVER, MULTI_PLAYER, HIGHSCORES, COMPLETE
} state_menus;

//Inputs in  state machine to transition between menus
typedef enum {
	PLAY, GO_OPTIONS, PLAY_2, EXIT, SCORES, //main menu
	COLLISION, ESC_PRESSED, BACK
} event_game;

//Character movement types
typedef enum {
	RUN_LOW, FLY_UP, FLY_DOWN
} state_t;

//Transition in chacter movement transition
typedef enum {
	UP, DOWN
} event_t;

/// Represents the current states in the two sub state machines
typedef struct {
	state_menus menu;
	state_t movement;
} state_mach;

/**
* @brief Starts the state_mach structure with the initial values ​​and allocates memory
* required with the size of this variable, since there is only one variable of this type,
* I decided to use the singleton technique to be accessible in any function and to not
* have to go through all the functions as an argument
*/
state_mach* machine_start();
/**
* @brief Receives an event_game that causes the state machine to change states
* between the access menus
*
* @param event_game event to be received as input to change state in machine
*/
void menu_input(event_game option);
/**
* @brief Receives an event_t that causes the state machine to change states
* between the player movement options
*
* @param event_t event to be received as input to change state in machine
*/
void jetpack_move(event_t evt);
/**
* @brief deletes state machine and frees all alocated memory
*/
void delete_machine();

#endif
