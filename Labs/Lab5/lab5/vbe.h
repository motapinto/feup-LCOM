#pragma once

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


int vbe_get_info(uint16_t mode, vbe_mode_info_t *vmi_p);
int vbe_set_mode(uint16_t function, uint16_t mode);

uint32_t return_address(phys_bytes f_add, void* adress_init);
int vbe_get_control(vg_vbe_contr_info_t *vci_p, vbe_ctrl_info *info);
uint8_t get_bits_per_pixel();
