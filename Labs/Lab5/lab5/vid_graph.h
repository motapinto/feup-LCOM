#ifndef _LCOM_VID_GRAPH_H_
#define _LCOM_VID_GRAPH_H_
/*
#define PB2BASE(x) 					  (((x) >> 4) & 0x0F000)
#define PB2OFF(x)			  		  ((x) & 0x0FFFF)*/

#define VRAM_PHYS_ADDR 	      0xE0000000 //This address may depend on the VM used. So, I provide a program that allows you to find out this address
#define H_RES                 1024
#define V_RES		              768
#define BITS_PER_PIXEL	      8
#define MODEINFO_SIZE			    256
#define LINEAR_FBUF_BIT       1<<14 // set bit 14: linear framebuffer (always set)

#define VID_MOD_FUN           0X00
#define TEXT_MODE             0X03
#define GRAPHIC_MODE          0X105 //VBE graphics mode 105h

#define VBE_VID_BIOS_SERV       0x10 //basic  bios service (VIDEO CARD)
#define VBE_PC_CONF_BIOS_SERV   0x11 //basic  bios service (PC CONFIG)
#define VBE_MEM_CONF_SERV       0x12 //basic  bios service (MEMORY CONFIG)
#define VBE_KBD_BIOS_SERV       0x16 //basic  bios service (KEYBOARD)


#define GET_VBE_MODE          0x4F01// return VBE Mode Information
#define SET_VBE_MODE          0x4F02//Set VBE mod
#define CURRENT_VBE_MODE      0X4F03 //Return current VBE Mode //BX = Current VBE mode


#endif
