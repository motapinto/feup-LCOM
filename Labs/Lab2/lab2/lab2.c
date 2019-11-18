#include "i8254.h"
#include <lcom/lab2.h>
#include <lcom/lcf.h>
#include <lcom/timer.h>
#include <stdbool.h>
#include <stdint.h>

extern uint32_t contador;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need/ it]
  lcf_trace_calls("/home/lcom/labs/lab2/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab2/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(timer_test_read_config)(uint8_t timer, enum timer_status_field field) {
  uint8_t st;
  if (timer_get_conf(timer, &st) != 0) { //vai buscar a config e ve se da erro
    return -1;                           //erro em get_conf
  }

  if (timer_display_conf(timer, st, field) != 0) { //se a get_conf nao der erro imprime a conf
    return -1;                                     //erro em display conf
  }
  return 0; //retorna 0 apos sucesso
}

int(timer_test_time_base)(uint8_t timer, uint32_t freq) {
  if (timer_set_frequency(timer, freq) != 0) { //ocorreu um erro se nao retornar 0
    return -1;
  }
  return 0;
}

int(timer_test_int)(uint8_t time) { //print one mess per second with duration = time

  message msg;
  int ipc_status, r;
  uint8_t bit_no = 0;
  int contador1 = 0; //conta os segundo e o global conta os interrupts

  if (timer_subscribe_int(&bit_no) != 0) {
    printf("Erro em timer_subscribe_int() ou em timer_set_frequency na funcao 'timer_test_int'\n");
  }

  int irq_set = bit_no; //irq_Set == bit(hook_id) <-> bit mask

  while (contador1 < time) {
    // Get a request message.
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { // received notification! /
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:                             // hardware interrupt notification /
          if (msg.m_notify.interrupts & irq_set) { // subscribed interrupt /
            timer_int_handler();                   //incrementa contador que conta o numero de interrupcoes (60 por segundo)
            if (contador % 60 == 0) {              //se o resto da divisao de contador por 60 e 0 passaram nsegundos
              contador1++;                         //contador que conta os segundos incrementa
              timer_print_elapsed_time(); //funcao que imprime a cada segundo
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
  *lsb = (uint8_t) val & 0xFF;// and COM 1111 1111 (MANTEM ULTIMOS 8 BITS)
  return 0;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  *msb = val >> 8;//FICA COM 
  return 0;
}
