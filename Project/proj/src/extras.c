#include <stdint.h>
#include <math.h>
#include "extras.h"

extern state_mach *machine;//from proj.c
extern game_info *info; //from proj.c

void distance_update_metros()
{
  int digit[6]={0};
  //insertion in global struct with bitmap number correspond to each postion
  if(machine->menu == PLAY_GAME) {
    info->dist_cover++;
    //separation  numbers by digits
    for(int i=1; i<=6; i++){
      digit[i-1] = (info->dist_cover % (int)pow(10, i))/(pow(10, i-1));
    }
    info->metros_pos0 = bitmap_num_Dist(digit[0]);//returns bitmap
    info->metros_pos1 = bitmap_num_Dist(digit[1]);//returns bitmap
    info->metros_pos2 = bitmap_num_Dist(digit[2]);//returns bitmap
    info->metros_pos3 = bitmap_num_Dist(digit[3]);//returns bitmap
    info->metros_pos4 = bitmap_num_Dist(digit[4]);//returns bitmap
    info->metros_pos5 = bitmap_num_Dist(digit[5]);//returns bitmap
  }

  else if(machine->menu == GAMEOVER ) {
    //separation  numbers by digits
    for(int i=1; i<=6; i++){
      digit[i-1] = (info->dist_cover % (int)pow(10, i))/(pow(10, i-1));
    }
    info->metros_pos0 = bitmap_to_num(digit[0]);//returns bitmap
    info->metros_pos1 = bitmap_to_num(digit[1]);//returns bitmap
    info->metros_pos2 = bitmap_to_num(digit[2]);//returns bitmap
    info->metros_pos3 = bitmap_to_num(digit[3]);//returns bitmap
    info->metros_pos4 = bitmap_to_num(digit[4]);//returns bitmap
    info->metros_pos5 = bitmap_to_num(digit[5]);//returns bitmap
  }
}

void distance_draw()
{//score bitmpas have 16 pixels of width
  if(machine->menu == PLAY_GAME) {
    drawBitmap(info->metro, 104, 38, ALIGN_LEFT);
    drawBitmap(info->metros_pos0, 85, 40, ALIGN_LEFT);
    drawBitmap(info->metros_pos1, 68, 40, ALIGN_LEFT);
    drawBitmap(info->metros_pos2, 51, 40, ALIGN_LEFT);
    drawBitmap(info->metros_pos3, 34, 40, ALIGN_LEFT);
    drawBitmap(info->metros_pos4, 17, 40, ALIGN_LEFT);
    drawBitmap(info->metros_pos5, 0, 40, ALIGN_LEFT);
  }

  if(machine->menu == GAMEOVER) {
    drawBitmap(info->metros_pos0, 840, 64, ALIGN_LEFT);
    drawBitmap(info->metros_pos1, 820, 64, ALIGN_LEFT);
    drawBitmap(info->metros_pos2, 800, 64, ALIGN_LEFT);
    drawBitmap(info->metros_pos3, 780, 64, ALIGN_LEFT);
    drawBitmap(info->metros_pos4, 760, 64, ALIGN_LEFT);
    drawBitmap(info->metros_pos5, 740, 64, ALIGN_LEFT);
  }
}
void score_update()
{
  int digit[6]={0};
  if(machine->menu == PLAY_GAME ) {
    //separation numbers by digits
    for(int i=1; i<=6; i++){
      digit[i-1] = (info->score_counter % (int)pow(10, i))/(pow(10, i-1));
    }
    //insertion in global struct with bitmap number correspond to each postion
    info->score_pos0 = bitmap_num_Score(digit[0]);//return bitmap
    info->score_pos1 = bitmap_num_Score(digit[1]);//return bitmap
    info->score_pos2 = bitmap_num_Score(digit[2]);//return bitmap
    info->score_pos3 = bitmap_num_Score(digit[3]);//return bitmap
    info->score_pos4 = bitmap_num_Score(digit[4]);//return bitmap
    info->score_pos5 = bitmap_num_Score(digit[5]);//return bitmap
  }
  else if(machine->menu == GAMEOVER) {

    //separation numbers by digits
    for(int i=1; i<=6; i++){
      digit[i-1] = (info->score_counter % (int)pow(10, i))/(pow(10, i-1));
    }
    //insertion in global struct with bitmap number correspond to each postion
    info->score_pos0 = bitmap_to_num(digit[0]);//return bitmap
    info->score_pos1 = bitmap_to_num(digit[1]);//return bitmap
    info->score_pos2 = bitmap_to_num(digit[2]);//return bitmap
    info->score_pos3 = bitmap_to_num(digit[3]);//return bitmap
    info->score_pos4 = bitmap_to_num(digit[4]);//return bitmap
    info->score_pos5 = bitmap_to_num(digit[5]);//return bitmap
  }

}
void score_draw()
{
  if(machine->menu == PLAY_GAME) {
    drawBitmap(info->score_pos0, 985, 40, ALIGN_LEFT);
    drawBitmap(info->score_pos1, 968, 40, ALIGN_LEFT);
    drawBitmap(info->score_pos2, 951, 40, ALIGN_LEFT);
    drawBitmap(info->score_pos3, 934, 40, ALIGN_LEFT);
    drawBitmap(info->score_pos4, 917, 40, ALIGN_LEFT);
    drawBitmap(info->score_pos5, 900, 40, ALIGN_LEFT);
  }
  else {
    drawBitmap(info->score_pos0, 840, 145, ALIGN_LEFT);
    drawBitmap(info->score_pos1, 820, 145, ALIGN_LEFT);
    drawBitmap(info->score_pos2, 800, 145, ALIGN_LEFT);
    drawBitmap(info->score_pos3, 780, 145, ALIGN_LEFT);
    drawBitmap(info->score_pos4, 760, 145, ALIGN_LEFT);
    drawBitmap(info->score_pos5, 740, 145, ALIGN_LEFT);
  }
}

