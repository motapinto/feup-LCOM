#include <stdint.h>
#include <math.h>
#include "objects.h"

extern int packet_store_counter; //from mouse.c
extern game_info *info; //from proj.c
extern state_mach *machine;//from proj.c
extern struct packet pp;//from mouse.c

extern int run_counter; //from game.c
extern int fly_counter;
extern int die_counter;
extern bool lost;

void mouse_update()
{
  uint16_t hres = get_hres();
  uint16_t vres = get_vres();

  mouse.x +=  info->pp.delta_x;
  mouse.y -=  info->pp.delta_y;

  if(mouse.x < 0) mouse.x = 0;
  if(mouse.y < 0) mouse.y = 0;
  if(mouse.x > (hres-mouse.width))  mouse.x = hres-mouse.width;
  if(mouse.y > (vres-mouse.height)) mouse.y = vres-mouse.height;
}

void mouse_init()
{
  mouse.x = 0;
  mouse.y = 0;
  mouse.width  = info->mouse->bitmapInfoHeader.width;
  mouse.height = info->mouse->bitmapInfoHeader.height;
}

void player_init()
{
  player.x   =  55;
  player.y   =  620;
  player.time2 =  0;
  player.fall_counter = 0;
  player.bmp = info->run5;
  player.width  = info->run5->bitmapInfoHeader.width;
  player.height = info->run5->bitmapInfoHeader.height;
  player.speed_y = 0;
  player.time = 0;
  player.ac = -9.8; //gravity in earth
}

void jetpack_run()
{
  if(machine->movement == RUN_LOW){
    if(run_counter == 0){
      player.bmp = info->run5;
    }
    else if(run_counter == 1){
      player.bmp = info->run4;
    }
    else if(run_counter == 2){
      player.bmp = info->run3;
    }
    else if(run_counter == 3){
      player.bmp = info->run2;
    }
    else if(run_counter == 4){
      player.bmp = info->run1;
      run_counter = 0;
    }
  }
}

void jetpack_fly()
{
  int y = 6;

  if(machine->movement == FLY_UP){
    player.time = 0;
    player.y -= y; //sobe
    player.y0 = abs(player.y-620); // y0 is last y in fly up -> formula is because of referencial

    if(fly_counter == 0) player.bmp = info->fly4 ;
    else if(fly_counter == 1) player.bmp = info->fly3 ;
    else if(fly_counter == 2) player.bmp = info->fly2 ;
    else if(fly_counter == 3) player.bmp = info->fly1 ;

    if(fly_counter > 3) fly_counter = 0;
  }

  if(machine->movement == FLY_DOWN){
    player.time += 12/60.0;
    player.y = player.y0 -5*pow(player.time, 2); // y = y0 - 5*t^2
    player.y = abs(player.y-620);
    player.bmp = info->fly5;
    fly_counter = 0;
  }
  if(player.y <= 100) {
    player.y = 100;
  }
  if(player.y >= 620) {
    player.y = 620; //colision with ground
    machine->movement = RUN_LOW;//person type of movement change
  }
}

void player_draw()
{
  drawBitmap(player.bmp, player.x, player.y, ALIGN_LEFT);
}

void player_die()
{
  die_counter++;
  if(die_counter >= 3)      player.bmp = info->die4 ;
  else if(die_counter == 1) player.bmp = info->die2 ;
  else if(die_counter == 2) player.bmp = info->die3 ;
}

void laser_init()
{
  uint16_t hres = get_hres();

  for(int i=0; i<3; i++){
    laser[i].draw = false;
    laser[i].empty = true; //inicially all pos are empty
    laser[i].x = hres;
  }
}

void lasers_randomise()//Positons for each object
{
  uint16_t hres = get_hres();
  uint16_t vres = get_vres();

  for(int i=0; i < LASER_NUM; i++) { //search for an empty
    if(!laser[i].empty) //if i is not empty
      continue ; //tries in the next
    else if (laser[i].empty)  { //is empty
      laser[i].x = hres;//starts in the right
      laser[i].y = rand()%(vres-380)+100; //to stay between bondaries (100--488)
      laser[i].type = rand()%3;
      if(laser[i].type == 2) {
        laser[i].x_med = 108 + laser[i].x ;
        laser[i].y_med = 110 + laser[i].y ;
      }
      laser[i].width  = info->bmp[laser[i].type]->bitmapInfoHeader.width;
      laser[i].height = info->bmp[laser[i].type]->bitmapInfoHeader.height;
      laser[i].draw = true;
      laser[i].empty = false;
      break;
    }
  }
}

