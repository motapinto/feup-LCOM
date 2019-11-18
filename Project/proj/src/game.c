#include "game.h"
#include <lcom/timer.h>
#include "rtc.h"
#include "mouse.h"
#include "keyboard.h"

extern uint32_t contador; //from timer.c
extern int packet_store_counter; //from mouse.c
extern uint8_t scan_code; //from keyboard.c
extern struct packet pp;//from mouse.c

extern state_mach *machine;//from proj.c
extern game_info *info; //from proj.c
extern highscore single[];//from highscores.h
extern highscore multi[];//from highscores.h
extern Mouse mouse;
extern vetor *coins; //from proj.c

int run_counter = 0;
int fly_counter = 0;
int die_counter = 0;
int col_counter = 0;
bool lost = false;

game_info* game_init()
{
  info = (game_info*)malloc(sizeof(game_info)); //allocates space in memory

  if(info == NULL){//request failed
    printf("Error in malloc(sizeof(game_info))!\n");
    return NULL;
  }
  if(mouse_subscribe_int(&info->mouse_irq) != 0){
    printf("Erro em mouse_subscribe_int()\n");
    return NULL;
  }
  if(timer_subscribe_int(&info->timer_irq) != 0){
    printf("Erro em timer_subscribe_int())\n");
    return NULL;
  }
  if(kbd_subscribe_int(&info->kbd_irq) != 0){
    printf("Erro em kbd_subscribe_int())\n");
  }
  if(rtc_subscribe_int(&info->rtc_irq) != 0){
    printf("Erro em rtc_subscribe_int())\n");
  }

  info->dist_cover    = 0;
  info->coins_collected = 0;
  info->score_counter = 0;
  info->int_counter   = 0;
  info->final_int_counter = 0;
  info->scan_code     = 0;
  info->difficulty    = 1; //default option
  info->snow          = 0; //false
  info->time_out      = 0;
  //Bitmap images used:
  info->mouse         = loadBitmap("/home/lcom/labs/proj/res/mouse.bmp");
  info->rose          = loadBitmap("/home/lcom/labs/proj/res/rose.bmp");
  info->coin          = loadBitmap("/home/lcom/labs/proj/res/coin.bmp");
  info->rocket        = loadBitmap("/home/lcom/labs/proj/res/rocket.bmp");
  info->point        = loadBitmap("/home/lcom/labs/proj/res/point.bmp");
  //Backgrounds for the menus
  info->main_menu        = loadBitmap("/home/lcom/labs/proj/res/main_menu.bmp");
  info->background       = loadBitmap("/home/lcom/labs/proj/res/background.bmp");
  info->christmas_back   = loadBitmap("/home/lcom/labs/proj/res/background_christmas.bmp");
  info->options_menu     = loadBitmap("/home/lcom/labs/proj/res/menu_options.bmp");
  info->gameover         = loadBitmap("/home/lcom/labs/proj/res/gameover.bmp");
  info->highscores_menu  = loadBitmap("/home/lcom/labs/proj/res/menu_highscores.bmp");
  //Objects in game (lasers)
  info->bmp[0]        = loadBitmap("/home/lcom/labs/proj/res/hor.bmp");
  info->bmp[1]        = loadBitmap("/home/lcom/labs/proj/res/vert.bmp");
  info->bmp[2]        = loadBitmap("/home/lcom/labs/proj/res/diag.bmp");
  info->snowbmp       = loadBitmap("/home/lcom/labs/proj/res/bola_de_neve.bmp");
  //Character states and sprites
  info->fly1       =  loadBitmap("/home/lcom/labs/proj/res/fly_1.bmp");
  info->fly2       =  loadBitmap("/home/lcom/labs/proj/res/fly_2.bmp");
  info->fly3       =  loadBitmap("/home/lcom/labs/proj/res/fly_3.bmp");
  info->fly4       =  loadBitmap("/home/lcom/labs/proj/res/fly_4.bmp");
  info->fly5       =  loadBitmap("/home/lcom/labs/proj/res/fly5.bmp");
  info->run1       =  loadBitmap("/home/lcom/labs/proj/res/run1.bmp");
  info->run2       =  loadBitmap("/home/lcom/labs/proj/res/run2.bmp");
  info->run3       =  loadBitmap("/home/lcom/labs/proj/res/run3.bmp");
  info->run4       =  loadBitmap("/home/lcom/labs/proj/res/run4.bmp");
  info->run5       =  loadBitmap("/home/lcom/labs/proj/res/run5.bmp");
  info->die1       =  loadBitmap("/home/lcom/labs/proj/res/dead1.bmp");
  info->die2       =  loadBitmap("/home/lcom/labs/proj/res/dead2.bmp");
  info->die3       =  loadBitmap("/home/lcom/labs/proj/res/dead3.bmp");
  info->die4       =  loadBitmap("/home/lcom/labs/proj/res/dead4.bmp");
  //Score digits
  info->one        = loadBitmap("/home/lcom/labs/proj/res/1.bmp");
  info->two        = loadBitmap("/home/lcom/labs/proj/res/2.bmp");
  info->three      = loadBitmap("/home/lcom/labs/proj/res/3.bmp");
  info->four       = loadBitmap("/home/lcom/labs/proj/res/4.bmp");
  info->five       = loadBitmap("/home/lcom/labs/proj/res/5.bmp");
  info->six        = loadBitmap("/home/lcom/labs/proj/res/6.bmp");
  info->seven      = loadBitmap("/home/lcom/labs/proj/res/7.bmp");
  info->eight      = loadBitmap("/home/lcom/labs/proj/res/8.bmp");
  info->nine       = loadBitmap("/home/lcom/labs/proj/res/9.bmp");
  info->zero       = loadBitmap("/home/lcom/labs/proj/res/0.bmp");
  info->score      = loadBitmap("/home/lcom/labs/proj/res/score.bmp");
  //Represents score digits postions - each will contain a bitmap x_digit - inicializes with 00000
  info->score_pos0 = info->zero; info->score_pos1 = info->zero;
  info->score_pos2 = info->zero; info->score_pos3 = info->zero;
  info->score_pos4 = info->zero; info->score_pos5 = info->zero;
  //Distance digits and M(for metros)
  info->one_dist   = loadBitmap("/home/lcom/labs/proj/res/1_dist.bmp");
  info->two_dist   = loadBitmap("/home/lcom/labs/proj/res/2_dist.bmp");
  info->three_dist = loadBitmap("/home/lcom/labs/proj/res/3_dist.bmp");
  info->four_dist  = loadBitmap("/home/lcom/labs/proj/res/4_dist.bmp");
  info->five_dist  = loadBitmap("/home/lcom/labs/proj/res/5_dist.bmp");
  info->six_dist   = loadBitmap("/home/lcom/labs/proj/res/6_dist.bmp");
  info->seven_dist = loadBitmap("/home/lcom/labs/proj/res/7_dist.bmp");
  info->eight_dist = loadBitmap("/home/lcom/labs/proj/res/8_dist.bmp");
  info->nine_dist  = loadBitmap("/home/lcom/labs/proj/res/9_dist.bmp");
  info->zero_dist  = loadBitmap("/home/lcom/labs/proj/res/0_dist.bmp");
  info->metro      = loadBitmap("/home/lcom/labs/proj/res/M.bmp");
  //Represents distance digits postions - each will contain a bitmap x_digit - inicializes with 00000
  info->metros_pos0 = info->zero_dist; info->metros_pos1 = info->zero_dist;
  info->metros_pos2 = info->zero_dist; info->metros_pos3 = info->zero_dist;
  info->metros_pos4 = info->zero_dist; info->metros_pos5 = info->zero_dist;
  //TIME, DATE, HIGHSCORES DIGITS (for main menu and hihscores menu)
  info->one_num   = loadBitmap("/home/lcom/labs/proj/res/1_num.bmp");
  info->two_num   = loadBitmap("/home/lcom/labs/proj/res/2_num.bmp");
  info->three_num = loadBitmap("/home/lcom/labs/proj/res/3_num.bmp");
  info->four_num  = loadBitmap("/home/lcom/labs/proj/res/4_num.bmp");
  info->five_num  = loadBitmap("/home/lcom/labs/proj/res/5_num.bmp");
  info->six_num   = loadBitmap("/home/lcom/labs/proj/res/6_num.bmp");
  info->seven_num = loadBitmap("/home/lcom/labs/proj/res/7_num.bmp");
  info->eight_num = loadBitmap("/home/lcom/labs/proj/res/8_num.bmp");
  info->nine_num  = loadBitmap("/home/lcom/labs/proj/res/9_num.bmp");
  info->zero_num  = loadBitmap("/home/lcom/labs/proj/res/0_num.bmp");
  info->traco     = loadBitmap("/home/lcom/labs/proj/res/traco.bmp");
  info->time_sep  = loadBitmap("/home/lcom/labs/proj/res/dois_pontos.bmp");
  //Represents time, date, highscore digits postions - each will contain a bitmap x_digit - inicializes with 00000
  info->time_0_pos = info->zero_num; info->time_1_pos = info->zero_num;
  info->time_2_pos = info->zero_num; info->time_3_pos = info->zero_num;
  info->time_4_pos = info->zero_num; info->time_5_pos = info->zero_num;

  info->date_0_pos = info->zero_num; info->date_1_pos = info->zero_num;
  info->date_2_pos = info->zero_num; info->date_3_pos = info->zero_num;
  info->date_4_pos = info->zero_num; info->date_5_pos = info->zero_num;

  info->hscore_0_pos = info->zero_num; info->hscore_1_pos = info->zero_num;
  info->hscore_2_pos = info->zero_num; info->hscore_3_pos = info->zero_num;
  info->hscore_4_pos = info->zero_num; info->hscore_4_pos = info->zero_num;

  highscore_read(ONE_PLAY); //reads to single player highscores struct (singleplayer_highscores.txt)

  return info; //returns info with all var's inicialized
}

