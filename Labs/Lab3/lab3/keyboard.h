#pragma once
#include <minix/syslib.h>
#include <stdint.h>

#ifdef LAB3
int sys_inb_cnt(port_t port, uint32_t *byte);
#else
#define sys_inb_cnt(p,q) sys_inb(p,q)
#endif

//IF BIT(7) is activated is a break code otherwise is a make code (0x01 vs 0x081 ESC codes)
//=1 if bit 7(msb) is activated -> is a break code
//returns 1 if is a break code otherwise returns 0(make code)
int make_break(uint8_t scan_code);

//return 1 is a scan_code is correspondent with a special key
int is_special_key(uint8_t scan_code);

/**
 * @brief Subscribes and enables Keyboard interrupts
 *
 * @param bit_no address of memory to be initialized with the
 *         bit number to be set in the mask returned upon an interrupt
 * @return Return 0 upon success and non-zero otherwise
 */

int kbd_subscribe_int(uint8_t *bit_no);
int kbd_unsubscribe_int();

void (kbc_ih)(void);
void kbc_asm_ih();//assembly interrupt handler

int kbd_reset_status();//resets status to enable interrupts
uint32_t kbd_cmd(uint32_t cmd);
uint32_t kbd_read_outbuf();
uint32_t kbd_write_outbuf(uint32_t cmd);
