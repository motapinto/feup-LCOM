#include <lcom/lcf.h>
#include <lcom/timer.h>
#include <stdint.h>
#include "i8254.h"

#define FUNCTION_FAIL -2//quando uma funcao da erro retorna -2
#define PARAMETROS_FAIL -3//quando ha erro nos parametros, valores impossiveis retorna -3

int timer_hook_id;//variavel global para puder ser acessivel
uint32_t contador = 0;//variavel global para poder ser acedida fora de time_handler

//define max frquencia as 19
int (timer_set_frequency)(uint8_t timer, uint32_t freq) {

  if (timer < 0 || timer > 2 || freq <= 0) {
    printf("Valor do timer errado E freq tem ser ser >0\n");
    return PARAMETROS_FAIL;
}
  uint8_t st, lsb, msb;

  if(timer_get_conf(timer, &st)!=0){//ve a configuaracao de timer
    printf("Erro em timer_get_conf na funcao: 'timer_set_frequency'\n");
    return FUNCTION_FAIL;
  }

  st = st & 0x0F;//mantem os 4 lsb para enviar para o controlo
  u32_t control_byte = TIMER_LSB_MSB | st;//adiciona o modo LSB_MSB
  freq = TIMER_FREQ/freq;

  if (timer == 0){//bit 6 and 7 already 0
    //nao precisa de fazer nada --->> bit 6 and bit 7 = 0
  }
  else if(timer == 1){
    control_byte |= BIT(6);//ativa o bit 6
  }
  else if(timer ==2){
    control_byte |= BIT(7);//ativa bit 7
  }

  if (sys_outb(TIMER_CTRL, control_byte)!=OK) {
    printf("Erro em sys_outb na funcao: 'timer_set_frequency'\n");
    return FUNCTION_FAIL;
  }

  if(util_get_LSB((uint16_t)freq, &lsb)!=0){
    printf("Erro em until_get_LSB na funcao: 'timer_set_frequency'\n");
    return FUNCTION_FAIL;
  }

  if(sys_outb(TIMER_0+timer, lsb)!=OK){
    printf("Erro em sys_outb na funcao: 'timer_set_frequency'\n");
    return FUNCTION_FAIL;
  }

  if(util_get_MSB((uint16_t)freq, &msb)!=0){
    printf("Erro em until_get_MSB na funcao: 'timer_set_frequency'\n");
    return FUNCTION_FAIL;
  }

  if(sys_outb(TIMER_0+timer, msb)!=OK){
    printf("Erro em sys_outb na funcao: 'timer_set_frequency'\n");
    return FUNCTION_FAIL;
  }
  return 0;
}



int (timer_subscribe_int)(uint8_t *bit_no) {
  uint8_t temp=TIMER0_IRQ;
  timer_hook_id = temp; //hook_id = TIMER0_IRQ

  if(sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &timer_hook_id) != OK){
    printf("Erro em sys_irqsetpolicy na funcao: 'timer_subscribe_int'\n");
    if(sys_irqenable(&timer_hook_id) != OK){//se a set policy der erro
      printf("Erro em sys_irqenable na funcao: 'timer_subscribe_int'\n");
      return -1;
    }
  }

  *bit_no = BIT(temp);//bit mask
  return 0;
}



int (timer_unsubscribe_int)() {
  if(sys_irqdisable (&timer_hook_id) !=OK){//disables interrupts
    printf("Erro em sys_irqdisable na funcao: 'timer_unsubscribe_int'\n");
    return FUNCTION_FAIL;
  }
  if(sys_irqrmpolicy(&timer_hook_id) != OK){//unsubscribes previous subscription
    printf("Erro em sys_irqrmpolicy na funcao: 'timer_unsubscribe_int'\n");
    return FUNCTION_FAIL;
  }
  return 0;
}



void (timer_int_handler)() {
    contador++;//incrementa contador global que conta o n de interrupcoes
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
    if(timer < 0 || timer >2 || st == NULL){
      printf("st vazia ou valor do timer errado!\n");
      return PARAMETROS_FAIL;
    }

    u32_t status;
    u32_t control_byte = TIMER_RB_COUNT_ | TIMER_RB_CMD | TIMER_RB_SEL(timer);//control_byte para o controlo
    //manda para o control a informaçao pedida
    if(sys_outb(TIMER_CTRL, control_byte) != OK){
      printf("Erro em sys_outb() na funcao: 'timer_get_conf'\n");
      return PARAMETROS_FAIL;
    }
    //escreve em st os primeiros 8 bits do timer 0
    if(sys_inb(TIMER_0 + timer, &status)!=OK){
      printf("Erro em sys_inb() na funcao: 'timer_get_conf'\n");
      return PARAMETROS_FAIL;
    }

    *st = (uint8_t) status;//devolve a configuracao de timer para st

    return 0;
}


int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {

  if(timer_get_conf(timer, &st)!=0){//primeiro faz get_conf
    printf("Erro em timer_get_conf na funcao: 'timer_display_conf'\n");
    return FUNCTION_FAIL;
  }
  if(timer<0 || timer>2){//ve se os parametros estao corretos
    printf("Valor do timer errado \n");
    return PARAMETROS_FAIL;
  }


  union timer_status_field_val status;
  uint8_t comp;

  if(field == all){// st corresponde a um byte (8 bits)
    status.byte = st;
  }

  else if(field == initial){//inicialização

    comp = st >> 4 & 3 ;//desloca os 4 msb para a direita e faz a comparacao de todas as possibilidades(9-3 timers*3 configuaracoes)

    if(comp == 2){ // 0010 ou 0110 ou 1010 ->> msb pois bit 5=1 e bit6=0 ...
      status.in_mode = MSB_only;
    }
    else if(comp == 1){
      status.in_mode = LSB_only;
    }
    else if(comp == 3){
      status.in_mode = MSB_after_LSB;
    }
    else{
      status.in_mode =  INVAL_val;
    }
  }

  else if(field == mode){
    comp = (st & (BIT(3) | BIT(2) | BIT(1))) >> 1;//comp-comparacao; desloca os bits para a direita e ve a que numeros corresponde da tab 1

    if (comp == 2 || comp == 6) {//...0010/...0110
      status.count_mode = 2;
    }
    else if (comp == 3 || comp == 7) {//...0011/...0111
      status.count_mode = 3;
    }
    else if(comp == 1) {
      status.count_mode = 1;
    }
    else if(comp == 0) {
      status.count_mode = 0;
    }
    else if(comp == 4) {
      status.count_mode = 4;
    }
    else if(comp == 5) {
      status.count_mode = 5;
    }
  }

  else if(field == base){
    if((st & BIT(0)) == 1){//and com 00000001
      status.bcd = 1;
    }
    else
      status.bcd = 0;
  }

  if(timer_print_config(timer, field, status)!=OK){//imprime a configuracao
    printf("Erro em timer_print_config na funcao: 'timer_display_conf'\n");
    return FUNCTION_FAIL;
  }
  return 0;
}
