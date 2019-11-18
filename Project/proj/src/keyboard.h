#pragma once
#include <stdint.h>
#include <stdbool.h>

/** @defgroup keyboard keyboard
 * @{
 *
 */

#ifdef LAB3
int sys_inb_cnt(port_t port, uint32_t *byte);
#else
#define sys_inb_cnt(p,q) sys_inb(p,q)
#endif

/**
 * @brief Ckecks if a given scan code is make or a break code
 *
 * @param scan_code scan code to be checked
 * @return Returns 1 if is a break code otherwise returns 0(make code)
 */
int make_break(uint8_t scan_code);
/**
 * @brief Ckecks if a given scan code is a special code
 *
 * @param scan_code scan code to be checked
 * @return Return 1 if is a special key and 0 otherwise
 */
int is_special_key(uint8_t scan_code);
/**
 * @brief Subscribes to Keyboard interrupts
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int kbd_subscribe_int(uint8_t *bit_no);
/**
 * @brief Unsubscribes to Keyboard interrupts
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int kbd_unsubscribe_int();
/**
 * @brief Keyboard interrupt handler
 */
void (kbc_ih)(void);
/**
 * @brief Resets status to enable interruptions
 */
int kbd_reset_status();//resets status to enable interrupts
/**
 * @brief Sends a comand cmd to KBD_STATUS_PORT
 * @param cmd command to be send
 */
int kbd_cmd(uint32_t cmd);
/**
 * @brief Writes a command cmd to KBD_IN_BUF
 *
 * @param cmd command to be written
 * @return Return 0 upon success and non-zero otherwise
 */
int kbd_write_inbuf(uint32_t cmd);
/**
 * @brief Reads from the outbuf
 *
 * @param data value read from outbuf returned as a pointer
 * @return Return 0 upon success and non-zero otherwise
 */
int kbd_read_outbuf(uint8_t *value);
