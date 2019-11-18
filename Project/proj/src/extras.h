#ifndef EXTRAS_H
#define EXTRAS_H

/** @defgroup extras extras
 * @{
 *
 */

#include "game.h" //game_info var
#include "highscores.h"
#include "bitmap.h"

/**
 * @brief Updates distance counter and the bitmap position related to the distance covered
 */
void distance_update_metros();
/**
 * @brief Draws the distance bmp
 */
void distance_draw();
/**
 * @brief Updates score counter and the bitmap position related to the score
 */
void score_update();
/**
 * @brief Draws the score bmp
 */
void score_draw();
/**
 * @brief Updates the bitmap position for the date in the main menu
 * If it is in the highscores menu updates bitmap positions according to
 * i(pos in array) and to type(which array(multi or single))
 *
 * @param i position in array
 * @param type selects array, between multi or single
 */
void num_date(int i, game_mode type);
/**
 * @brief Draws date in main menu or date recorded to each highscore in
 * the highscores menu
 */
void display_date();
/**
 * @brief Updates the bitmap position for the time in the main menu
 * If it is in the gameover menu updates bitmap positions to detail the
 * game time duration
 */
void num_time();
/**
 * @brief Draws time in main menu or time recorded to each game in
 * the gameover menu
 */
void display_time();
/**
 * @brief Updates the bitmap position for each highscores positions
 * @param i position in array
 * @param type selects array, between multi or single
 */
void num_highscore(int i, game_mode type);
/**
 * @brief Draws the highscore in the highscore menu
 */
void display_highscore();
/**
 * @brief Adds some details regarding play time, socre and distanced covered
 */
void gameover_extras();
/**
 * @brief Returns the bitmap corresponding to the number
 * @param num number to be converted
 */
Bitmap* bitmap_num_Dist(int num);//distance
/**
 * @brief Returns the bitmap corresponding to the number
 * @param num number to be converted
 */
Bitmap* bitmap_num_Score(int num);//score
/**
 * @brief Returns the bitmap corresponding to the number
 * @param num number to be converted
 */
Bitmap* bitmap_to_num(int num);
/**
 * @brief Deletes all bitmap used
 */
void delete_all_bitmap();

#endif
