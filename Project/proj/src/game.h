#ifndef GAME_H
#define GAME_H

/** @defgroup game game
 * @{
 *
 */

#include <lcom/lcf.h>
#include "state.h" //state machine variable and functions
#include "bitmap.h"
#include "highscores.h"
#include "objects.h"
#include "extras.h"
#include "vetor.h"
#include "video.h"//gt methods
#include "ibm_8042.h"//kbd and mouse macros

#define LASER_NUM 			3

typedef enum {
	LASERS, ROCKETS
} Obj;

/// Game information
typedef struct {
	uint8_t timer_irq, kbd_irq, rtc_irq, sp1_irq;
	uint32_t mouse_irq, int_counter;//interrupt counter
	uint8_t scan_code;//kbd scan_code (most recent interruption)
	struct packet pp;//most recent mouse packet
  //RTC time/date variables
  uint32_t seg, min, hour, day, month, year, final_int_counter;
	//Scores
	unsigned int dist_cover, score_counter, difficulty, coins_collected;
	bool snow, time_out;
	//Imagens utilizadas:
	//Distance digits related:
	Bitmap *metros_pos0, *metros_pos1, *metros_pos2, *metros_pos3, *metros_pos4, *metros_pos5 ; //digits for distance count
	Bitmap *metro, *one_dist, *two_dist, *three_dist, *four_dist, *five_dist, *six_dist ;//all numbers to be placed n position 0-5
	Bitmap *seven_dist, *eight_dist, *nine_dist, *zero_dist;
	//Score digits related:
	Bitmap *one, *two, *three, *four, *five, *six, *seven, *eight, *nine, *zero, *score ;//digits for score count
	Bitmap *score_pos0, *score_pos1, *score_pos2, *score_pos3, *score_pos4, *score_pos5 ;//postions for score count
  //DATE ,TIME, HIGHSCORE DIGITS AND POSITIONS
  Bitmap *one_num, *two_num, *three_num, *four_num, *five_num, *six_num, *seven_num, *eight_num, *nine_num, *zero_num ;
  Bitmap *time_0_pos, *time_1_pos, *time_2_pos, *time_3_pos, *time_4_pos, *time_5_pos, *traco, *time_sep ;
  Bitmap *date_0_pos, *date_1_pos, *date_2_pos, *date_3_pos, *date_4_pos, *date_5_pos;
  Bitmap *hscore_0_pos, *hscore_1_pos, *hscore_2_pos, *hscore_3_pos, *hscore_4_pos, *hscore_5_pos;
	//Others (intuitive)
	Bitmap *gameover, *background, *main_menu, *options_menu, *highscores_menu, *christmas_back; //backgrounds/menus
	Bitmap *bmp[3], *snowbmp;//3 laser types and snow bmp
	Bitmap *run1, *run2, *run3, *run4, *run5 ; // animated_sprites - CAN WE USE Bitmap * run[6] ?
	Bitmap *fly1, *fly2, *fly3, *fly4, *fly5 ;
  Bitmap *die1, *die2, *die3, *die4 ; // animated_sprites
	Bitmap *mouse, *rose, *coin, *rocket, *point;

} game_info;

/**
* @brief Subscribes to timer, keyboard, mouse and rtc interrupts and also
* Inicializes counter, bool statment and loads all bitmaps
*/
game_info* game_init(); //starts game
/**
* @brief Universal interrupt handler cycle that can only be exited after leaving game
* This function will deal with all interrupts from timer, keyboard, mouse and rtc
*/
void univesal_ih();
/**
* @brief Exits game by unsubscribing all interrupts of the timer, keyboard, mouse and rtc
* and deletes state machine and game_info struct
*/
void game_exit();
/**
* @brief Deals with mouse interrupts after being assemble to mouse packet
* in regard of, depending of it state menu, where it clicks and its movement
*/
void mouse_device();
/**
* @brief Deals with timer interrupts
* Draws all bitmap images (updates frame) after each interruption of the timer (60 fps)
* Also performs page flipping read time and date from rtc and check for collisions
*/
void time_device();
/**
* @brief Deals with keyboard interruptions in regard of, depending of it state menu, checks
* if esc is pressed or other key such as the space bar or the arrow keys to modify the player
* state
*/
void keyboard_device();
/**
* @brief Checks for a collision in 2 steps
* Firstly checks if the lasers intercept the player bmp (interception between 2 rectangles)
* After that, to avoid unnecessary calculations, performs a number of steps such as creating
* a third buffer in each will draw a rose background with each laser and then draws the new
* pos for the player and if in a given pixel non rose in the player bmp  before drawing it
* if there is a pixel non rose in that same pos for one laser then it detects a collisions
*
* @return 1 if a collision is detected and 0 otherwise
*/
int collision(Obj obj);

/**
* @brief Checks if the coins intercept the player bmp (interception between 2 rectangles)
* because the size of the coin bmp it is unnecessary to check pixel-by-pixel
*
* @return 1 if a collision is detected and 0 otherwise
*/
int collect_coins(vetor *coins);

/**
* @brief Reboots game and resets all counter and structs such as the state machine
* scores, and initializes player, laser and snow initial positions
* Before rebooting stores the score if it is in the top 5 stored scores
*/
void reboot_game();
/**
* @brief Checks what menu is selected in the main menu
*/
void main_menu_select();
/**
* @brief Checks what menu is selected in the options menu
*/
void options_menu_select();
/**
* @brief Checks what menu is selected in the gameover menu
*/
void gameover_menu_select();
/**
* @brief Checks what menu is selected in the highscores menu
*/
void highscores_menu_select();

/**
 * @brief Keyboard interrupt handler
 */
void (kbc_asm_ih) (void);

#endif
