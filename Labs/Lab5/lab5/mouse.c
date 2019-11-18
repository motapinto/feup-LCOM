#include <lcom/lcf.h>
#include <stdint.h>
#include <stdbool.h>
#include <mouse.h>
#include "ibm_8042.h"


extern u32_t scan_code;//from lab3 e keyboard
int packet_store_counter = 0; //to use in mouse_ih
int mouse_hook_id;
struct packet pp;

int mouse_subscribe_int(uint32_t *bit_no) {
	mouse_hook_id = MOUSE_IRQ_LINE; //mouse_irq_line = 4 (4a linha do pic2)
	if(sys_irqsetpolicy(MOUSE_IRQ , IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_hook_id)!=OK){//mouse_irq = 12
		printf("Erro em sys_irqsetpolicy() na funcao: 'mouse_subscribe_int'\n");
		return FUNCTION_FAIL;
	}
	*bit_no = BIT(MOUSE_IRQ_LINE);
	return 0;
}

int mouse_unsubscribe_int() {
	if(sys_irqrmpolicy(&mouse_hook_id)!=OK){
		printf("Erro em sys_irqrmpolicy() na funcao: 'mouse_unsubscribe_int'\n");
		return FUNCTION_FAIL;
		}
	return 0;
}

int mouse_int(bool condition){// bit aux ver verificacao par aver se o que sai e do mouse
	if(condition == true){
		if(sys_irqenable(&mouse_hook_id)!=OK){//enables interruptions
			printf("Erro em sys_irqenable() na funcao: 'mouse_int'\n");
			return FUNCTION_FAIL;
		}
	}

	else{
		if(sys_irqdisable(&mouse_hook_id)!=OK){//disables interruptions
			printf("Erro em sys_irqdisable() na funcao: 'mouse_int'\n");
			return FUNCTION_FAIL;
		}
	}
	return 0;
}

int mouse_data_report(bool condition){
	uint8_t check_byte;
	int result;

	if(condition == true){
		if(m_write_cmd(KBD_STATUS_PORT, ENABLE_MOUSE) != 0){
			printf("Erro em stream mode ao fazer write to mouse\n");
			return FUNCTION_FAIL;
		}
		if(m_write_cmd(KBD_STATUS_PORT, WRITE_TO_MOUSE) != 0){
			printf("Erro em data report enable ao fazer write to mouse\n");
			return FUNCTION_FAIL;
		}
		if(m_write_cmd(KBD_IN_BUF, ENABLE_REP) != 0){
			printf("Erro em data report enable ao fazer enable stream mode\n");
			return FUNCTION_FAIL;
		}
	}

	else{
		if(m_write_cmd(KBD_STATUS_PORT, ENABLE_MOUSE) != 0){
			printf("Erro em stream mode ao fazer write to mouse\n");
			return FUNCTION_FAIL;
		}
		if(m_write_cmd(KBD_STATUS_PORT, WRITE_TO_MOUSE) != 0){
			printf("Erro em data report disable ao fazer write to mouse\n");
			return FUNCTION_FAIL;
		}
		if(m_write_cmd(KBD_IN_BUF, DISABLE_REP) != 0){
			printf("Erro em data report disable ao fazer disable stream mode\n");
			return FUNCTION_FAIL;
		}
	}

	result = ack_check(&check_byte);//ver ack sempre que se inscreve no inbuf comandos do mouse
	if(result == NACK){//retry
		mouse_data_report(condition);
	}
	else if(result == ERROR){//exit
		return FUNCTION_FAIL;
	}
	return 0;
}

int mouse_set_mode(enum mode mod){
	uint8_t check_byte=0;
	int result;

	if(mod == stream){
		if(m_write_cmd(KBD_STATUS_PORT, ENABLE_MOUSE) != 0){
			printf("Erro em stream mode ao fazer mouse enable\n");
			return FUNCTION_FAIL;
		}
		if(m_write_cmd(KBD_STATUS_PORT, WRITE_TO_MOUSE) != 0){
			printf("Erro em stream mode ao fazer write to mouse\n");
			return FUNCTION_FAIL;
		}
		if(m_write_cmd(KBD_IN_BUF, SET_STREAM ) != 0){
			printf("Erro em stream mode ao fazer enable stream mode\n");
			return FUNCTION_FAIL;
		}
	}

	else if (mod == remote){
		if(m_write_cmd(KBD_STATUS_PORT, ENABLE_MOUSE) != 0){
			printf("Erro em stream mode ao fazer mouse enable\n");
			return FUNCTION_FAIL;
		}
		if(m_write_cmd(KBD_STATUS_PORT, WRITE_TO_MOUSE) != 0){
			printf("Erro em remote mode fazer write to mouse\n");
			return FUNCTION_FAIL;
		}
		if(m_write_cmd(KBD_IN_BUF, SET_REMOTE ) != 0){
			printf("Erro em remote mode ao fazer disable stream mode\n");
			return FUNCTION_FAIL;
		}
	}

	else{
		return PARAMETROS_FAIL;
	}

	result = ack_check(&check_byte); //ver ack sempre que se inscreve no inbuf comandos do mouse
	if(result == NACK){//retry
		mouse_set_mode(mod);
	}
	else if(result == ERROR){//exit
		return FUNCTION_FAIL;
	}

	return 0;
}