void game_exit()
{
  highscore_write(ONE_PLAY); //writes to txt file if higher scores were made
  vetor_apaga(coins);
  if(mouse_unsubscribe_int()!=0) printf("Erro em mouse_unsubscribe_int()\n");
  if(timer_unsubscribe_int()!=0) printf("Erro em timer_unsubscribe_int()\n");
  if(kbd_unsubscribe_int()!=0)   printf("Erro em kbd_unsubscribe_int()\n");
  if(rtc_unsubscribe_int()!=0)   printf("Erro em rtc_unsubscribe_int()\n");

  delete_machine(machine);//return voids
  free(info); //frees all dynamic allocated memory for struct
}

void univesal_ih()
{
  int r, ipc_status;
  message msg;
  bool print = false;

  while(machine->menu != COMPLETE) { //based in mouse inverted V gesture
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification! */
      switch (_ENDPOINT_P(msg.m_source)) {
      case HARDWARE: /* hardware interrupt notification */

          if (msg.m_notify.interrupts & info->timer_irq) { /* subscribed interrupt */
            timer_int_handler();
            info->int_counter = contador;
            print = true;
          }

          if (msg.m_notify.interrupts & info->mouse_irq) { // subscribed interrupt
            mouse_ih(); //"Should read only one byte per interrupt"
            info->pp = pp;
          }

          if (msg.m_notify.interrupts & info->kbd_irq) { // subscribed interrupt
            //kbc_ih();
            kbc_asm_ih();
            info->scan_code = scan_code;
            keyboard_device();
          }

          if(packet_store_counter > 2){
              mouse_device();
              packet_store_counter = 0;
          }

          if(print){
            time_device();//atualiza frame
            print = false;
          }

          break;
      default:
          break; /* no other notifications expected: do nothing */
      }
    }
    else{}
  }
}

