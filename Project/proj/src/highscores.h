#ifndef HIGHSCORES_H
#define HIGHSCORES_H

/** @defgroup highscores highscores
 * @{
 *
 */

#include <stdint.h>
#include <string.h>

#define MAX_SCORES 5

typedef enum {
	ONE_PLAY, TWO_PLAY
} game_mode;

typedef struct {
  uint32_t score;
  uint8_t day, month, year;
} highscore ;

static char file_path_singleplayer[] = "/home/lcom/labs/proj/doc/singleplayer_highscores.txt" ;
static char file_path_multiplayer[] = "/home/lcom/labs/proj/doc/multiplayer_highscores.txt" ;
highscore single [MAX_SCORES] ;
highscore multi [MAX_SCORES] ;

/**
* @brief Inicializes highscore single and multi structs
*/
void highscore_init();
/**
* @brief Reads the highscores from singleplayer_highscores.txt or
* multiplayer_highscores.txt depending on the game_type
*
* @param game_type ONE_PLAY if it´s to read the singleplayer highscores
* and TWO_PLAY if it´s to read the multiplayer highscores
*/
void highscore_read(game_mode game_type);
/**
* @brief Writes the highscores to singleplayer_highscores.txt or
* multiplayer_highscores.txt depending on the game_type
*
* @param game_type ONE_PLAY if it´s to write to the singleplayer highscores
* and TWO_PLAY if it´s to write to the multiplayer highscores
*/
void highscore_write(game_mode game_type);
/**
* @brief Orders the read highscores in single or multi struct variables
*
* @param game_type ONE_PLAY if it´s to order the singleplayer highscores struct
* and TWO_PLAY if it´s to order the multiplayer highscores struct
*/
void highscore_order(game_mode game_type);
/**
* @brief After each game adds the highscore in the last position if it's score is
* nor greater than the one in the last position (after being ordered)
*
* @param new_score new score to be added
* @param new_day day of the new score
* @param new_month month of the new score
* @param new_year year of the new score
* @param game_type ONE_PLAY if it´s to add to the singleplayer highscores struct
* and TWO_PLAY if it´s to add to the multiplayer highscores struct
*/
void highscore_add(uint32_t new_score, uint8_t new_day, uint8_t new_month, uint8_t new_year, game_mode game_type);
/**
* @brief Eliminates all highscores and resets its  value to 0
*
* @param game_type ONE_PLAY if it´s to clean the singleplayer highscores
* and TWO_PLAY if it´s to clean the multiplayer highscores
*/
void highscores_cleanup(game_mode game_type);

#endif