int m_write_cmd(u32_t port, u32_t cmd) {//funcao bem sucedida
	u32_t status;
	int contador=5;

	while (contador > 0) {
		sys_inb(KBC_CMD_REG, &status);
		if (!(status & IBF)) {//INBUF NOT FULL ->ready to write
			if(sys_outb(port, cmd) == 0){
				return 0;
			}
		}
		tickdelay(micros_to_ticks(DELAY_US));
		contador--;
	}
	return TIME_OUT;
}


int mouse_read_outbuf(uint8_t *data) {//funcao bem definida
	u32_t status;
	int contador=5;
	u32_t data_32;

	while (contador > 0) {
		sys_inb(KBC_CMD_REG, &status);
		if (status & OBF) {//OUTBUF FULL ->ready to read
			sys_inb(KBD_OUT_BUF, &data_32);
		}
			if ((status & (PAR_ERR | TO_ERR)) == 0){
				*data = (uint8_t)data_32;
				return 0;
		 }
			else
				return FUNCTION_FAIL;
		tickdelay(micros_to_ticks(DELAY_US));
		contador--;
	}
	return TIME_OUT;
}

int m_read_cmd(uint32_t port, uint8_t *data) {
	u32_t status;
	int contador=5;
	u32_t data_32;

	while (contador > 0) {
		if(sys_inb(KBC_CMD_REG, &status)!=OK){
			return FUNCTION_FAIL;
		}
		if (status & OBF) {//OUTBUF FULL ->ready to read
			if(sys_inb(port, &data_32)!= OK ){
				return FUNCTION_FAIL;
			}
			if ((status & (PAR_ERR | TO_ERR)) == 0){
				*data = (uint8_t)data_32;
				return 0;
		 }
			else
				return FUNCTION_FAIL;
		}
		tickdelay(micros_to_ticks(DELAY_US));
		contador--;
	}
	return TIME_OUT;
}

int ack_check (uint8_t *check_byte)
{
	while(*check_byte != NACK && *check_byte!=ACK && *check_byte!=ERROR){
		m_read_cmd(KBD_OUT_BUF, check_byte);
		if(*check_byte == ACK){
			return ACK;
		}
    else if(*check_byte == NACK){
			printf("NACK received... Retrying...\n");
			return NACK;
		}
		if(*check_byte == ERROR){
			printf("ERROR received... Exiting\n");
			return ERROR;
		}
	}
	return 0;
}

void (mouse_ih) (){
	//kbc_ih(); // read scan_code(global var) "Try to use kbc_ih() from Lab 3"
	uint8_t data=0x00;
	mouse_read_outbuf(&data);
	packet_store(data);
}

void packet_store(uint32_t byte)
{
		if(packet_store_counter>2){
			packet_store_counter = 0;
		}

		if(packet_store_counter == 0 && ((byte & BIT(3))==BIT(3)) ){//Hint Bit 3 of first byte of a packet is always set
			pp.bytes[0] = byte;
			if(pp.bytes[0] & MOUSE_MB)
				pp.mb = true;
			else
				pp.mb = false;

			if(pp.bytes[0] & MOUSE_RB)
				pp.rb = true;
			else
				pp.rb = false;

			if(pp.bytes[0] & MOUSE_LB)
				pp.lb = true;
			else
				 pp.lb = false;

			if(pp.bytes[0] & MOUSE_Y_OV)
				pp.y_ov = true;
			else
				pp.y_ov = false;

			if(pp.bytes[0] & MOUSE_X_OV)
				pp.x_ov = true;
			else
				pp.x_ov = false;

			if(pp.bytes[0] & MOUSE_X_SIGN)
				pp.delta_x = 0xff00;
			else
				pp.delta_x = 0x0000;

				if(pp.bytes[0] & MOUSE_Y_SIGN)
					pp.delta_y = 0xff00;
				else
					pp.delta_y = 0x0000;
		}

		if(packet_store_counter == 1){
			pp.bytes[1] = byte;
			pp.delta_x |= byte;
		}

		if(packet_store_counter == 2){
			pp.bytes[2] = byte;
			pp.delta_y |= byte;
		}
		packet_store_counter++;//counter to keep track of byte number
}
struct mouse_ev mouse_event_handler(struct packet *pp) {
    static bool lb_pressed = false, rb_pressed = false, mb_pressed = false;
    struct mouse_ev event;
    if (pp->lb != lb_pressed) {
        if (pp->lb && !rb_pressed && !mb_pressed){ //lb pressed only if other buttons are not pressed
					event.type = LB_PRESSED;//atualiza event type
				}
        else if (pp->lb || rb_pressed || mb_pressed) {
					event.type = BUTTON_EV;
				}
        else {
					event.type = LB_RELEASED;
				}
        lb_pressed = pp->lb; //atualiza bool, pp->lb is a bool
    }

    else if (pp->rb != rb_pressed) {
        if (pp->rb && !lb_pressed && !mb_pressed){
					event.type = RB_PRESSED;
				}
        else if (pp->rb || lb_pressed || mb_pressed){
					event.type = BUTTON_EV;
				}
        else {
					event.type = RB_RELEASED;
				}
        rb_pressed = pp->rb; //atualiza bool
    }
    else if (pp->mb != mb_pressed) {
        event.type = BUTTON_EV;
        mb_pressed = pp->mb;
    }
    else if (pp->delta_x || pp->delta_y) {//se alguns dos delta != 0 ->mouse move
        event.type = MOUSE_MOV;
        event.delta_x = pp->delta_x;
        event.delta_y = pp->delta_y;
    }
    else {
			event.type = BUTTON_EV;
		}
    return event;
}