void time_device()
{
    uint16_t hres = get_hres();
    int diff = info->difficulty;

    if (machine->menu == MAIN_MENU) {
      drawBitmap(info->main_menu, 0, 0, ALIGN_LEFT); //draws main menu
      drawBitmap(info->mouse, mouse.x, mouse.y, ALIGN_LEFT);

      display_date();  //draws date in main menu
      display_time(); //draws time in main menu
      read_time();
      read_date();
    }

    else if (machine->menu == PLAY_GAME) {

      if(info->snow) {
        drawBitmap(info->christmas_back, -2*diff*info->int_counter%hres, 0, ALIGN_RIGHT);
        drawBitmap(info->christmas_back, -2*diff*info->int_counter%hres, 0, ALIGN_LEFT);
        snow_draw();
        snow_update_pos();
        snow_randomise();
      }
      else {
        drawBitmap(info->background, -2*diff*info->int_counter%hres, 0, ALIGN_RIGHT);
        drawBitmap(info->background, -2*diff*info->int_counter%hres, 0, ALIGN_LEFT);
      }

      coins_draw(coins);
      coins_update_pos(coins);
      player_draw();
      score_draw();
      rocket_draw();
      distance_draw();
      laser_draw();
      laser_update_pos();//after updating laser pos see if there is colission

      info->score_counter = info->difficulty*(info->dist_cover + 3*info->coins_collected) ;
      score_update();

      if(info->int_counter % abs(40-6*diff) == 0)
        distance_update_metros();
      if(info->int_counter % abs(12-2*diff) == 0)
        run_counter++;
      if(info->int_counter % 5 == 0)
        fly_counter++;

      if(info->int_counter % (200-diff*49) == 0) {  //faz randomise
        if(laser[0].empty || laser[1].empty || laser[2].empty) { //if one of them is empty3
          lasers_randomise(); //randomise lasers only if there is less than 3
        }
      }

      if(info->int_counter % (400-6*diff) == 0)
        rocket_randomise();

      if(coins->tamanho == 0)
        coins_randomise(coins);

      if(!lost && (collision(LASERS) || collision(ROCKETS) )) {
        lost = true;
        player.bmp = info->die1;
        player.y02 = abs(player.y-650);
      }

      if(collect_coins(coins)) {
        score_update();
      }
      if(collect_coins(coins)) {
        score_update();
      }

      if(lost) {
        player.time2 += 12/60.0;
        player.fall_counter++;
        player.y = player.y02 - 5*player.time2*player.time2; // y = y0 - 5*t^2
        player.y = abs(player.y-650);
        if(player.y >= 650)
          player.y = 650;
      }

      if((info->int_counter % 8 == 0) && lost) {
        player_die();
        if(player.y == 650)
          menu_input(COLLISION);
      }

      if (machine->movement == RUN_LOW && !lost)
          jetpack_run();

      else if((machine->movement == FLY_UP || machine->movement == FLY_DOWN) && !lost) {
        jetpack_fly();
        run_counter = 0;
      }
    }

    else if (machine->menu == MENU_OPTIONS) {
      drawBitmap(info->options_menu, 0, 0, ALIGN_LEFT);
      drawBitmap(info->mouse, mouse.x, mouse.y, ALIGN_LEFT);
    }

    else if (machine->menu == GAMEOVER) {
      if(!info->time_out) {
        info->final_int_counter = info->int_counter ;
        info->time_out = true;
      }

      drawBitmap(info->gameover, 0, 0, ALIGN_LEFT);
      drawBitmap(info->mouse, mouse.x, mouse.y, ALIGN_LEFT);
      score_update();
      score_draw();
      distance_update_metros();
      distance_draw();
      display_time();
    }

    else if (machine->menu == HIGHSCORES) {
      drawBitmap(info->highscores_menu, 0, 0, ALIGN_LEFT);
      drawBitmap(info->mouse, mouse.x, mouse.y, ALIGN_LEFT);
      display_highscore();
      display_date();
    }
    page_flipping();
}

