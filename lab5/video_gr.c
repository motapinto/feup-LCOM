#include <lcom/lcf.h>
#include <lcom/timer.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

#include <machine/int86.h> // /usr/src/include/arch/i386
#include "i8254.h"
#include "ibm_8042.h"
#include "video_macros.h"
#include "vbe.h"

/* Private global variables */

static char *video_mem;         /* frame-buffer VM address */
static uint16_t h_res; /* Horizontal screen resolution in pixels */
static uint16_t v_res; /* Vertical screen resolution in pixels */
static uint8_t bits_per_pixel; /* Number of VRAM bits per pixel */


void *(vg_init)(uint16_t mode) {//Implementada no powerpoint
	struct minix_mem_range mr; /* physical memory range */
	static uint vram_size, vram_base;
	vbe_mode_info_t vbe_mode;
	int r;

	lm_init(false);

	if (vbe_get_info(mode, &vbe_mode) != 0){ //Get vbe info
		printf("Erro em vb_get_info()\n");
		return NULL;
	}

	v_res = vbe_mode.YResolution; /* Horizontal screen resolution in pixels */
	h_res = vbe_mode.XResolution;/* Vertical screen resolution in pixels */
	bits_per_pixel = vbe_mode.BitsPerPixel;/* Number of VRAM bits per pixel */
	vram_size = h_res * v_res * ceil(bits_per_pixel / 8.0);//Store the size of the vram
	vram_base = vbe_mode.PhysBasePtr;

	/* Allow memory mapping */
	mr.mr_base = (phys_bytes) vram_base;
	mr.mr_limit = mr.mr_base + vram_size;
	if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
		panic("sys_privctl (ADD_MEM) failed: %d\n", r);

	/* Map memory */
	video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);/* frame-buffer VM address */

	if(video_mem == MAP_FAILED){
			panic("Error mapping video memory\n");
		 	return NULL;
	}

	if (vbe_set_mode(SET_VBE_MODE, mode) != 0){ //Set graphics mode
		printf("Erro em vb_set_mode()\n");
		return NULL;
	}

	return video_mem;//virtual adress vram was map to
}

//Advice from professor: do get methods
uint16_t get_hres(){
	return h_res;
}
uint16_t get_vres(){
	return v_res;
}
void* get_vid_mem(){
	return video_mem;
}
uint8_t get_bits_per_pixel(){
	return bits_per_pixel;
}

/*struct reg86u is a struct with a union of structs
--->b is the member to access 8-bit registers
--->w is the member to access 16-bit registers
--->l is the member to access 32-bit registers
 The names of the members of the structs are the standard
names of IA-32 registers.*/
