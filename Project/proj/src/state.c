#include <stdint.h>
#include "state.h"
#include "game.h"

extern state_mach *machine;//from proj.c
extern game_info *info; //from proj.c

state_mach* machine_start() {
	//Aloca memoria
	machine = (state_mach*) malloc(sizeof(state_mach));
	machine->menu = MAIN_MENU;
	machine->movement = RUN_LOW;

	return machine;
}

void menu_input(event_game option) {

	switch (machine->menu) {

	case MAIN_MENU:
		if (option == PLAY)
			machine->menu = PLAY_GAME;
		else if (option == GO_OPTIONS)
			machine->menu = MENU_OPTIONS;
		else if (option == PLAY_2)
			machine->menu = MULTI_PLAYER;
		else if (option == SCORES)
			machine->menu = HIGHSCORES;
		else if (option == EXIT)
			machine->menu = COMPLETE;

	case PLAY_GAME:
		if (option == COLLISION)
			machine->menu = GAMEOVER;
		else if (option == ESC_PRESSED)//can press ESC to end game while playing
			machine->menu = GAMEOVER;

	case MENU_OPTIONS:
		if(option == ESC_PRESSED || option == BACK)//exit
			machine->menu = MAIN_MENU;

	case HIGHSCORES:
		if(option == ESC_PRESSED || option == BACK)//exit
			machine->menu = MAIN_MENU;

	case GAMEOVER:
		if (option == PLAY)
			machine->menu = PLAY_GAME;
	  else if (option == BACK)
		  machine->menu = MAIN_MENU;

		break;
	default:
		break;
	}

	return;
}

void jetpack_move(event_t evt) {

	switch (machine->movement) {

	case RUN_LOW:
		if (evt == UP)
			machine->movement = FLY_UP;
		else if (evt == DOWN)
			machine->movement = RUN_LOW;
		break;

	case FLY_UP:
		if (evt == DOWN)
			machine->movement = FLY_DOWN;
		else if(evt == UP)
			machine->movement = FLY_UP;
		break;

	case FLY_DOWN:
	 	if (evt == UP)
			machine->movement = FLY_UP;
		else if(evt == DOWN)
			machine->movement = FLY_DOWN;
		break;

	default:
		break;
	}
	return;
}

void delete_machine() {
	free(machine);
	return;
}
