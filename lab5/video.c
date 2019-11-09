#include <lcom/lcf.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

#include <machine/int86.h> // /usr/src/include/arch/i386
#include <lcom/timer.h>
#include "vbe.h"
#include "keyboard.h"
#include "video.h"

#include "ibm_8042.h"
#include "video_macros.h"

extern uint8_t scan_code; //from kbd.c
extern uint32_t contador; //from timer.c

void pixel_set(uint16_t x, uint16_t y, uint32_t color)
{
	char *vid_m = get_vid_mem();
	uint16_t hres = get_hres();
	uint8_t bits_ppixel = get_bits_per_pixel();
	uint8_t byte = ceil(bits_ppixel/8.0);

	vid_m +=((x + y * hres) * byte);

  for (uint i = 0; i < byte; i++){
    *vid_m = color;
    vid_m++;
    color = color >> 8;
  }
}

int draw_ret(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color)
{
	uint16_t hres = get_hres();
	uint16_t vres = get_vres();
	uint16_t i, j;

	if ( x < 0 || x >= hres || y < 0 || y >= vres)
		return PARAMETROS_FAIL;

	for (j = 0; j < height; j++) {
		for (i = 0; i < width; i++) {
			pixel_set(x + i, y + j, color);
		}
	}
	return 0;
}

void disassemble_rgb(uint32_t *color, uint32_t *red,
				uint32_t *green, uint32_t *blue, vbe_mode_info_t vbe_mode)
{
	*blue = *color & ((1 << vbe_mode.BlueMaskSize)-1);
	*green = *color >> vbe_mode.BlueMaskSize & ((1 << vbe_mode.GreenMaskSize)-1);
	*red = *color >> (vbe_mode.BlueMaskSize + vbe_mode.GreenMaskSize) &((1 << vbe_mode.RedMaskSize)-1);
}

void assemble_rgb(uint32_t *color, uint32_t *red,
				uint32_t *green, uint32_t *blue, vbe_mode_info_t vbe_mode)
{
	*color = *blue | (*green<<vbe_mode.BlueMaskSize) | (*red<<(vbe_mode.BlueMaskSize+vbe_mode.GreenMaskSize));
}

int draw_pat(uint8_t no_rect, uint32_t first, uint8_t step
						,vbe_mode_info_t vbe_mode)
{
	uint16_t hres = get_hres(),  width=hres/no_rect;
	uint16_t vres = get_vres(),  height=vres/no_rect;
	uint32_t color=0, red_first=0, blue_first=0, green_first=0;
	uint32_t red, blue, green;
	bool index = false;

	if(vbe_mode.BitsPerPixel == 8){
		index = true;
	}
	else{
		disassemble_rgb(&first, &red_first, &green_first, &blue_first, vbe_mode);
	}

	for(int row=0; row<no_rect; row++){
		for(int col=0; col<no_rect; col++){

			if(index) {
				color = (first + (row * no_rect + col) * step) % (1 << (get_bits_per_pixel()));
			}
			else {
				red = (red_first + col * step) % (1 << vbe_mode.RedMaskSize);
				green = (green_first + row * step) % (1 << vbe_mode.GreenMaskSize);
				blue = (blue_first + (col + row) * step) % (1 << vbe_mode.BlueMaskSize);
				assemble_rgb(&color, &red, &green, &blue, vbe_mode);
			}

			if(draw_ret(width*col, row*height, width, height, color)!=0){
				printf("Erro em draw_ret()\n");
				return FUNCTION_FAIL;
			}
		}
	}
	return 0;
}

int xpm_draw(const char *xpm[], uint16_t x, uint16_t y)
{
	int s_width, s_heigh;
	char *sprite = read_xpm(xpm, &s_width, &s_heigh);

	if(sprite == NULL){
		printf("Erro em read_xpm()\n");
		return FUNCTION_FAIL;
	}

	for(int i=0; i<s_heigh; i++){
		for(int j=0; j<s_width; j++){
			pixel_set(x+j, y+i, *(sprite +s_width*i + j));
		}
	}
	free(sprite);
	return 0;
}

int xpm_destroy(const char *xpm[], uint16_t x, uint16_t y)
{
	int s_width, s_heigh;
	char *sprite = read_xpm(xpm, &s_width, &s_heigh);

	if(sprite == NULL){
		printf("Erro em read_xpm()\n");
		return FUNCTION_FAIL;
	}

	for(int i=0; i<s_heigh; i++){
		for(int j=0; j<s_width; j++){
			pixel_set(x+j, y+i, 0);
		}
	}
	free(sprite);
	return 0;
}

