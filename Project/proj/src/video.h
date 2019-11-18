#ifndef __VIDEO_H
#define __VIDEO_H

/** @defgroup video video
 * @{
 *
 */

#include <stdint.h>
#include "vbe.h"

// Because of LCF this functions must be declared in this header file,
// but are written in video_gr.c

/**
 * @brief Returns hres
 */
 uint16_t get_hres();
/**
 * @brief Returns vres
 */
 uint16_t get_vres();
/**
 * @brief Returns video memory
 */
 void* get_vid_mem();
/**
 * @brief Returns double buffer
 */
 char* get_double_buffer();
/**
 * @brief Returns a third buffer that is used to detect collisions
 */
 unsigned char *get_rd_buffer();
/**
 * @brief Copies memory from double buffer were all images were draw to video_mem
 */
 void page_flipping();

#endif /* __VIDEO_GR_H */
