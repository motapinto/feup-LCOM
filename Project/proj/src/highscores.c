#include "highscores.h"
#include "game.h"

void highscore_init() {
  for(int i = 0; i < MAX_SCORES ; i++) {
      single[i].score = 0;
      single[i].day = 0;
      single[i].month = 0;
      single[i].year = 0;

      multi[i].score = 0;
      multi[i].day = 0;
      multi[i].month = 0;
      multi[i].year = 0;
  }
}

void highscore_read(game_mode game_type) {
  FILE* ptr;
  int temp;
  uint day, month, year, counter = 5;
  char line1[50], line2[50], *token;

  if(game_type == ONE_PLAY)
    ptr = fopen(file_path_singleplayer, "r");//read mode - file must exist
  else if(game_type == TWO_PLAY)
    ptr = fopen(file_path_multiplayer, "r");//read mode - file must exist
  else
    return;


  if(ptr == NULL) {
    printf("Could not open file in read mode\n"); //writes in console
    return;
  }

  rewind(ptr); //goes to begining of file

  while(counter > 0) { //until does not  reach end of file
    fgets(line1, 50, ptr);

    token = strtok(line1, ":");
    token = strtok(NULL, ":");    temp  =  strtol(token, NULL, 10); //strtol is equivalent to atoi

    fgets(line2, 50, ptr);
    token = strtok(line2, ":");
    token = strtok(NULL, "/");    day   =  strtol(token, NULL, 10);
    token = strtok(NULL, "/");    month =  strtol(token, NULL, 10);
    token = strtok(NULL, "/");    year  =  strtol(token, NULL, 10);

    highscore_add(temp, day, month, year, game_type);
    counter--;
  }

  fclose(ptr);

  return;
}

void highscore_write(game_mode game_type) {
  FILE* ptr;

  if(game_type == ONE_PLAY)
    ptr = fopen(file_path_singleplayer, "w");//read mode overriding previous info (write mode only)
  else if(game_type == TWO_PLAY)
    ptr = fopen(file_path_multiplayer, "w");//read mode overriding previous info (write mode only)
  else
    return;

  if(ptr == NULL){
    printf("Could not open file in write mode\n"); //writes in console
    return;
  }

  if(game_type == ONE_PLAY) {
    for(int i = 0; i < MAX_SCORES ; i++) {
      fprintf(ptr, "Score: %d\r\nDate: %d/%d/%d\r\n", single[i].score, single[i].day, single[i].month, single[i].year);
    }
  }

  else {
    for(int i = 0; i < MAX_SCORES ; i++) {
      fprintf(ptr, "Score: %d\r\nDate: %d/%d/%d\r\n\r\n", multi[i].score, multi[i].day, multi[i].month, multi[i].year);
    }
  }

  fclose(ptr);
}

void highscore_order(game_mode game_type) {
  int i,j;
  uint32_t temp;
  uint8_t day, month, year;

  if(game_type == ONE_PLAY) {
    for(i = 0; i < MAX_SCORES ; i++) {
      for(j = i+1; j < MAX_SCORES ; j++) {
        if(single[j].score > single[i].score) {
          day   = single[i].day;
          month = single[i].month;
          year  = single[i].year;
          temp  = single[i].score;

          single[i] = single[j];

          single[j].day   = day;
          single[j].month = month;
          single[j].year  = year;
          single[j].score = temp;
        }
      }
    }
  }

  else {
    for(i = 0; i < MAX_SCORES ; i++) {
      for(j = i+1; j < MAX_SCORES ; j++) {
        if(multi[j].score > multi[i].score) {
          day   = multi[j].day;
          month = multi[j].month;
          year  = multi[j].year;
          temp  = multi[j].score;

          multi[i] = multi[j];

          multi[j].day   = day;
          multi[j].month = month;
          multi[j].year  = year;
          multi[j].score = temp;
        }
      }
    }
  }

}

void highscore_add(uint32_t new_score, uint8_t new_day, uint8_t new_month, uint8_t new_year, game_mode game_type) {
  //printf("%d %d %d %d\n", new_score, new_day, new_month, new_year);
  if(game_type == ONE_PLAY) {
    if(new_score > single[MAX_SCORES-1].score) { //presuming it is in order
      single[MAX_SCORES-1].day   = new_day; //(if the there is a score bigger than the worst score -> add)
      single[MAX_SCORES-1].month = new_month;
      single[MAX_SCORES-1].year  = new_year;
      single[MAX_SCORES-1].score = new_score;
    }
  }

  else {
    if(new_score > multi[MAX_SCORES-1].score) { //presuming it is in order
      multi[MAX_SCORES-1].day   = new_day; //(if the there is a score bigger than the worst score -> add)
      multi[MAX_SCORES-1].month = new_month;
      multi[MAX_SCORES-1].year  = new_year;
      multi[MAX_SCORES-1].score = new_score;
    }
  }

  highscore_order(game_type);
}

void highscores_cleanup(game_mode game_type) {
  FILE* ptr;

  if(game_type == ONE_PLAY)
    ptr = fopen(file_path_singleplayer, "w");//read mode overriding previous info (write mode only)
  else if(game_type == TWO_PLAY)
    ptr = fopen(file_path_multiplayer, "w");//read mode overriding previous info (write mode only)
  return;
}
