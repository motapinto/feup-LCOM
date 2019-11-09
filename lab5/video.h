#ifndef __VIDEO_GR_H
#define __VIDEO_GR_H


void pixel_set(uint16_t x, uint16_t y, uint32_t color);
int draw_ret(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);
int draw_pat(uint8_t no_rect, uint32_t first, uint8_t step,
                  vbe_mode_info_t vbe_mode);
void disassemble_rgb(uint32_t *color, uint32_t *red, uint32_t *green,
                    uint32_t *blue, vbe_mode_info_t vbe_mode);
void assemble_rgb(uint32_t *color, uint32_t *red, uint32_t *green,
                    uint32_t *blue, vbe_mode_info_t vbe_mode);


/*
Returns the address of the allocated memory where the sprite was read.
Updates "width" and "heigh" with the sprite dimension. Return NULL on error.
 Assumes that VRES and HRES (the screen vertical and horizontal resolution)
 are externaly defined.
 */
int xpm_draw(const char *xpm[], uint16_t x, uint16_t y);
int xpm_destroy(const char *xpm[], uint16_t x, uint16_t y);


int draw_move_not_speedy(const char *xpm[], uint16_t x, uint16_t y,
  uint16_t xf, uint16_t yf, int16_t speed, uint8_t fr_rate);
int draw_move_speedy(const char *xpm[], uint16_t x, uint16_t y,
  uint16_t xf, uint16_t yf, int16_t speed,  uint8_t fr_rate);


uint16_t get_hres();
uint16_t get_vres();
void* get_vid_mem();

/**
 * @brief Initializes the video module in graphics mode
 *
 * Uses the VBE INT 0x10 interface to set the desired
 *  graphics mode, maps VRAM to the process' address space and
 *  initializes static global variables with the resolution of the screen,
 *  and the number of colors
 *
 * @param mode 16-bit VBE mode to set
 * @return Virtual address VRAM was mapped to. NULL, upon failure.
 */
//void *vg_init(uint16_t mode);

 /**
 * @brief Returns to default Minix 3 text mode (0x03: 25 x 80, 16 colors)
 *
 * @return 0 upon success, non-zero upon failure
 */
//int vg_exit();



#endif /* __VIDEO_GR_H */