void mouse_device()
{

    if (machine->menu == MAIN_MENU) {
      mouse_update();
      main_menu_select();
    }

    else if (machine->menu == PLAY_GAME) {
      if (machine->movement == RUN_LOW) {
        if (info->pp.lb)
          jetpack_move(UP);
        else
          jetpack_move(DOWN);
      }
      if (machine->movement == FLY_UP) {
        if (info->pp.lb)
          jetpack_move(UP);
        else
          jetpack_move(DOWN);
      }
      if (machine->movement == FLY_DOWN) {
        if (info->pp.lb)
          jetpack_move(UP);
        else
          jetpack_move(DOWN);
      }
    }

    else if (machine->menu == MENU_OPTIONS) {
      mouse_update();
      options_menu_select();
    }

    else if (machine->menu == GAMEOVER) {
      mouse_update();
      gameover_menu_select();
    }

    else if (machine->menu == HIGHSCORES) {
      mouse_update();
      highscores_menu_select();
    }
}

void keyboard_device()
{
    if (machine->menu == PLAY_GAME) {
        if (info->scan_code == UP_MAKE || info->scan_code == SPACE_BAR)//or space. see space bar
          jetpack_move(UP);
        else
          jetpack_move(DOWN);
        if (info->scan_code == ESC){
          //menu_input(ESC_PRESSED);
          machine->menu = GAMEOVER;
        }
    }

    else if (machine->menu == MENU_OPTIONS) {
        if(info->scan_code == ESC)
          menu_input(ESC_PRESSED);
    }

    else if (machine->menu == GAMEOVER) {
        if(info->scan_code == ESC)
          menu_input(ESC_PRESSED);
    }

    else if (machine->menu == HIGHSCORES) {
        if(info->scan_code == ESC)
          menu_input(ESC_PRESSED);
    }
}

