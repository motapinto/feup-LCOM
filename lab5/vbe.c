#include <lcom/lcf.h>
#include <machine/int86.h>
#include "liblm.h"

#include "video_macros.h" //video graphics macros
#include "ibm_8042.h"
#include "vbe.h"

int vbe_get_info(uint16_t mode, vbe_mode_info_t *vmi_p) {
	mmap_t map;
	struct reg86u reg;
	phys_bytes buf;

	if (lm_alloc(sizeof(vbe_mode_info_t), &map) == NULL) //alloc memory needed
	{
		printf("Erro em lm_alloc\n");//reboot minix if there is problem or do retrys but may not solve
		return FUNCTION_FAIL;
	}

	memset(&reg, 0, sizeof(reg));
	buf = map.phys;

	reg.u.w.ax = GET_VBE_MODE; // VBE get mode info (0x4F01)
	reg.u.w.es = PB2BASE(buf); // PB2BASE Is a macro for computing the base of a segment, a 16-bit value, given a 32-bit linear address;
	reg.u.w.di = PB2OFF(buf); // PB2OFF Is a macro for computing the offset with respect to the base of a segment, a 16-bit value, given a 32-bit linear address;
	reg.u.w.cx = mode;
	reg.u.b.intno = VBE_VID_BIOS_SERV	;// (0x10)

	if (sys_int86(&reg) != OK) { // call BIOS
			printf("Erro em sys_int86()\n");
			lm_free(&map); //free memory
			return FUNCTION_FAIL;
	}

	if(reg.u.b.ah == 01 || reg.u.b.ah == 02 || reg.u.b.ah == 03){ // (return status)
		printf("\n\nErro no valor de reg86.u.b.ah (return status)\nErro: %x\n", reg.u.b.ah);
		lm_free(&map); //free memory
		return FUNCTION_FAIL;
	}

	memcpy(vmi_p, map.virt, map.size);
	lm_free(&map);//free memory
	return 0;
}

int vbe_get_control(vg_vbe_contr_info_t *vci_p, vbe_ctrl_info *info) {
	mmap_t map;
	struct reg86u reg;
	phys_bytes buf;
	*vci_p=*vci_p;

	if (lm_alloc(sizeof(vbe_ctrl_info), &map) == NULL) //alloc memory needed
	{
		printf("Erro em lm_alloc\n");//reboot minix if there is problem or do retrys but may not solve
		return FUNCTION_FAIL;
	}

	memset(&reg, 0, sizeof(reg));
	buf = map.phys;

	reg.u.w.ax = GET_VBE_CTRL_INFO; // VBE get mode info (0x4F00)
	reg.u.w.es = PB2BASE(buf); // PB2BASE Is a macro for computing the base of a segment, a 16-bit value, given a 32-bit linear address;
	reg.u.w.di = PB2OFF(buf); // PB2OFF Is a macro for computing the offset with respect to the base of a segment, a 16-bit value, given a 32-bit linear address;
	reg.u.b.intno = VBE_VID_BIOS_SERV	;// (0x10)

	if (sys_int86(&reg) != OK) { // call BIOS
			printf("Erro em sys_int86()\n");
			lm_free(&map); //free memory
			return FUNCTION_FAIL;
	}

	if(reg.u.b.ah == 01 || reg.u.b.ah == 02 || reg.u.b.ah == 03){ // (return status)
		printf("\n\nErro no valor de reg86.u.b.ah (return status)\nErro: %x\n", reg.u.b.ah);
		return FUNCTION_FAIL;
	}

	memcpy(info, map.virt, map.size);
	lm_free(&map);//free memory
	return 0;
}

int vbe_set_mode(uint16_t function, uint16_t mode) {
	struct reg86u reg;
	memset(&reg, 0, sizeof(reg));

	reg.u.w.ax = function; // VBE call, function 02 -- set VBE mode
	reg.u.w.bx = LINEAR_FBUF_BIT | mode; // set bit 14: linear framebuffer (always set)
	reg.u.b.intno = VBE_VID_BIOS_SERV	;// (0x10)

	if( sys_int86(&reg) != OK ) {
		printf("Erro em sys_int86()\n");
		return FUNCTION_FAIL;
	}

	if(reg.u.b.ah == 01 || reg.u.b.ah == 02 || reg.u.b.ah == 03){ // (return status)
		printf("\n\nErro no valor de reg86.u.b.ah (return status)\nErro: %x\n", reg.u.b.ah);
		return FUNCTION_FAIL;
	}
	return 0;
}

uint32_t return_address(phys_bytes f_add, void* adress_init) {
    uint32_t address = f_add;
    return ((address >> 16) * 0x10) + (address & 0x0FFFF) + (uint32_t)adress_init;
}
