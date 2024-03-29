#include <lcom/lcf.h>
#include <stdint.h>
#include "keyboard.h"
#include "ibm_8042.h"

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

int kbd_read_outbuf(uint8_t *value) {//Read Use KBC command 0x20, which must be written to 0x64 !! But the value of the “command byte” must be read from 0x60
	uint32_t status, data;
	int contador = 5;

	while(contador >=0) {
		sys_inb(KBD_STATUS_PORT, &status);
		if(status & OBF) { //outbuf full -> can read
			sys_inb(KBD_OUT_BUF, &data);
      if((status & (PAR_ERR | TO_ERR)) == 0){
        *value = data;
        return 0;
      }
			else
				return FUNCTION_FAIL;
		}
		tickdelay(micros_to_ticks(DELAY_US));
		contador--;
	}

	return FUNCTION_FAIL;
}

int kbd_write_inbuf(uint32_t cmd) {
	int contador = 5;
  uint32_t status;

	while(contador >= 0) {
    if(sys_inb(KBD_STATUS_PORT, &status)!=OK)
        return FUNCTION_FAIL;
    if( (status & IBF) == 0){
        sys_outb(KBD_IN_BUF, cmd);
        return 0;
    }
		tickdelay(micros_to_ticks(DELAY_US));
    contador--;
	}
	return FUNCTION_FAIL;
}

void (kbc_ih)(void)//return always 1 byte
{
  kbd_read_outbuf(&scan_code);
}

int kbd_reset_status() {
  uint8_t status;

  if(kbd_cmd(READ) != 0){//read = 0x20
    printf("Erro em kbd_cmd() na funcao: 'kbd_reset_status'\n");
    return FUNCTION_FAIL;
  }

  kbd_read_outbuf(&status);//reads what is in OUT_BUF

  if(kbd_cmd(WRITE) != 0){ //write = 0x60
    printf("Erro em kbd_cmd() na funcao: 'kbd_reset_status'\n");
    return FUNCTION_FAIL;
  }
  if(kbd_write_inbuf(status | BIT(0) | BIT(1)) != 0){//aiva interrupcoes do mouse e keyboard
      printf("Erro em kbd_write_outbuf() na funcao: 'kbd_reset_status'\n");
      return FUNCTION_FAIL;
  }
  return 0;
}

int kbd_cmd(uint32_t cmd) {
	int contador = 5;
  uint32_t status;

	while(contador >= 0) {
    sys_inb(KBD_STATUS_PORT, &status);
    if( (status & IBF) == 0){
        sys_outb(KBC_CMD_REG, cmd);
        return 0;
    }
		tickdelay(micros_to_ticks(DELAY_US));
    contador--;
	}
	return FUNCTION_FAIL;
}