int collision(Obj obj)
{ //um dos cantos dos lasers tem de estar contido na imagem ou vice-versa
/* 1º) intersecao entee quadrados - return
  3º) memset rosa do 3º buffer
  4º) draw bitmap em rosa do obj 1(player) para 3rd bufferi
  5º) criar nova funcao que recebe o buffer - funcao semelhante ao drawbitmap mas em ve de desenhar verifica se houver 1 pixel dif de rosa entao ha colisao
*/
  bool obj_inside_cse = false;
  bool obj_inside_csd = false;
  bool obj_inside_cie = false;
  bool obj_inside_cid = false;
  int cse_x, cid_x, cse_y, cid_y;

  switch(obj) {
    case LASERS:
        for(int i=0; i<LASER_NUM; i++){
          cse_x = laser[i].x,                  cse_y = laser[i].y ;
          cid_x = laser[i].x + laser[i].width, cid_y = laser[i].y + laser[i].height ;

          //QUANDO OS OBJETOS(x) ENTRAM DENTRO DA IMAGEM (player)
          if((cid_x>player.x && cid_x<player.x+player.width &&
             cse_y>player.y && cse_y<player.y+player.height) ||( //csd obj entra na img do player
             cse_x>player.x && cse_x<player.x+player.width &&
             cse_y<player.y && cid_y>player.y+player.height)){ //img do player entra do lado esqerdo  da img do obj (qd este e mais largo)
             obj_inside_csd = true;
          }

          if((cse_x>player.x && cse_x<player.x+player.width &&
            cse_y>player.y && cse_y<player.y+player.height) || //cse obj entra na img do player
            (cse_x<player.x && cid_x>player.x+player.width &&
            cse_y<player.y && cid_y<player.y+player.height && cid_y > player.y)){//img do player entra em baixo da img do obj (qd este e mais largo)
            obj_inside_cse = true;
          }

          if((cid_x>player.x && cid_x<player.x+player.width &&
            cid_y>player.y && cid_y<player.y+player.height) || //cid obj entra na img do player
            (cse_x<player.x && cid_x>player.x+player.width &&
            cse_y>player.y && cse_y<player.y+player.height)){ //img do player entra em cima da img do obj (quanod este e mais largo)
            obj_inside_cid = true;
          }

          if(cse_x>player.x && cse_x<player.x+player.width && cid_y>player.y && cid_y<player.y+player.height)// cie obj entra na img do player
            obj_inside_cie = true;


          if(obj_inside_csd || obj_inside_cse || obj_inside_cie || obj_inside_cid){
            draw_rd(info->rose, 0, 0, ALIGN_LEFT); //draws in the 3rd buffer all rose
            draw_rd(info->bmp[laser[i].type], laser[i].x,laser[i].y, ALIGN_LEFT); //draws player in 3rd buffer
            if(drawBitmap_collision(player.bmp, player.x, player.y, ALIGN_LEFT))
              return 1;
          }
        }
        break;

    case ROCKETS:
            cse_x = rocket.x,                  cse_y = rocket.y ;
            cid_x = rocket.x + rocket.width,   cid_y = rocket.y + rocket.height ;

            //QUANDO OS OBJETOS(x) ENTRAM DENTRO DA IMAGEM (player)
            if((cid_x>player.x && cid_x<player.x+player.width &&
               cse_y>player.y && cse_y<player.y+player.height) ||(
               cse_x>player.x && cse_x<player.x+player.width &&
               cse_y<player.y && cid_y>player.y+player.height)){//
              obj_inside_csd = true;
            }

            if((cse_x>player.x && cse_x<player.x+player.width &&
              cse_y>player.y && cse_y<player.y+player.height) ||
              (cse_x<player.x && cid_x>player.x+player.width &&
              cse_y<player.y && cid_y<player.y+player.height)){//laser vertical baixo e laser horzontal direita
              obj_inside_cse = true;
            }

            if((cid_x>player.x && cid_x<player.x+player.width &&
              cid_y>player.y && cid_y<player.y+player.height) ||
              (cse_x<player.x && cid_x>player.x+player.width &&
              cse_y>player.y && cse_y<player.y+player.height)){
              obj_inside_cid = true;
            }

            if(cse_x>player.x && cse_x<player.x+player.width && cid_y>player.y && cid_y<player.y+player.height){//laser vertical baixo
              obj_inside_cie = true;
            }

            if(obj_inside_csd || obj_inside_cse || obj_inside_cie || obj_inside_cid){
              draw_rd(info->rose, 0, 0, ALIGN_LEFT); //draws in the 3rd buffer all rose
              draw_rd(info->rocket, rocket.x, rocket.y, ALIGN_LEFT); //draws player in 3rd buffer
              if(drawBitmap_collision(player.bmp, player.x, player.y, ALIGN_LEFT)) {
                return 1;
              }
          break;
          }
    default:
        break;
  }

  return 0;
}