void laser_update_pos()
{
  int x_change_frame = 2*info->difficulty;

    for(int i=0; i<LASER_NUM; i++){
      if(laser[i].draw){
        laser[i].x -= x_change_frame;
        if(laser[i].type == 2)
          laser[i].x_med -= x_change_frame;
        if((laser[i].x + laser[i].width) < 0){
          laser[i].draw = false;
          laser[i].empty = true; //becames empty and ready to be again randomized
        }
      }
    }

}

void laser_draw()
{
    for(int i=0; i<LASER_NUM; i++){
      if(laser[i].draw ) // draws if laser_i exists and has been randomized
        drawBitmap(info->bmp[laser[i].type], laser[i].x, laser[i].y, ALIGN_LEFT);
    }
}

void snow_init()
{
  for(int i=0; i<20; i++){
    snow[i].width  = info->bmp[i]->bitmapInfoHeader.width;
    snow[i].height = info->bmp[i]->bitmapInfoHeader.height;
    snow[i].draw = false;
    snow[i].empty = true; //inicially all pos are empty
    snow[i].bmp = info->snowbmp;
  }
}

void snow_randomise()//Positons for each object
{
  for(int i=0; i<20; i++) { //search for an empty
    if(!snow[i].empty) //if i is not empty
      continue ; //tries in the next
    else if (snow[i].empty)  { //is empty
      snow[i].x = rand()%1024;
      snow[i].y = 0;
      snow[i].draw = true;
      snow[i].empty = false;
      break;
    }
  }
}

void snow_update_pos()
{
  uint16_t vres = get_vres();
  int x_change_frame = 2*info->difficulty;
  int y_change_frame = 2*info->difficulty;

    for(int i=0; i<20; i++){
      if(snow[i].draw){
        snow[i].x -= x_change_frame;
        snow[i].y += y_change_frame;
        if((snow[i].x + snow[i].width) < 0 || (snow[i].y - snow[i].height) > vres){
          snow[i].draw = false;
          snow[i].empty = true; //becames empty and ready to be again randomized
        }
      }
    }

}

void snow_draw()
{
    for(int i=0; i<20; i++){
      if(snow[i].draw ) // draws if snow_i exists and has been randomized
        drawBitmap(snow[i].bmp, snow[i].x, snow[i].y, ALIGN_LEFT);
    }
}

vetor* coins_init()
{
  vetor* vec;
  vec = vetor_novo();
  return vec;
}

void coins_randomise(vetor *vec)//Positons for each object
{
  uint16_t hres = get_hres();
  int i, j;
  int matrix_x = rand() % 10 + 1; //(1-8)
  int matrix_y = rand() % 6 + 1; //(1-5)
  int y_pos = rand() % 450 + 100;

  for( i=0; i < matrix_y; i++) {
    for( j=0; j < matrix_x; j++) {
      vetor_insere(vec, -1, hres+40*j, y_pos+i*25);
    }
  }
}

void coins_update_pos(vetor *vec)
{
  int x_change_frame = 2*info->difficulty;
  int width = info->coin->bitmapInfoHeader.width;

    for(int pos=0; pos<vec->tamanho; pos++) {
        vec->elementos[pos].x -= x_change_frame;
        if((vec->elementos[pos].x + width) < 0) {
          vetor_remove(vec, pos) ;
        }
    }
}

void coins_draw(vetor *vec)
{
    for(int i=0; i < vec->tamanho; i++)
      drawBitmap(info->coin, vec->elementos[i].x, vec->elementos[i].y, ALIGN_LEFT);
}

void rocket_init()
{
  uint16_t hres = get_hres();

  rocket.x = hres;
  rocket.width  = info->rocket->bitmapInfoHeader.width;
  rocket.height = info->rocket->bitmapInfoHeader.height;
  rocket.draw = false;
}

void rocket_randomise()//Positons for each object
{
  uint16_t hres = get_hres();

  rocket.x = hres;
  rocket.y = rand()%560+70; //to stay between bondaries (100--488)
  rocket.draw = true;
}

void rocket_update_pos()
{
  int x_change_frame = 2;

  rocket.x -= x_change_frame*info->difficulty+4;
  if((rocket.x + rocket.width) < 0){
      rocket.draw = false;
  }
}

void rocket_draw()
{
  if(rocket.draw ) {
    rocket_update_pos();
    drawBitmap(info->rocket, rocket.x, rocket.y, ALIGN_LEFT);
  }
}
