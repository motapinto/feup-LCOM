#include <stdint.h>
#include "keyboard.h"
#include "ibm_8042.h"
#include <lcom/lcf.h>

#define UNS_FUNCTION_FAIL 2  //unsigned function fail

#ifdef LAB3
uint32_t sys_inb_count = 0;
int sys_inb_cnt(port_t port, uint32_t *byte){
  sys_inb_count++;
  return sys_inb(port, byte);
}
#endif

int kbd_hook_id;
uint8_t scan_code;

//IF BIT(7) is activated is a break code otherwise is a make code (0x01 vs 0x081 ESC codes)
int make_break(uint8_t scan_code)
{
  int make_break_code = ((scan_code & BIT(7)) >>7 );//=1 if bit 7(msb) is activated -> is a break code
  return make_break_code;//returns 1 if is a break code otherwise returns 0(make code)
}


int is_special_key(uint8_t scan_code){
  if(scan_code == SPECIAL_KEY1 || scan_code == SPECIAL_KEY2){//special key if scan_code = 0xe0 ou 0xe1
    return 1;
  }
  else
    return 0;
}


int kbd_subscribe_int(uint8_t *bit_no)
{
  kbd_hook_id = KBD_IRQ;//set to 1

  if(sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &kbd_hook_id) != OK){
    printf("Erro em sys_irqsetpolicy na funcao: 'kb_subscribe_int'\n");
  }

  *bit_no = BIT(KBD_IRQ);
  return 0;//nao da para mandar BIT(hook_id) pois este va ser alterado no set_policy
}


int kbd_unsubscribe_int()
{
  if(sys_irqrmpolicy(&kbd_hook_id) != OK){//unsubscribes previous subscription
    printf("Erro em sys_irqrmpolicy na funcao: 'kb_unsubscribe_int'\n");
    return -1;
  }
  return 0;
}


void (kbc_ih)(void)//return always 1 byte
{
  scan_code = kbd_read_outbuf();
}


int kbd_reset_status() {
  u32_t status;

  if(kbd_cmd(READ) == UNS_FUNCTION_FAIL){//read = 0x20
    printf("Erro em kbd_cmd() na funcao: 'kbd_reset_status'\n");
    return UNS_FUNCTION_FAIL;
  }

  status = kbd_read_outbuf();//reads what is in OUT_BUF

  if(kbd_cmd(WRITE)==UNS_FUNCTION_FAIL){ //write = 0x60
    printf("Erro em kbd_cmd() na funcao: 'kbd_reset_status'\n");
    return UNS_FUNCTION_FAIL;
  }
  if(kbd_write_outbuf(status | BIT(0) | BIT(1))==UNS_FUNCTION_FAIL){//aiva interrupcoes do mouse e keyboard
      printf("Erro em kbd_write_outbuf() na funcao: 'kbd_reset_status'\n");
      return UNS_FUNCTION_FAIL;
  }
  return 0;
}




u32_t kbd_cmd(u32_t cmd) {
	int contador = 5;
  u32_t status;

	while(contador >= 0) {
    sys_inb_cnt(KBD_STATUS_PORT, &status);
    if( (status & IBF) == 0){
        sys_outb(KBC_CMD_REG, cmd);
        return 0;
    }
		tickdelay(micros_to_ticks(DELAY_US));
    contador--;
	}
	return UNS_FUNCTION_FAIL;
}

u32_t kbd_write_outbuf(u32_t cmd) {
	int contador = 5;
  u32_t status;

	while(contador >= 0) {
    if(sys_inb_cnt(KBD_STATUS_PORT, &status)!=OK)
        return 2;
    if( (status & IBF) == 0){
        sys_outb(KBD_OUT_BUF, cmd);
        return 0;
    }
		tickdelay(micros_to_ticks(DELAY_US));
    contador--;
	}
	return UNS_FUNCTION_FAIL;
}


u32_t kbd_read_outbuf() {//Read Use KBC command 0x20, which must be written to 0x64 !! But the value of the “command byte” must be read from 0x60
	u32_t status, data;
	int contador = 5;

	while(contador >=0) {
		sys_inb_cnt(KBD_STATUS_PORT, &status);
		if(status & OBF) { //outbuf full -> can read
			sys_inb_cnt(KBD_OUT_BUF, &data);
      if((status & (PAR_ERR | TO_ERR)) == 0){
        return data;
      }
			else
				return 2;
		}
		tickdelay(micros_to_ticks(DELAY_US));
		contador--;
	}

	return UNS_FUNCTION_FAIL;
}