void num_date(int i, game_mode type)
{
  int digit[6]={0};
  int value;

  if(type == ONE_PLAY && machine->menu == MAIN_MENU) {
    value = (info->day*pow(10, 4) + info->month*pow(10, 2) + info->year); //ex:070119
    //separation numbers by digits
    for(int i=1; i<=6; i++){
      digit[i-1] = (value % (int)pow(10, i))/(pow(10, i-1));
    }
    //insertion in global struct with bitmap number correspond to each postion
    info->date_0_pos = bitmap_to_num(digit[0]);//return bitmap
    info->date_1_pos = bitmap_to_num(digit[1]);//return bitmap
    info->date_2_pos = bitmap_to_num(digit[2]);//return bitmap
    info->date_3_pos = bitmap_to_num(digit[3]);//return bitmap
    info->date_4_pos = bitmap_to_num(digit[4]);//return bitmap
    info->date_5_pos = bitmap_to_num(digit[5]);//return bitmap
  }

  else {

    if(type == ONE_PLAY) {
      value = (single[i].day*pow(10, 4) + single[i].month*pow(10, 2) + single[i].year);
    }
    else if(type == TWO_PLAY) {
      value = (multi[i].day*pow(10, 4) + multi[i].month*pow(10, 2) + multi[i].year);
    }

    for(int j=1; j<=6; j++){
      digit[j-1] = (value % (int)pow(10, j))/(pow(10, j-1));
    }
    //insertion in global struct with bitmap number correspond to each postion
    info->date_0_pos = bitmap_to_num(digit[5]);//return bitmap
    info->date_1_pos = bitmap_to_num(digit[4]);//return bitmap
    info->date_2_pos = bitmap_to_num(digit[3]);//return bitmap
    info->date_3_pos = bitmap_to_num(digit[2]);//return bitmap
    info->date_4_pos = bitmap_to_num(digit[1]);//return bitmap
    info->date_5_pos = bitmap_to_num(digit[0]);//return bitmap
  }
}