//speed<0 = nº frame por cada delta de 1 pixel
int draw_move_not_speedy(const char *xpm[], uint16_t x, uint16_t y,
	uint16_t xf, uint16_t yf, int16_t speed, uint8_t fr_rate)
	{

	int ipc_status, r, time_per_frame = sys_hz()/fr_rate;
	uint8_t kbd_irq_set, timer_irq_set;
	uint16_t hres = get_hres(), vres = get_vres();
	uint16_t xn = x, yn = y;
	message msg;

	if( yf>vres || xf>hres || x<0 || y<0){
		printf("Erros nos parametros x e/ou y\n");
		return PARAMETROS_FAIL;
	}
	if (kbd_subscribe_int(&kbd_irq_set) != 0) {
		printf("Erro em timer_subscribe_int()\n");
		return FUNCTION_FAIL;
	}
	if (timer_subscribe_int(&timer_irq_set) != 0) {
		printf("Erro em timer_subscribe_int()\n");
		return FUNCTION_FAIL;
	}
	if(xpm_draw(xpm, xn, yn)!=0){//primeira frame
		printf("Erro em xpm_draw()\n");
		return FUNCTION_FAIL;
	}

	while (scan_code!= ESC) {
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}

		if (is_ipc_notify(ipc_status)) { // received notification
			switch (_ENDPOINT_P(msg.m_source)) {
				case HARDWARE: // hardware interrupt notification
					if (msg.m_notify.interrupts & timer_irq_set){
						timer_int_handler(); speed = abs(speed);
						if(contador%speed==0){

							if(fr_rate<=60){
								if((xn<xf || yn<yf) && (contador%time_per_frame==0)){
										if(xpm_destroy(xpm, xn, yn)!=0){
											printf("Erro em xpm_draw()\n");
											return FUNCTION_FAIL;
										}

										if(xf==x) yn++;
										else if(yf==y) xn++;

										if(yn>yf || xn>xf){
											if(xpm_draw(xpm, xf, yf)!=0){
												printf("Erro em xpm_destroy(false)\n");
												return FUNCTION_FAIL;
										  }
											break;
										}

										if(xpm_draw(xpm, xn, yn)!=0){
											printf("Erro em xpm_destroy(false)\n");
											return FUNCTION_FAIL;
									  }
							  }
							}
					  }
				  }
					if (msg.m_notify.interrupts & kbd_irq_set){ // subscribed interrupt
						kbc_ih();
					}
							break;
				default:
					break; // no other notifications expected: do
			}
		}
	}

	if (kbd_unsubscribe_int() != 0) {
		printf("ERROR: kbd_subscribe_int failed.\n");
		return FUNCTION_FAIL;
  }

	if (timer_unsubscribe_int() != 0) {
		printf("ERROR: timer_unsubscribe_int failed.\n");
		return FUNCTION_FAIL;
	}

return 0;
}


//speed>0 = delta do nº de pixeis por interrupt/frame
int draw_move_speedy(const char *xpm[], uint16_t x, uint16_t y,
	uint16_t xf, uint16_t yf, int16_t speed, uint8_t fr_rate)
	{

	int ipc_status, r, time_per_frame = sys_hz()/fr_rate;
	uint8_t kbd_irq_set, timer_irq_set;
	uint16_t hres = get_hres(), vres = get_vres();
	uint16_t xn = x, yn = y;
	message msg;

	if( yf>vres || xf>hres || x<0 || y<0){
		printf("Erros nos parametros x e/ou y\n");
		return PARAMETROS_FAIL;
	}
	if (kbd_subscribe_int(&kbd_irq_set) != 0) {
		printf("Erro em timer_subscribe_int()\n");
		return FUNCTION_FAIL;
	}
	if (timer_subscribe_int(&timer_irq_set) != 0) {
		printf("Erro em timer_subscribe_int()\n");
		return FUNCTION_FAIL;
	}
	if(xpm_draw(xpm, xn, yn)!=0){//primeira frame
		printf("Erro em xpm_draw()\n");
		return FUNCTION_FAIL;
	}

	while (scan_code!= ESC) {
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}

		if (is_ipc_notify(ipc_status)) { // received notification
			switch (_ENDPOINT_P(msg.m_source)) {
				case HARDWARE: // hardware interrupt notification
					if (msg.m_notify.interrupts & timer_irq_set){
						timer_int_handler();
							if(fr_rate<=60){
								if((xn<xf || yn<yf) && (contador%time_per_frame==0)){
										if(xpm_destroy(xpm, xn, yn)!=0){
											printf("Erro em xpm_draw()\n");
											return FUNCTION_FAIL;
										}

										if(xf==x) yn+=speed;
										else if(yf==y) xn+=speed;

										if(yn>yf || xn>xf){
											if(xpm_draw(xpm, xf, yf)!=0){
												printf("Erro em xpm_destroy(false)\n");
												return FUNCTION_FAIL;
										  }
											break;
										}
										if(xpm_draw(xpm, xn, yn)!=0){
											printf("Erro em xpm_destroy(false)\n");
											return FUNCTION_FAIL;
									  }
							  }
							}
				  }
					if (msg.m_notify.interrupts & kbd_irq_set){ // subscribed interrupt
						kbc_ih();
					}
							break;
				default:
					break; // no other notifications expected: do
			}
		}
	}

	if (kbd_unsubscribe_int() != 0) {
		printf("ERROR: kbd_subscribe_int failed.\n");
		return FUNCTION_FAIL;
  }

	if (timer_unsubscribe_int() != 0) {
		printf("ERROR: timer_unsubscribe_int failed.\n");
		return FUNCTION_FAIL;
	}

return 0;
}
