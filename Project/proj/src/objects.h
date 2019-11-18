#ifndef OBJECTS_H
#define OBJECTS_H

/** @defgroup objects objects
 * @{
 *
 */

#include "vetor.h"
#include "game.h"
#include "video.h" //video graphics functions and declared get methods
#include "ibm_8042.h"//kbd and mouse macros

//Structs of all objects
typedef struct {
    int x, y, width, height;
} Mouse;

typedef struct {
    int x, y, y0, y02, width, height, fall_counter;
    double speed_y, ac;
    double time, time2;
    Bitmap *bmp;
} Charac;
//Each laser struct must have bitmap var as there may be the possibility of multiples diferent type lasers at one determined frame
typedef struct {
    int x, y, width, height, x_med, y_med;
    Bitmap *bmp;
    int type;
    bool draw, empty;
} Laser;

typedef struct {
    int x, y, width, height;
    Bitmap *bmp;
    bool draw, empty;
} Snow;

typedef struct {
    int x, y, width, height;
    bool draw;
} Rocket;

//Global variables that contain more accesible info
Mouse mouse;
Charac player, player2;
Laser laser[3]; //maximum of 3 lasers at one frame
Snow snow[20]; //maximum of 20 snow at one frame
Rocket rocket;

/**
* @brief Updates mouse positions with deltax deltay form mouse packet,
* to mouse global struct variable
*/
void mouse_update();
/**
* @brief Inicializes mouse struct values, with a inicial position
*/
void mouse_init();
/**
* @brief Inicializes player struct values, with a inicial position, movement, ...
*/
void player_init();
/**
* @brief With a global counter run_counter switches betwen the 5 jetpack run bitmap
* if and only if the movment is RUN_LOW
*/
void jetpack_run();
/**
* @brief With a global counter fly_counter switches betwen the 5 jetpack fly bitmap
* if and only if the movment is FLY_UP or FLY_DOWN, also to increase the realism of
* this movement itwas implemented some physics of free fall equations of physics to
* update the coordinate in y-axis in player struct
*/
void jetpack_fly();
/**
* @brief Draws player bitmap according to its movement and image type counter
*/
void player_draw();
/**
* @brief With a global counter die_counter switches betwen the 4 jetpack dead bitmap
* if and only if it detects a collision
*/
void player_die();
/**
* @brief Inicializes laser struct values, with a inicial position, and bool values
*/
void laser_init();
/**
* @brief Randomizes with rand() function if the bool empty is true at which laser.y
* it will be regerated considering the y bondaries and also randomizes laser type
* between horizontal, diagonal or vertical and also inicializes same bool statements
*/
void lasers_randomise();
/**
* @brief Because of the limit of lasers that we decided to put on a certain frame (3)
* it was decided to use a static and not dynamic array to randomise the lasers.
* Inicially because there is no lasers it draws 3 lasers (the maximum) and after that
* after each laser.x+laser.width is less than 0 updates the flag draw to false and empty to true
* Also updates the x value of laser.x according to the difficulty of the game
*/
void laser_update_pos();
/**
* @brief Draws all lasers bitmap according to its type selected inicially in laser_randomize()
*/
void laser_draw();
/**
* @brief Inicializes snow struct values, with a inicial position, bool values, and bmp
*/
void snow_init();
/**
* @brief Randomizes with rand() function if the bool empty is true at which snow.x
* it will be regerated considering the x bondaries and also inicializes same bool statements
*/
void snow_randomise();
/**
* @brief Because of the limit of snow that we decided to put on a certain frame (20)
* it was decided to use a static and not dynamic array to randomise the lasers.
* Inicially because there is no snow it draws 20 "snow" (the maximum) and after that
* after each snow.x+snow.width is less than 0 updates the flag draw to false and empty to true
* Also updates the x value of snow.x according to the difficulty of the game
*/
void snow_update_pos();
/**
* @brief Draws all snow bitmap
*/
void snow_draw();
/**
* @brief Inicializes coins dynamic vector struct values, with a inicial position, width, height, bmp, ...
*/
vetor* coins_init();
/**
* @brief Randomizes with rand() function the width and height of the coins matrix that will be generated
* it will be regerated considering the x bondaries and also inicializes same bool statements
*/
void coins_randomise(vetor *vec);
/**
* @brief Because of the high number of coins
* it was decided to use a dynamic array to randomise the coins.
* Updates the x value of all coins x coordinate according to the difficulty of the game
*/
void coins_update_pos(vetor *vec);
/**
* @brief Draws all coins bitmaps in vetor
*/
void coins_draw(vetor *vec);
/**
* @brief Inicializes rocket struct values, with a inicial position, bool values, and bmp
*/
void rocket_init();
/**
* @brief Randomizes with rand() function if the bool empty is true at which rocket.y
* it will be regerated considering the x bondaries and also inicializes same bool statements
*/
void rocket_randomise();
/**
* @brief Because of the limit of rocket that we decided to put on a certain frame (1)
* it was decided to use a static and not dynamic array to randomise the rockets.
* Updates the x value of rocket.x according to the difficulty of the game
*/
void rocket_update_pos();
/**
* @brief Draws the rocket bitmap
*/
void rocket_draw();

#endif