void display_date() //date nao e a atual mas a dos highscores
{
  switch (machine->menu) {
    case MAIN_MENU:
      num_date(0, ONE_PLAY);
      drawBitmap(info->date_0_pos, 960, 655, ALIGN_LEFT);
      drawBitmap(info->date_1_pos, 940, 655, ALIGN_LEFT);
      drawBitmap(info->traco , 930, 660, ALIGN_LEFT);
      drawBitmap(info->date_2_pos, 900, 655, ALIGN_LEFT);
      drawBitmap(info->date_3_pos, 880, 655, ALIGN_LEFT);
      drawBitmap(info->traco , 870, 660, ALIGN_LEFT);
      drawBitmap(info->date_4_pos, 840, 655, ALIGN_LEFT);
      drawBitmap(info->date_5_pos, 820, 655, ALIGN_LEFT);
      break;

    case HIGHSCORES: //depois de azer multiplayer meter os draw de multi em baixo de de fazer num datemulti
      num_date(0, ONE_PLAY);
      drawBitmap(info->date_0_pos, 200, 230, ALIGN_LEFT);      drawBitmap(info->date_0_pos, 712, 230, ALIGN_LEFT);
      drawBitmap(info->date_1_pos, 220, 230, ALIGN_LEFT);      drawBitmap(info->date_1_pos, 732, 230, ALIGN_LEFT);
      drawBitmap(info->traco , 250, 235, ALIGN_LEFT);          drawBitmap(info->traco , 762, 235, ALIGN_LEFT);
      drawBitmap(info->date_2_pos, 260, 230, ALIGN_LEFT);      drawBitmap(info->date_2_pos, 772, 230, ALIGN_LEFT);
      drawBitmap(info->date_3_pos, 280, 230, ALIGN_LEFT);      drawBitmap(info->date_3_pos, 792, 230, ALIGN_LEFT);
      drawBitmap(info->traco , 310, 235, ALIGN_LEFT);          drawBitmap(info->traco , 822, 235, ALIGN_LEFT);
      drawBitmap(info->date_4_pos, 320, 230, ALIGN_LEFT);      drawBitmap(info->date_4_pos, 832, 230, ALIGN_LEFT);
      drawBitmap(info->date_5_pos, 340, 230, ALIGN_LEFT);      drawBitmap(info->date_5_pos, 852, 230, ALIGN_LEFT);

      num_date(1, ONE_PLAY);
      drawBitmap(info->date_0_pos, 200, 338, ALIGN_LEFT);      drawBitmap(info->date_0_pos, 712, 338, ALIGN_LEFT);
      drawBitmap(info->date_1_pos, 220, 338, ALIGN_LEFT);      drawBitmap(info->date_1_pos, 732, 338, ALIGN_LEFT);
      drawBitmap(info->traco , 250, 343, ALIGN_LEFT);          drawBitmap(info->traco , 762, 343, ALIGN_LEFT);
      drawBitmap(info->date_2_pos, 260, 338, ALIGN_LEFT);      drawBitmap(info->date_2_pos, 772, 338, ALIGN_LEFT);
      drawBitmap(info->date_3_pos, 280, 338, ALIGN_LEFT);      drawBitmap(info->date_3_pos, 792, 338, ALIGN_LEFT);
      drawBitmap(info->traco , 310, 343, ALIGN_LEFT);          drawBitmap(info->traco , 822, 343, ALIGN_LEFT);
      drawBitmap(info->date_4_pos, 320, 338, ALIGN_LEFT);      drawBitmap(info->date_4_pos, 832, 338, ALIGN_LEFT);
      drawBitmap(info->date_5_pos, 340, 338, ALIGN_LEFT);      drawBitmap(info->date_5_pos, 852, 338, ALIGN_LEFT);

      num_date(2, ONE_PLAY);
      drawBitmap(info->date_0_pos, 200, 440, ALIGN_LEFT);      drawBitmap(info->date_0_pos, 712, 440, ALIGN_LEFT);
      drawBitmap(info->date_1_pos, 220, 440, ALIGN_LEFT);      drawBitmap(info->date_1_pos, 732, 440, ALIGN_LEFT);
      drawBitmap(info->traco , 250, 445, ALIGN_LEFT);          drawBitmap(info->traco , 762, 445, ALIGN_LEFT);
      drawBitmap(info->date_2_pos, 260, 440, ALIGN_LEFT);      drawBitmap(info->date_2_pos, 772, 440, ALIGN_LEFT);
      drawBitmap(info->date_3_pos, 280, 440, ALIGN_LEFT);      drawBitmap(info->date_3_pos, 792, 440, ALIGN_LEFT);
      drawBitmap(info->traco , 310, 445, ALIGN_LEFT);          drawBitmap(info->traco , 822, 445, ALIGN_LEFT);
      drawBitmap(info->date_4_pos, 320, 440, ALIGN_LEFT);      drawBitmap(info->date_4_pos, 832, 440, ALIGN_LEFT);
      drawBitmap(info->date_5_pos, 340, 440, ALIGN_LEFT);      drawBitmap(info->date_5_pos, 852, 440, ALIGN_LEFT);

      num_date(3, ONE_PLAY);
      drawBitmap(info->date_0_pos, 200, 540, ALIGN_LEFT);      drawBitmap(info->date_0_pos, 712, 540, ALIGN_LEFT);
      drawBitmap(info->date_1_pos, 220, 540, ALIGN_LEFT);      drawBitmap(info->date_1_pos, 732, 540, ALIGN_LEFT);
      drawBitmap(info->traco , 250, 545, ALIGN_LEFT);          drawBitmap(info->traco , 762, 545, ALIGN_LEFT);
      drawBitmap(info->date_2_pos, 260, 540, ALIGN_LEFT);      drawBitmap(info->date_2_pos, 772, 540, ALIGN_LEFT);
      drawBitmap(info->date_3_pos, 280, 540, ALIGN_LEFT);      drawBitmap(info->date_3_pos, 792, 540, ALIGN_LEFT);
      drawBitmap(info->traco , 310, 545, ALIGN_LEFT);          drawBitmap(info->traco , 822, 545, ALIGN_LEFT);
      drawBitmap(info->date_4_pos, 320, 540, ALIGN_LEFT);      drawBitmap(info->date_4_pos, 832, 540, ALIGN_LEFT);
      drawBitmap(info->date_5_pos, 340, 540, ALIGN_LEFT);      drawBitmap(info->date_5_pos, 852, 540, ALIGN_LEFT);

      num_date(4, ONE_PLAY);
      drawBitmap(info->date_0_pos, 200, 643, ALIGN_LEFT);      drawBitmap(info->date_0_pos, 712, 643, ALIGN_LEFT);
      drawBitmap(info->date_1_pos, 220, 643, ALIGN_LEFT);      drawBitmap(info->date_1_pos, 732, 643, ALIGN_LEFT);
      drawBitmap(info->traco , 245, 648, ALIGN_LEFT);          drawBitmap(info->traco , 757, 648, ALIGN_LEFT);
      drawBitmap(info->date_2_pos, 260, 643, ALIGN_LEFT);      drawBitmap(info->date_2_pos, 772, 643, ALIGN_LEFT);
      drawBitmap(info->date_3_pos, 280, 643, ALIGN_LEFT);      drawBitmap(info->date_3_pos, 792, 643, ALIGN_LEFT);
      drawBitmap(info->traco , 305, 648, ALIGN_LEFT);          drawBitmap(info->traco , 817, 648, ALIGN_LEFT);
      drawBitmap(info->date_4_pos, 320, 643, ALIGN_LEFT);      drawBitmap(info->date_4_pos, 832, 643, ALIGN_LEFT);
      drawBitmap(info->date_5_pos, 340, 643, ALIGN_LEFT);      drawBitmap(info->date_5_pos, 852, 643, ALIGN_LEFT);

      break;

    default:
      break;
  }
}
void num_time()
{
  int digit[6]={0};
  int date =(info->hour*pow(10, 4) + info->min*pow(10, 2) + info->seg);

  if(machine->menu == MAIN_MENU) {
    //separation if numbers by digits - stack overflow
    for(int i=1; i<=6; i++){
      digit[i-1] = (date % (int)pow(10, i))/(pow(10, i-1));
    }
    //insertion in global struct with bitmap number correspond to each postion
    info->time_0_pos = bitmap_to_num(digit[0]);//return bitmap
    info->time_1_pos = bitmap_to_num(digit[1]);//return bitmap
    info->time_2_pos = bitmap_to_num(digit[2]);//return bitmap
    info->time_3_pos = bitmap_to_num(digit[3]);//return bitmap
    info->time_4_pos = bitmap_to_num(digit[4]);//return bitmap
    info->time_5_pos = bitmap_to_num(digit[5]);//return bitmap
  }

  else if(machine->menu == GAMEOVER) {
    int value = info->final_int_counter  / 60;//nº seg jogados
    int hour, min, seg;
    min = value/ 60;
    seg = value - (min * 60);
    hour = min / 60;
    min = min - (hour * 60);
    value = (hour*pow(10, 4) + min*pow(10, 2) + seg);

    for(int i=1; i<=6; i++){
      digit[i-1] = (value % (int)pow(10, i))/(pow(10, i-1));
    }

    info->time_0_pos = bitmap_to_num(digit[0]);//return bitmap
    info->time_1_pos = bitmap_to_num(digit[1]);//return bitmap
    info->time_2_pos = bitmap_to_num(digit[2]);//return bitmap
    info->time_3_pos = bitmap_to_num(digit[3]);//return bitmap
    info->time_4_pos = bitmap_to_num(digit[4]);//return bitmap
    info->time_5_pos = bitmap_to_num(digit[5]);//return bitmap
  }
}