int collect_coins()
{
  bool obj_inside_cse = false;
  bool obj_inside_csd = false;
  bool obj_inside_cie = false;
  bool obj_inside_cid = false;

  int width = info->coin->bitmapInfoHeader.width;
  int height = info->coin->bitmapInfoHeader.height;
  //points
  for(int i=0; i<coins->tamanho; i++){
    int cse_x = coins->elementos[i].x,                  cse_y = coins->elementos[i].y ;
    int cid_x = coins->elementos[i].x + width,          cid_y = coins->elementos[i].y + height ;

    //QUANDO OS OBJETOS(x) ENTRAM DENTRO DA IMAGEM (player)
    if(cid_x>player.x && cid_x<player.x+player.width && cse_y>player.y && cse_y<player.y+player.height) {//
      obj_inside_csd = true;
    }

    if(cse_x>player.x && cse_x<player.x+player.width && cse_y>player.y && cse_y<player.y+player.height) {//laser vertical baixo e laser horzontal direita
      obj_inside_cse = true;
    }

    if(cid_x>player.x && cid_x<player.x+player.width && cid_y>player.y && cid_y<player.y+player.height) {
      obj_inside_cid = true;
    }

    if(cse_x>player.x && cse_x<player.x+player.width && cid_y>player.y && cid_y<player.y+player.height) {//laser vertical baixo
      obj_inside_cie = true;
    }

    if(obj_inside_csd || obj_inside_cse || obj_inside_cie || obj_inside_cid) {
      info->coins_collected++;
      vetor_remove(coins, i);
      return 1;
    }
  }
  return 0;
}

