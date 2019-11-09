#include "ibm_8042.h"
#include "keyboard.h"
#include <lcom/timer.h>
#include <lcom/lcf.h>
#include <stdbool.h>
#include <stdint.h>

#define FUNCTION_FAIL -2
#define PARAMETERS_FAIL -3
#define UNS_FUNCTION_FAIL 2

extern u8_t scan_code;//variavel global dos ficheiros lab3 e keyboard
extern u32_t contador;// contador que conta o numero de interrupcoes

#ifdef LAB3
  extern uint32_t sys_inb_count;//new definition of sys_inb
#endif


int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");
  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need/ it]
  lcf_trace_calls("/home/lcom/labs/lab3/trace.txt");
  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab3/output.txt");
  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;
  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();
  return 0;
}


//read scancodes from KBC using IH
int (kbd_test_scan)(bool assembly)//if bool=true should use IH written in assembly otherwise it is written in C
{
  uint8_t bit_no=0;
  uint8_t size=0, bytes[2] = {0};//no maximo pode ter 2 bytes, vetor inicializado a 0
  bool flag_special_byte = false;

  message msg;
  int ipc_status, r;

  if(kbd_subscribe_int(&bit_no)!=0){//"independently you should first subscribe the KBC interrupts"
    printf("Erro em kbd_subscribe_int() na funcao: 'kbd_test_scan'\n");
    return FUNCTION_FAIL;
  }

  uint8_t kb_irq_set = bit_no;

  while(scan_code != ESC) { /*Quando for premida a tecla ESC(0X81) acaba o ciclo */

    /* Get a request message. */
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
        printf("driver_receive failed with: %d", r);
        continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification */

        switch (_ENDPOINT_P(msg.m_source))
        {
            case HARDWARE: /* hardware interrupt notification */
                if (msg.m_notify.interrupts & kb_irq_set) { /* subscribed interrupt */

                    if(assembly==0){//recebe em c
                      kbc_ih();
                    }

                    else if(assembly==1){//recebe em assembly
                      kbc_asm_ih();
                    }


                    if(flag_special_byte){//se bool = true ->preencher a segunda pos
                        bytes[1] = scan_code;
                        size = 2;
                        flag_special_byte = false;//atualizar bool
                    }

                    else{
                      bytes[0] = scan_code;
                      size = 1;
                    }

                    if(is_special_key(scan_code)){
                      flag_special_byte = true;
                    }

                    if(!flag_special_byte && kbd_print_scancode(!make_break(scan_code), size, bytes)!=OK){//make_break = 0(break || make_break = 1(make)
                      printf("Erro em kbd_test_scan() na funcao: 'kbd_print_scancode'\n");
                      return FUNCTION_FAIL;
                    }


                }//subscribe interrupt loop end

                break;

            default:
                break; /* no other notifications expected: do nothing */
        }
    }
 }//while loop end

 #ifdef LAB3
   kbd_print_no_sysinb(sys_inb_count);
 #endif

  if(kbd_unsubscribe_int()!=0){//"Sould cancel subscripton of the KBC interrupt before ending"
    printf("Erro em kbd_test_scan() na funcao: 'kbd_unsubscribe_int'\n");
    return FUNCTION_FAIL;
  }

  return 0;
}


int (kbd_test_poll)()
{
  uint8_t size=0, bytes[2] = {0};//no maximo pode ter 2 bytes, vetor inicializado a 0
  bool flag_special_byte = false;

  while(scan_code != ESC) { /*Quando for premida a tecla ESC(0X81) acaba o ciclo */

      scan_code = kbd_read_outbuf(); //reads scan_code from outbuff

      if(flag_special_byte){//se bool = true ->preencher a segunda pos
          bytes[1] = scan_code;
          size = 2;
          flag_special_byte = false;//atualizar bool
      }

      else{
          bytes[0] = scan_code;
          size = 1;
      }

      if(is_special_key(scan_code)){
          flag_special_byte = true;
      }

      if(!flag_special_byte && kbd_print_scancode(!make_break(scan_code), size, bytes)!=OK){//make_break = 0(break || make_break = 1(make)
          printf("Erro em kbd_test_scan() na funcao: 'kbd_print_scancode'\n");
          return FUNCTION_FAIL;
      }
  }//while loop end

  #ifdef LAB3
    kbd_print_no_sysinb(sys_inb_count);
  #endif

  if(kbd_reset_status()==UNS_FUNCTION_FAIL){
    printf("Erro em kbd_test_scan() na funcao: 'kbd_reset_status'\n");
    return FUNCTION_FAIL;
  }
  return 0;
}



int (kbd_test_timed_scan)(uint8_t n)
{
  message msg;
  int ipc_status, r;
  unsigned int contador1=0;
  uint8_t timer_irq_set, bit_no_timer;
  uint8_t kbd_irq_set, bit_no_kbd;
  uint8_t size=0, bytes[2] = {0};//no maximo pode ter 2 bytes, vetor inicializado a 0
  bool flag_special_byte = false;

  if(kbd_subscribe_int(&bit_no_kbd)){//"independently you should first subscribe the KBC interrupts"
    printf("Erro em kbd_subscribe_int() na funcao: 'kbd_test_timed_scan'\n");
    return FUNCTION_FAIL;
  }

  kbd_irq_set = bit_no_kbd;

  if(timer_subscribe_int(&bit_no_timer)){//"independently you should first subscribe the KBC interrupts"
    printf("Erro em kbd_subscribe_int() na funcao: 'kbd_test_timed_scan'\n");
    return FUNCTION_FAIL;
  }

  timer_irq_set = bit_no_timer;

  while (contador1 < n && scan_code != ESC) {
    // Get a request message.
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { // received notification! /
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
                              // hardware interrupt notification /
            if (msg.m_notify.interrupts & timer_irq_set){ // subscribed interrupt /
              timer_int_handler();                   //incrementa contador que conta o numero de interrupcoes (60 por segundo)
              if (contador % 60 == 0) {              //se o resto da divisao de contador por 60 e 0 passaram nsegundos
                contador1++;                         //contador que conta os segundos incrementa
              }
            }

            if (msg.m_notify.interrupts & kbd_irq_set){

              kbc_ih();
              contador = 0;
              if(flag_special_byte){//se bool = true ->preencher a segunda pos
                  bytes[1] = scan_code;
                  size = 2;
                  flag_special_byte = false;//atualizar bool
              }

              else{
                bytes[0] = scan_code;
                size = 1;
              }

              if(is_special_key(scan_code)){
                flag_special_byte = true;
              }

              if(!flag_special_byte && kbd_print_scancode(!make_break(scan_code), size, bytes)!=OK){//make_break = 0(break || make_break = 1(make)
                printf("Erro em kbd_test_scan() na funcao: 'kbd_print_scancode'\n");
                return FUNCTION_FAIL;
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

  if(kbd_unsubscribe_int()!=0){//"Sould cancel subscripton of the KBC interrupt before ending"
    printf("Erro em kbd_test_scan() na funcao: 'kbd_unsubscribe_int'\n");
    return FUNCTION_FAIL;
  }

  if(timer_unsubscribe_int()!=0){//"Sould cancel subscripton of the KBC interrupt before ending"
    printf("Erro em kbd_test_scan() na funcao: 'kbd_unsubscribe_int'\n");
    return FUNCTION_FAIL;
  }

  printf("%d\n", n);

  return 0;
}

//sempre que ha um interrupt fazer interupt hnadler