void display_time()
{
  num_time();

  if(machine->menu == MAIN_MENU) {
    drawBitmap(info->time_0_pos, 960, 710, ALIGN_LEFT);
    drawBitmap(info->time_1_pos, 940, 710, ALIGN_LEFT);
    drawBitmap(info->time_sep ,  920, 710, ALIGN_LEFT);
    drawBitmap(info->time_2_pos, 900, 710, ALIGN_LEFT);
    drawBitmap(info->time_3_pos, 880, 710, ALIGN_LEFT);
    drawBitmap(info->time_sep ,  860, 710, ALIGN_LEFT);
    drawBitmap(info->time_4_pos, 840, 710, ALIGN_LEFT);
    drawBitmap(info->time_5_pos, 820, 710, ALIGN_LEFT);
  }
  else if(machine->menu == GAMEOVER) {
    drawBitmap(info->time_0_pos, 880, 236, ALIGN_LEFT);
    drawBitmap(info->time_1_pos, 860, 236, ALIGN_LEFT);
    drawBitmap(info->time_sep ,  840, 236, ALIGN_LEFT);
    drawBitmap(info->time_2_pos, 820, 236, ALIGN_LEFT);
    drawBitmap(info->time_3_pos, 800, 236, ALIGN_LEFT);
    drawBitmap(info->time_sep ,  780, 236, ALIGN_LEFT);
    drawBitmap(info->time_4_pos, 760, 236, ALIGN_LEFT);
    drawBitmap(info->time_5_pos, 740, 236, ALIGN_LEFT);
  }
}

