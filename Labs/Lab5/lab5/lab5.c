#include <lcom/lcf.h>
#include <lcom/timer.h>
#include <lcom/lab5.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h> //sleep function

#include "keyboard.h"
#include "video.h"
#include "vbe.h"

#include "i8254.h"
#include "ibm_8042.h"
#include "video_macros.h" //video graphics macros

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");
  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab5/trace.txt");
  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab5/output.txt");
  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;
  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();
  return 0;
}

int (video_test_init)(uint16_t mode, uint8_t delay) {//change to graphics mode
  void *init = vg_init(mode); //virtual address
	if(init == NULL){
		printf("Erro em vg_init()\n");
    if(vg_exit()!=0){
  		printf("Erro em vg_exit()\n");
  		return FUNCTION_FAIL;
  	}
    return FUNCTION_FAIL;
	}

  sleep(delay);//"podemos usar sleep - moodle"

	if(vg_exit()!=0){//back to text mode
		printf("Erro em vg_exit()\n");
		return FUNCTION_FAIL;
	}
  return 0;
}


int (video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {

  void *init = vg_init(mode); //virtual address
	if(init == NULL){
    printf("Erro em vg_init()\n");
    if(vg_exit()!=0){//if vg_init dont work go to default minix text mode
  		printf("Erro em vg_exit()\n");
  		return FUNCTION_FAIL;
  	}
    return FUNCTION_FAIL;
	}

	if(draw_ret(x, y, width, height, color) != 0){
		printf("Erro em draw_ret()\n");
    return FUNCTION_FAIL;
	}

	if(kbd_scan_esc(false)!=0){//in kbd.c
		printf("Erro em kbd_scan_esc()\n");
    return FUNCTION_FAIL;
	}

	if(vg_exit()!=0){
		printf("Erro em vg_exit()\n");
		return FUNCTION_FAIL;
	}
  return 0;
}

int (video_test_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step) {//no_ret represent the number in x and y axis
  if(no_rectangles <0){
		printf("Numero de retangulos tem de ser superior ou igual a 0\n");
		return PARAMETROS_FAIL;
	}

  void *init = vg_init(mode); //virtual address
	if(init == NULL){
    printf("Erro em vg_init()\n");
    if(vg_exit()!=0){//if vg_init dont work go to default minix text mode
  		printf("Erro em vg_exit()\n");
  		return FUNCTION_FAIL;
  	}
    return FUNCTION_FAIL;
	}

  vbe_mode_info_t vbe_mode;
  if (vbe_get_info(mode, &vbe_mode) != 0){ //Get vbe info
    printf("Erro em vb_get_info()\n");
    return FUNCTION_FAIL;
  }

	if(draw_pat(no_rectangles, first, step, vbe_mode) != 0){
		printf("Erro em draw_ret()\n");
    return FUNCTION_FAIL;
	}

  if(kbd_scan_esc(false)!=0){
	   printf("Erro em kbd_scan_esc()\n");
     return FUNCTION_FAIL;
	}

	if(vg_exit()!=0){
		printf("Erro em vg_exit()\n");
		return FUNCTION_FAIL;
	}

  return 0;
}

int (video_test_xpm)(const char *xpm[], uint16_t x, uint16_t y)
{
  void *init = vg_init(MODO_105); //virtual address
	if(init == NULL){
    printf("Erro em vg_init()\n");
    if(vg_exit()!=0){//if vg_init dont work go to default minix text mode
  		printf("Erro em vg_exit()\n");
      return FUNCTION_FAIL;
  	}
    return FUNCTION_FAIL;
	}

  if(xpm_draw(xpm, x, y) != 0){ //false = draw, true=destroy
    printf("Erro em xpm_draw()\n");
    return FUNCTION_FAIL;
  }

  if(kbd_scan_esc(false)!=0){
    printf("Erro em kbd_scan_esc()\n");
    return FUNCTION_FAIL;
  }

  if(vg_exit()!=0){
		printf("Erro em vg_exit()\n");
		return FUNCTION_FAIL;
	}
  return 0;
}

int (video_test_move)(const char *xpm[], uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf, int16_t speed, uint8_t fr_rate)
{
  void *init = vg_init(MODO_105); //virtual address
	if(init == NULL){
    printf("Erro em vg_init()\n");
    if(vg_exit()!=0){//if vg_init dont work go to default minix text mode
  		printf("Erro em vg_exit()\n");
  		return FUNCTION_FAIL;
  	}
    return FUNCTION_FAIL;
	}

  if(speed>0){
    if(draw_move_speedy(xpm, xi, yi, xf, yf, speed, fr_rate) != 0){
      printf("Erro em draw_move_speedy()\n");
      FUNCTION_FAIL;
    }
  }
  else {
    if(draw_move_not_speedy(xpm, xi, yi, xf, yf, speed, fr_rate) != 0){
      printf("Erro em draw_move_not_speedy\n");
      FUNCTION_FAIL;
    }
  }

  if(vg_exit()!=0){
		printf("Erro em vg_exit()\n");
		return FUNCTION_FAIL;
	}
  return 0;
}


int (video_test_controller)()
{
  void *init = lm_init(false);
  if(init == NULL){
    return FUNCTION_FAIL;
  }

  vg_vbe_contr_info_t ctrl_info;
  vbe_ctrl_info info;

  if(vbe_get_control(&ctrl_info, &info) != 0){
    printf("Erro em vbe_get_control()\n");
    return FUNCTION_FAIL;
  }
  //desisto...
  memcpy(ctrl_info.VBESignature, info.VbeSignature, 4);
  ctrl_info.VBEVersion[0] = info.VbeVersion;
  ctrl_info.VBEVersion[1] = info.VbeVersion >> 8;
  ctrl_info.TotalMemory = info.TotalMemory*64;
  ctrl_info.OEMString = (char*)return_address(info.OemStringPtr, init);
  ctrl_info.VideoModeList = (uint16_t*)return_address(info.VideoModePtr, init);
  ctrl_info.OEMVendorNamePtr = (char*)return_address(info.OemVendorNamePtr, init);
  ctrl_info.OEMProductNamePtr = (char*)return_address(info.OemProductNamePtr, init);
  ctrl_info.OEMProductRevPtr = (char*)return_address(info.OemProductRevPtr, init);

  if((vg_display_vbe_contr_info)(&ctrl_info)!= 0){
    printf("Erro em vg_display_vbe_contr_info()\n");
    return FUNCTION_FAIL;
  }
  return 0;
}

/*
typedef uint8_t BCD;
typedef struct {
    char VBESignature[4];
    BCD VBEVersion[2];
    char *OEMString;
    uint16_t *VideoModeList;
    uint32_t TotalMemory;
    char *OEMVendorNamePtr;
    char *OEMProductNamePtr;
    char *OEMProductRevPtr;
} vg_vbe_contr_info_t;

*/
