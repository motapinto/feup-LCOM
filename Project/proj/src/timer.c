#include <lcom/lcf.h>
#include <lcom/timer.h>
#include <stdint.h>
#include "ibm_8042.h"
#include "i8254.h"

int timer_hook_id;     //variavel global para puder ser acessivel
uint32_t contador; //variavel global para poder ser acedida fora de time_handler

//define max frquencia as 19
int (timer_frequency_set)(uint8_t timer, uint32_t freq) {

  if (timer < 0 || timer > 2 || freq <= 0) {
    printf("Valor do timer errado E freq tem ser ser >0\n");
    return PARAMETROS_FAIL;
  }
  uint8_t st, lsb, msb;

  if (timer_get_conf(timer, &st) != 0) { //ve a configuaracao de timer
    printf("Erro em timer_get_conf na funcao: 'timer_set_frequency'\n");
    return FUNCTION_FAIL;
  }

  st = st & 0x0F;                          //mantem os 4 lsb para enviar para o controlo
  uint32_t control_byte = TIMER_LSB_MSB | st; //adiciona o modo LSB_MSB
  freq = TIMER_FREQ / freq;

  if (timer == 0) { //bit 6 and 7 already 0
    //nao precisa de fazer nada --->> bit 6 and bit 7 = 0
  }
  else if (timer == 1) {
    control_byte |= BIT(6); //ativa o bit 6
  }
  else if (timer == 2) {
    control_byte |= BIT(7); //ativa bit 7
  }

  if (sys_outb(TIMER_CTRL, control_byte) != 0) {
    printf("Erro em sys_outb na funcao: 'timer_set_frequency'\n");
    return FUNCTION_FAIL;
  }

  if (util_get_LSB((uint16_t) freq, &lsb) != 0) {
    printf("Erro em until_get_LSB na funcao: 'timer_set_frequency'\n");
    return FUNCTION_FAIL;
  }

  if (sys_outb(TIMER_0 + timer, lsb) != 0) {
    printf("Erro em sys_outb na funcao: 'timer_set_frequency'\n");
    return FUNCTION_FAIL;
  }

  if (util_get_MSB((uint16_t) freq, &msb) != 0) {
    printf("Erro em until_get_MSB na funcao: 'timer_set_frequency'\n");
    return FUNCTION_FAIL;
  }

  if (sys_outb(TIMER_0 + timer, msb) != 0) {
    printf("Erro em sys_outb na funcao: 'timer_set_frequency'\n");
    return FUNCTION_FAIL;
  }
  return 0;
}

int (timer_subscribe_int)(uint8_t *bit_no) {

  timer_hook_id = 0; //hook_id = TIMER0_IRQ

  if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &timer_hook_id) != OK) {
    printf("Erro em sys_irqsetpolicy na funcao: 'timer_subscribe_int'\n");
    if (sys_irqenable(&timer_hook_id) != 0) { //se a set policy der erro
      printf("Erro em sys_irqenable na funcao: 'timer_subscribe_int'\n");
      return -1;
    }
  }

  *bit_no = BIT(0); //bit mask
  return 0;
}

int(timer_unsubscribe_int)() {
  if (sys_irqdisable(&timer_hook_id) != 0) { //disables interrupts
    printf("Erro em sys_irqdisable na funcao: 'timer_unsubscribe_int'\n");
    return FUNCTION_FAIL;
  }
  if (sys_irqrmpolicy(&timer_hook_id) != 0) { //unsubscribes previous subscription
    printf("Erro em sys_irqrmpolicy na funcao: 'timer_unsubscribe_int'\n");
    return FUNCTION_FAIL;
  }
  return 0;
}

/*void(timer_int_handler)() {
  contador++; //incrementa contador global que conta o n de interrupcoes
} */

int timer_sleep(uint8_t time) {

  message msg;
  int ipc_status, r;
  unsigned int contador_seg = 0; //conta os segundo e o global conta os interrupts
  uint8_t timer_irq_set;

  if (timer_subscribe_int(&timer_irq_set) != 0) {
    printf("Erro em timer_subscribe_int() ou em timer_set_frequency na funcao 'timer_test_int'\n");
  }
  while (contador_seg < time) {
    // Get a request message.
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { // received notification! /
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:                                   // hardware interrupt notification /
          if (msg.m_notify.interrupts & timer_irq_set) { // subscribed interrupt /
            timer_int_handler();                           //incrementa contador que conta o numero de interrupcoes (60 por segundo)
            if (contador % 60 == 0) {                    //se o resto da divisao de contador por 60 e 0 passaram nsegundos
              contador_seg++;                               //contador que conta os segundos incrementa
            }
          }
          break;
        default:
          break; // no other notifications expected: do nothing /
      }
    }
    else { // received a standard message, not a notification /
           // no standard messages expected: do nothing */
    }
  }
  return 0;
}

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  *lsb = (uint8_t) val & 0xFF; // and COM 1111 1111 (MANTEM ULTIMOS 8 BITS)
  return 0;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  *msb = val >> 8; //FICA COM
  return 0;
}
