#include <stdbool.h>
#pragma once

enum mode{stream, remote};//use in function mouse_set_mode

int mouse_subscribe_int(uint32_t *bit_no);
int mouse_unsubscribe_int();

int mouse_data_report(bool condition);
int mouse_set_mode(enum mode mod);

int m_write_cmd(uint32_t port, uint32_t cmd);
int mouse_read_outbuf(uint8_t *data);
int m_read_cmd(uint32_t port, uint8_t *data);
int ack_check (uint8_t *check_byte);

int mouse_int(bool condition);

void (mouse_ih)(void);
void packet_store(uint32_t byte);

struct mouse_ev mouse_event_handler(struct packet *pp);

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
