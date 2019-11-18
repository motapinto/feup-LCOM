#pragma once


struct packet {
    uint8_t bytes[3];
    bool rb;
    bool mb;
    bool lb;
    int16_t delta_x;
    int16_t delta_y;
    bool x_ov;
    bool y_ov;
};

void mouse_print_packet(struct packet *pp);
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