void reboot_game()//if esc is pressed to exit game without exiting aplication must free and alocated proprely memory
{
  highscore_add(info->score_counter, info->day, info->month, info->year, ONE_PLAY);

  info->dist_cover        = 0;
  info->score_counter     = 0;
  info->int_counter       = 0;
  info->final_int_counter = 0;
  info->coins_collected   = 0;
  info->time_out          = false;
  contador                = 0;

  info->score_pos0 = info->zero; info->score_pos1 = info->zero;
  info->score_pos2 = info->zero; info->score_pos3 = info->zero;
  info->score_pos4 = info->zero; info->score_pos5 = info->zero;

  info->metros_pos0 = info->zero_dist; info->metros_pos1 = info->zero_dist;
  info->metros_pos2 = info->zero_dist; info->metros_pos3 = info->zero_dist;
  info->metros_pos4 = info->zero_dist; info->metros_pos5 = info->zero_dist;

  run_counter = 0;
  fly_counter = 0;
  die_counter = 0;
  lost = false;

  player_init();
  laser_init();
  snow_init();
  laser_init();
  rocket_init();
  srand(time(NULL));
  vetor_eliminate(coins);
}

void main_menu_select()//which menu is clicked
{
  if(info->pp.lb) {
    if(mouse.x>725 && mouse.x<1010 && mouse.y<106 && mouse.y>16)
      menu_input(PLAY);
  /*  else if(mouse.x>725 && mouse.x<1010 && mouse.y<217 && mouse.y>123)
      menu_input(PLAY_2);*/
    else if(mouse.x>725 && mouse.x<1010 && mouse.y<323 && mouse.y>233)
      menu_input(GO_OPTIONS);
    else if(mouse.x>725 && mouse.x<1010 && mouse.y<431 && mouse.y>340)
      menu_input(SCORES);
    else if(mouse.x>725 && mouse.x<1010 && mouse.y<542 && mouse.y>445)
      menu_input(EXIT);
  }
}
void options_menu_select()//which menu is clicked
{
  if(info->pp.lb) {
    if(mouse.x>529 && mouse.x<748 && mouse.y<296 && mouse.y>247)//normal
      info->difficulty = 1;
    else if(mouse.x>529 && mouse.x<748 && mouse.y<365 && mouse.y>316)//hard
      info->difficulty = 2;
    else if(mouse.x>529 && mouse.x<748 && mouse.y<433 && mouse.y>384)//extreme
      info->difficulty = 3;
    else if(mouse.x>529 && mouse.x<748 && mouse.y<598 && mouse.y>548)//normal
      info->snow = false;
    else if(mouse.x>529 && mouse.x<748 && mouse.y<667 && mouse.y>618)//christmas mode
      info->snow = true;
    else if(mouse.x>684 && mouse.x<742 && mouse.y<120 && mouse.y>67)//back
      menu_input(BACK);
  }
}
void gameover_menu_select()//which menu is clicked
{
  if(info->pp.lb) {
    if(mouse.x>524 && mouse.x<888 && mouse.y<501 && mouse.y>342) {
      reboot_game();//go to main menu -> game must reset
      machine->movement = RUN_LOW;
      menu_input(BACK);
    }
    else if(mouse.x>525 && mouse.x<975 && mouse.y<687 && mouse.y>528) {
      reboot_game();//play again -> game must reset
      machine->movement = RUN_LOW;
      menu_input(PLAY);
    }
  }
}
void highscores_menu_select()//which menu is clicked
{
  if(info->pp.lb) {
    if(mouse.x>4 && mouse.x<124 && mouse.y<64 && mouse.y>5) {
      menu_input(BACK);
    }
  }
}