void num_highscore(int i, game_mode type)
{
  int digit[6]={0};
  int value;

  if(type == ONE_PLAY) {
    value = single[i].score;
  }
  else if(type == TWO_PLAY) {
    value = multi[i].score;
  }

  for(int j=1; j<=6; j++){
    digit[j-1] = (value % (int)pow(10, j))/(pow(10, j-1));
  }
  //insertion in global struct with bitmap number correspond to each postion
  info->hscore_0_pos = bitmap_to_num(digit[5]);//return bitmap
  info->hscore_1_pos = bitmap_to_num(digit[4]);//return bitmap
  info->hscore_2_pos = bitmap_to_num(digit[3]);//return bitmap
  info->hscore_3_pos = bitmap_to_num(digit[2]);//return bitmap
  info->hscore_4_pos = bitmap_to_num(digit[1]);//return bitmap
  info->hscore_5_pos = bitmap_to_num(digit[0]);//return bitmap
}

void display_highscore()
{                    //SINGLE_PLAYER                                          //MULTIPLAYER
    num_highscore(0, ONE_PLAY);                              //num_highscore(0, TWO_PLAY);
    drawBitmap(info->hscore_0_pos, 200, 266, ALIGN_LEFT);      drawBitmap(info->hscore_0_pos, 712, 266, ALIGN_LEFT);
    drawBitmap(info->hscore_1_pos, 220, 266, ALIGN_LEFT);      drawBitmap(info->hscore_1_pos, 732, 266, ALIGN_LEFT);
    drawBitmap(info->hscore_2_pos, 240, 266, ALIGN_LEFT);      drawBitmap(info->hscore_2_pos, 752, 266, ALIGN_LEFT);
    drawBitmap(info->hscore_3_pos, 260, 266, ALIGN_LEFT);      drawBitmap(info->hscore_3_pos, 772, 266, ALIGN_LEFT);
    drawBitmap(info->hscore_4_pos, 280, 266, ALIGN_LEFT);      drawBitmap(info->hscore_4_pos, 792, 266, ALIGN_LEFT);
    drawBitmap(info->hscore_5_pos, 300, 266, ALIGN_LEFT);      drawBitmap(info->hscore_5_pos, 812, 266, ALIGN_LEFT);
    num_highscore(1, ONE_PLAY);                              //num_highscore(1, TWO_PLAY);
    drawBitmap(info->hscore_0_pos, 200, 378, ALIGN_LEFT);      drawBitmap(info->hscore_0_pos, 712, 378, ALIGN_LEFT);
    drawBitmap(info->hscore_1_pos, 220, 378, ALIGN_LEFT);      drawBitmap(info->hscore_1_pos, 732, 378, ALIGN_LEFT);
    drawBitmap(info->hscore_2_pos, 240, 378, ALIGN_LEFT);      drawBitmap(info->hscore_2_pos, 752, 378, ALIGN_LEFT);
    drawBitmap(info->hscore_3_pos, 260, 378, ALIGN_LEFT);      drawBitmap(info->hscore_3_pos, 772, 378, ALIGN_LEFT);
    drawBitmap(info->hscore_4_pos, 280, 378, ALIGN_LEFT);      drawBitmap(info->hscore_4_pos, 792, 378, ALIGN_LEFT);
    drawBitmap(info->hscore_5_pos, 300, 378, ALIGN_LEFT);      drawBitmap(info->hscore_5_pos, 812, 378, ALIGN_LEFT);
    num_highscore(2, ONE_PLAY);                              //num_highscore(2, TWO_PLAY);
    drawBitmap(info->hscore_0_pos, 200, 482, ALIGN_LEFT);      drawBitmap(info->hscore_0_pos, 712, 482, ALIGN_LEFT);
    drawBitmap(info->hscore_1_pos, 220, 482, ALIGN_LEFT);      drawBitmap(info->hscore_1_pos, 732, 482, ALIGN_LEFT);
    drawBitmap(info->hscore_2_pos, 240, 482, ALIGN_LEFT);      drawBitmap(info->hscore_2_pos, 752, 482, ALIGN_LEFT);
    drawBitmap(info->hscore_3_pos, 260, 482, ALIGN_LEFT);      drawBitmap(info->hscore_3_pos, 772, 482, ALIGN_LEFT);
    drawBitmap(info->hscore_4_pos, 280, 482, ALIGN_LEFT);      drawBitmap(info->hscore_4_pos, 792, 482, ALIGN_LEFT);
    drawBitmap(info->hscore_5_pos, 300, 482, ALIGN_LEFT);      drawBitmap(info->hscore_5_pos, 812, 482, ALIGN_LEFT);
    num_highscore(3, ONE_PLAY);                              //num_highscore(3, TWO_PLAY);
    drawBitmap(info->hscore_0_pos, 200, 582, ALIGN_LEFT);      drawBitmap(info->hscore_0_pos, 712, 582, ALIGN_LEFT);
    drawBitmap(info->hscore_1_pos, 220, 582, ALIGN_LEFT);      drawBitmap(info->hscore_1_pos, 732, 582, ALIGN_LEFT);
    drawBitmap(info->hscore_2_pos, 240, 582, ALIGN_LEFT);      drawBitmap(info->hscore_2_pos, 752, 582, ALIGN_LEFT);
    drawBitmap(info->hscore_3_pos, 260, 582, ALIGN_LEFT);      drawBitmap(info->hscore_3_pos, 772, 582, ALIGN_LEFT);
    drawBitmap(info->hscore_4_pos, 280, 582, ALIGN_LEFT);      drawBitmap(info->hscore_4_pos, 792, 582, ALIGN_LEFT);
    drawBitmap(info->hscore_5_pos, 300, 582, ALIGN_LEFT);      drawBitmap(info->hscore_5_pos, 812, 582, ALIGN_LEFT);
    num_highscore(4, ONE_PLAY);                              //num_highscore(4, TWO_PLAY);
    drawBitmap(info->hscore_0_pos, 200, 686, ALIGN_LEFT);      drawBitmap(info->hscore_0_pos, 712, 686, ALIGN_LEFT);
    drawBitmap(info->hscore_1_pos, 220, 686, ALIGN_LEFT);      drawBitmap(info->hscore_1_pos, 732, 686, ALIGN_LEFT);
    drawBitmap(info->hscore_2_pos, 240, 686, ALIGN_LEFT);      drawBitmap(info->hscore_2_pos, 752, 686, ALIGN_LEFT);
    drawBitmap(info->hscore_3_pos, 260, 686, ALIGN_LEFT);      drawBitmap(info->hscore_3_pos, 772, 686, ALIGN_LEFT);
    drawBitmap(info->hscore_4_pos, 280, 686, ALIGN_LEFT);      drawBitmap(info->hscore_4_pos, 792, 686, ALIGN_LEFT);
    drawBitmap(info->hscore_5_pos, 300, 686, ALIGN_LEFT);      drawBitmap(info->hscore_5_pos, 812, 686, ALIGN_LEFT);
}

