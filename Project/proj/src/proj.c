// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>
#include <time.h>
#include "game.h"
#include "mouse.h" //just to test... after delete
#include "vetor.h"
#include "keyboard.h"
#include "objects.h"
#include "state.h"
#include "ibm_8042.h"

extern int packet_store_counter; //from mouse.c
extern uint8_t scan_code; //from keyboard.c
extern struct packet pp;//from mouse.c

//SINGLETONS
state_mach* machine;
game_info *info;
vetor *coins;

// Any header files included below this line should have been created by you

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  //lcf_trace_calls("/home/lcom/labs/proj/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/proj/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int (proj_main_loop)()
{
  sys_enable_iop (SELF);
  void *init = vg_init(0x117); //virtual address
	if(init == NULL){
		printf("Erro em vg_start()\n");
    if(vg_exit()!=0){//return to minix default text mode
      printf("Erro em vg_exit()\n");
      return FUNCTION_FAIL;
    }
	}
  coins = coins_init();
  info = game_init();//loads bitmaps, inicialized global struct game_info info
  if(info == NULL)  return FUNCTION_FAIL;

  machine = machine_start();//stars state machine
  player_init();
  mouse_init();
  laser_init();
  snow_init();
  rocket_init();

  srand(time(NULL));

  univesal_ih(); //while final state is not reached
  game_exit();//prepares to exit and frees all alocated memory

  if(vg_exit()!=0){//return to minix default text mode
    printf("Erro em vg_exit()\n");
    return FUNCTION_FAIL;
  }
  printf("\t\tEnter the command lcom_run proj to run again\n\n\n\n\n");
  return 0;
}
