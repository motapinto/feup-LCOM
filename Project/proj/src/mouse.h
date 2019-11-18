#include <stdbool.h>
#pragma once

/** @defgroup mouse mouse
 * @{
 *
 */

enum mode{stream, remote};//use in function mouse_set_mode

/**
 * @brief Subscribes to mouse interruptions
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int mouse_subscribe_int(uint32_t *bit_no);
/**
 * @brief Unsubscribes to mouse interruptions
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int mouse_unsubscribe_int();
/**
 * @brief Enale/Disable data reporting
 *
 * @param condition if true enables data report, otherwise disables it
 * @return Return 0 upon success and non-zero otherwise
 */
int mouse_data_report(bool condition);
/**
 * @brief Sets to stream/remote mode
 *
 * @param mod if stream sets to stream, if remote sets to remote
 * @return Return 0 upon success and non-zero otherwise
 */
int mouse_set_mode(enum mode mod);
/**
 * @brief Writes a command cmd to  port
 *
 * @param cmd command to be written
 * @param port port which to be written to
 * @return Return 0 upon success and non-zero otherwise
 */
int m_write_cmd(uint32_t port, uint32_t cmd);
/**
 * @brief Reads from the outbuf
 *
 * @param data value read from outbuf returned as a pointer
 * @return Return 0 upon success and non-zero otherwise
 */
int mouse_read_outbuf(uint8_t *data);
/**
 * @brief Reads from port to data
 *
 * @param data value read from port returned as a pointer
 * @param port port that is going to be read
 * @return Return 0 upon success and non-zero otherwise
 */
int m_read_cmd(uint32_t port, uint8_t *data);
/**
 * @brief After writing to inbuf checks ack/nack/error
 *
 * @param check_byte value read from outbuf
 * @return Return 0 upon success and non-zero otherwise
 */
int ack_check (uint8_t *check_byte);
/**
 * @brief Enale/Disable mouse interruptions
 *
 * @param condition if true mouse interruptions, otherwise disables it
 * @return Return 0 upon success and non-zero otherwise
 */
int mouse_int(bool condition);
/**
 * @brief Mouse interrupt handler
 */
void (mouse_ih)(void);
/**
 * @brief Receives a byte and assembles the mouse packet
 *
 * @param byte value read grom outbuf to be assembled in mouse packet
 */
void packet_store(uint32_t byte);

/*
Data Fields
uint8_t 	bytes [3]
 	mouse packet raw bytes

bool 	rb
 	right mouse button pressed

bool 	mb
 	middle mouse button pressed

bool 	lb
 	left mouse button pressed

int16_t 	delta_x
 	mouse x-displacement: rightwards is positive

int16_t 	delta_y
 	mouse y-displacement: upwards is positive

bool 	x_ov
 	mouse x-displacement overflow

bool 	y_ov
 	mouse y-displacement overflow
*/