Bitmap* bitmap_num_Score(int num)//score
{
  if(num == 1){
    return info->one;
  }
  else if (num == 2){
    return info->two;
  }
  else if (num == 3){
    return info->three;
  }
  else if (num == 4){
    return info->four;
  }
  else if (num == 5){
    return info->five;
  }
  else if (num == 6){
    return info->six;
  }
  else if (num == 7){
    return info->seven;
  }
  else if (num == 8){
    return info->eight;
  }
  else if (num == 9){
    return info->nine;
  }
  else if (num == 0){
    return info->zero;
  }
  return NULL;
}
Bitmap* bitmap_num_Dist(int num)//distance
{
  if(num == 1){
    return info->one_dist;
  }
  else if (num == 2){
    return info->two_dist;
  }
  else if (num == 3){
    return info->three_dist;
  }
  else if (num == 4){
    return info->four_dist;
  }
  else if (num == 5){
    return info->five_dist;
  }
  else if (num == 6){
    return info->six_dist;
  }
  else if (num == 7){
    return info->seven_dist;
  }
  else if (num == 8){
    return info->eight_dist;
  }
  else if (num == 9){
    return info->nine_dist;
  }
  else if (num == 0){
    return info->zero_dist;
  }
  return NULL;
}
Bitmap* bitmap_to_num(int num)//distance
{
  if(num == 1){
    return info->one_num;
  }
  else if (num == 2){
    return info->two_num;
  }
  else if (num == 3){
    return info->three_num;
  }
  else if (num == 4){
    return info->four_num;
  }
  else if (num == 5){
    return info->five_num;
  }
  else if (num == 6){
    return info->six_num;
  }
  else if (num == 7){
    return info->seven_num;
  }
  else if (num == 8){
    return info->eight_num;
  }
  else if (num == 9){
    return info->nine_num;
  }
  else if (num == 0){
    return info->zero_num;
  }
  return NULL;
}
void delete_all_bitmap()
{
  deleteBitmap(info->mouse);
  //Backgrounds for the menus
  deleteBitmap(info->main_menu);
  deleteBitmap(info->background);
  deleteBitmap(info->christmas_back);
  deleteBitmap(info->options_menu);
  deleteBitmap(info->gameover);
  deleteBitmap(info->highscores_menu);

  //Objects in game
  deleteBitmap(info->bmp[0]);
  deleteBitmap(info->bmp[1]);
  deleteBitmap(info->bmp[2]);
  deleteBitmap(info->coin);
  deleteBitmap(info->rose);
  //Score background and score digits and metro counters
  deleteBitmap(info->one);          deleteBitmap(info->one_dist);
  deleteBitmap(info->two);          deleteBitmap(info->two_dist);
  deleteBitmap(info->three);        deleteBitmap(info->three_dist);
  deleteBitmap(info->four);         deleteBitmap(info->four_dist);
  deleteBitmap(info->five);         deleteBitmap(info->five_dist);
  deleteBitmap(info->six);          deleteBitmap(info->six_dist);
  deleteBitmap(info->seven);        deleteBitmap(info->seven_dist);
  deleteBitmap(info->eight);        deleteBitmap(info->eight_dist);
  deleteBitmap(info->nine);         deleteBitmap(info->nine_dist);
  deleteBitmap(info->zero);         deleteBitmap(info->zero_dist);
  deleteBitmap(info->score);        deleteBitmap(info->metro);
  deleteBitmap(info->traco);        deleteBitmap(info->time_sep);
  deleteBitmap(info->score_pos0);   deleteBitmap(info->metros_pos0);
  deleteBitmap(info->score_pos1);   deleteBitmap(info->metros_pos1);
  deleteBitmap(info->score_pos2);   deleteBitmap(info->metros_pos2);
  deleteBitmap(info->score_pos3);   deleteBitmap(info->metros_pos3);
  deleteBitmap(info->score_pos4);   deleteBitmap(info->metros_pos4);
  deleteBitmap(info->score_pos5);   deleteBitmap(info->metros_pos5);

  deleteBitmap(info->one_num);          deleteBitmap(info->six_num);
  deleteBitmap(info->two_num);          deleteBitmap(info->seven_num);
  deleteBitmap(info->three_num);        deleteBitmap(info->eight_num);
  deleteBitmap(info->four_num);         deleteBitmap(info->nine_num);
  deleteBitmap(info->five_num);         deleteBitmap(info->zero_num);
  deleteBitmap(info->time_0_pos);       deleteBitmap(info->time_5_pos);
  deleteBitmap(info->time_1_pos);       deleteBitmap(info->time_sep);
  deleteBitmap(info->time_2_pos);       deleteBitmap(info->date_0_pos);
  deleteBitmap(info->time_3_pos);       deleteBitmap(info->date_1_pos);
  deleteBitmap(info->time_4_pos);       deleteBitmap(info->date_2_pos);

  deleteBitmap(info->date_3_pos);       deleteBitmap(info->hscore_0_pos);
  deleteBitmap(info->date_4_pos);       deleteBitmap(info->hscore_1_pos);
  deleteBitmap(info->date_5_pos);       deleteBitmap(info->hscore_2_pos);
  deleteBitmap(info->hscore_3_pos);     deleteBitmap(info->hscore_0_pos);
  deleteBitmap(info->hscore_4_pos);     deleteBitmap(info->hscore_1_pos);
  deleteBitmap(info->hscore_5_pos);     deleteBitmap(info->hscore_2_pos);

  deleteBitmap(info->run1);   deleteBitmap(info->fly1);   deleteBitmap(info->die1);
  deleteBitmap(info->run2);   deleteBitmap(info->fly2);   deleteBitmap(info->die2);
  deleteBitmap(info->run3);   deleteBitmap(info->fly3);   deleteBitmap(info->die3);
  deleteBitmap(info->run4);   deleteBitmap(info->fly4);   deleteBitmap(info->die4);
  deleteBitmap(info->run5);   deleteBitmap(info->fly5);   deleteBitmap(info->snowbmp);
}
