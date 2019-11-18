#pragma once

#include "liblm.h"
#include <lcom/lcf.h>
#include <machine/int86.h>

/** @defgroup vbe vbe
 * @{
 *
 */

#pragma pack(1)
typedef struct {
	uint8_t VbeSignature[4];
	uint16_t VbeVersion;
	phys_bytes OemStringPtr;
	uint8_t Capabilities[4];
	phys_bytes VideoModePtr;
	uint16_t TotalMemory;

	uint16_t OemSoftwareRev;
	phys_bytes OemVendorNamePtr;
	phys_bytes OemProductNamePtr;
	phys_bytes OemProductRevPtr;
	uint8_t Reserved[222];
	uint8_t OemData[256];
} vbe_ctrl_info;
#pragma options align=reset

/**
 * @brief Returns information on the VBE mode
 *
 * @param mode information about mode must be returned
 * @param vmi_p address of vbe_mode_info_t structure to be initialized
 * @return 0 on success, non-zero on fail
 */
int vbe_get_info(uint16_t mode, vbe_mode_info_t *vmi_p);
/**
 * @brief Function 02 of VBE -- set VBE mode and attributes
 * to reg.u.w.bx the mode specified in mode
 *
 * @param function Function to be set to reg.u.w.ax
 * @param mode Mode to be set to reg.u.w.bx
 *
 * @return 0 on success, non-zero on fail
 */
int vbe_set_mode(uint16_t function, uint16_t mode);
/**
 * @brief Returns bits per pixel
 */
uint8_t get_bits_per_pixel();
