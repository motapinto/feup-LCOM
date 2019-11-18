#include <lcom/lcf.h>
#include <lcom/timer.h>
#include <minix.h>
#include "ibm_8042.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <mouse.h>

extern struct packet pp; //from mouse.c
extern int packet_store_counter; //from mouse.c - conta a pos de packet pp[pos]
extern u32_t contador; // from timer.c - conta interrupcoes
extern int mouse_hook_id;

typedef enum {START, LINE1, VERTEX, LINE2, EXIT} state_t;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need/ it]
  lcf_trace_calls("/home/lcom/labs/lab4/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab4/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int (mouse_test_packet)(uint32_t cnt)//cnt:	Number of packets to receive and display before exiting
{
  uint32_t mouse_irq_set = 0;
  //uint8_t check_byte=0;
  int r, ipc_status;
  message msg;

  if(mouse_subscribe_int(&mouse_irq_set) != 0){
    printf("Erro em mouse_test_packet ao fazer mouse_subscribe_int\n");
  }
  if (mouse_int(false) != 0) {
    printf("Erro em mouse_int(false) \n");
    return -1;
  }
  if (mouse_set_mode(stream) != 0) {//"Need to enable stream mode"
    printf("Erro em set_mode(stream)\n");
    return -1;
  }
  if (mouse_data_report(true) != 0) {//"data reporting by the mouse must be enabled"
    printf("Erro em mouse_data_report(true) \n");
    return -1;
  }
  if (mouse_int(true) != 0) {
    printf("Erro em mouse_int(false) \n");
    return -1;
  }

  while(cnt>0) {

    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification! */
      switch (_ENDPOINT_P(msg.m_source)) {
      case HARDWARE: /* hardware interrupt notification */
          if (msg.m_notify.interrupts & mouse_irq_set) { /* subscribed interrupt */
            mouse_ih(); //"Should read only one byte per interrupt"
            if(packet_store_counter > 2){
              mouse_print_packet(&pp);
              cnt--;
            }
          }
          break;
      default:
          break; /* no other notifications expected: do nothing */
      }
    }
    else{}
  }
/*However, before returning, it should reset the mouse to its state before the execution of your program,
 i.e. it must disable data reporting and unsubscribe mouse interrupts.*/

 if (mouse_int(false) != 0) {
   printf("Erro em mouse_int(false) \n");
   return -1;
 }
 if (mouse_data_report(false) != 0) {
   printf("Erro em mouse_data_report(false) \n");
   return -1;
 }
 if (mouse_int(true) != 0) {
   printf("Erro em mouse_int(false) \n");
   return -1;
 }
 if (mouse_unsubscribe_int() != 0) {
   printf("Erro em mouse_unsubscribe_int() \n");
   return -1;
 }
// m_read_cmd(KBD_OUT_BUF, &check_byte);
 return 0;
}


//period	Period in milliseconds
//cnt	Number of packets to receive and display before exiting
int (mouse_test_remote)(uint16_t period, uint8_t cnt) {//should configure the mouse to operate in remote mode and periodically request a data packet from the mouse
  packet_store_counter = 0;
  uint8_t check_byte, data;
  int result;

//!!!Powerpoint information iswrong... lcf already consfigures in thes modes
  /*mouse_int(false); //disable mouse interruption
  mouse_data_report(false); //disables data report
  mouse_set_mode(remote); // remote mode enable
*/

  m_write_cmd(KBD_STATUS_PORT, WRITE_TO_MOUSE);
  m_write_cmd(KBD_IN_BUF, READ_DATA);//reads packet upon reading 3 times outbuf

  result = ack_check(&check_byte);//The device replies with an ACK, followed by a three- or six-byte motion data packet
  if(result == NACK){//retry
    m_write_cmd(KBD_STATUS_PORT, WRITE_TO_MOUSE);
    m_write_cmd(KBD_IN_BUF, READ_DATA);//reads packet upon reading 3 times outbuf
  }
  else if(result == ERROR){//exit
    return FUNCTION_FAIL;
  }

  while(cnt>0){
    while(packet_store_counter<=2){
      if(mouse_read_outbuf(&data)!=0){
        printf("Erro em mouse_read_outbuf()\n");
      }
      packet_store(data);
    }
    mouse_print_packet(&pp);
    tickdelay(micros_to_ticks(period));
    cnt--;
  }

  if (mouse_set_mode(stream) != 0) {//"Need to enable stream mode"
    printf("Erro em set_mode(stream)\n");
    return -1;
  }
  if (mouse_data_report(false) != 0) {
    printf("Erro em mouse_data_report(false) \n");
    return -1;
  }
  if(m_write_cmd(KBD_STATUS_PORT,WRITE)!=0){
    printf("Erro ao fazer m_write_cmd(KBD_STATUS_PORT,WRITE)\n");
  }
  if(m_write_cmd(KBD_IN_BUF, minix_get_dflt_kbc_cmd_byte())!=0){//set default... enables interrupts,...
    printf("Erro ao fazer m_write_cmd(KBD_IN_BUF, minix_get_dflt_kbc_cmd_byte())\n");
  }
  return 0;
}


//idle_time	Number of seconds without packets before exiting
int (mouse_test_async)(uint8_t idle_time) {
  uint8_t timer_irq_set=0, idle_time_counter = idle_time;
  uint32_t freq = sys_hz(), mouse_irq_set = 0;
  int r, ipc_status;
  message msg;

  if(mouse_subscribe_int(&mouse_irq_set) != 0){
    printf("Erro em mouse_test_packet ao fazer mouse_subscribe_int\n");
  }
  if(timer_subscribe_int(&timer_irq_set) != 0){
    printf("Erro em mouse_test_async ao fazer timer_subscribe_int\n");
  }
  if (mouse_int(false) != 0) {
    printf("Erro em mouse_int(false) \n");
    return -1;
  }
  if (mouse_set_mode(stream) != 0) {//"Need to enable stream mode"
    printf("Erro em set_mode(stream)\n");
    return -1;
  }
  if (mouse_data_report(true) != 0) {//"data reporting by the mouse must be enabled"
    printf("Erro em mouse_data_report(true) \n");
    return -1;
  }
  if (mouse_int(true) != 0) {
    printf("Erro em mouse_int(false) \n");
    return -1;
  }


  while(idle_time_counter>0) {

    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification! */
      switch (_ENDPOINT_P(msg.m_source)) {
      case HARDWARE: /* hardware interrupt notification */

          if (msg.m_notify.interrupts & timer_irq_set) { /* subscribed interrupt */
            timer_int_handler();
            if(contador%freq == 0){ //nºinterrupcoes >= segundos*freq (ex: 1seg frq=60 , se nºint>=60 break)
              idle_time_counter--;
            }
          }

          if (msg.m_notify.interrupts & mouse_irq_set) { /* subscribed interrupt */
            mouse_ih(); //"Should read only one byte per interrupt"
            contador = 0;
            idle_time_counter = idle_time;
            if(packet_store_counter > 2){
              mouse_print_packet(&pp);
            }
          }
          break;
      default:
          break; /* no other notifications expected: do nothing */
      }
    }
  }
  /*However, before returning, it should reset the mouse to its state before the execution of your program,
  i.e. it must disable data reporting and unsubscribe mouse interrupts.*/

  if (mouse_int(false) != 0) {
    printf("Erro em mouse_int(false) \n");
    return -1;
  }
  if (mouse_data_report(false) != 0) {
    printf("Erro em mouse_data_report(false) \n");
    return -1;
  }
  if (mouse_int(true) != 0) {
    printf("Erro em mouse_int(false) \n");
    return -1;
  }
  if (mouse_unsubscribe_int() != 0) {
    printf("Erro em mouse_unsubscribe_int() \n");
    return -1;
  }
  return 0;
}

state_t gesture_s_machine_inv_v(struct mouse_ev *evt ,uint8_t x_len, uint8_t tolerance) {
    static int32_t dx_t, dy_t; //dx and dy total
    static state_t state = START;

    switch (state) {
      case START:
        dx_t = 0;
        dy_t = 0;
        if(evt->type == LB_PRESSED)
          state = LINE1;
        break;

      case LINE1:
        if(evt->type == LB_RELEASED) {
          if (dx_t >= x_len && dy_t > dx_t){ //slope > 1 - first line
            state = VERTEX;
          }
          else {
            state = START;
          }
        }
        else if(evt->type == RB_PRESSED || evt->type == BUTTON_EV)
          state = START;//goes to initial state if rb or other button event detected
        else if(evt->type == MOUSE_MOV) {
          dx_t += evt->delta_x;
          dy_t += evt->delta_y;
          if(evt->delta_x + tolerance < 0){//evt->delta_x + tolerance has to be greater than 0 for first line
            state = START;
          }
        }
        break;

      case VERTEX:
        if(evt->type == LB_PRESSED)
          state = LINE1;
        if (evt->type == BUTTON_EV)
          state = START;
        if(evt->type == RB_PRESSED) { //change of buttons in vertex -> next state
          dx_t = 0;
          dy_t = 0;
          state = LINE2;
        }
        if(evt->type == MOUSE_MOV){
          if(abs(evt->delta_x) > tolerance || abs(evt->delta_y) > tolerance){
            state = START;
          }
        }
        break;

      case LINE2:
        if(evt->type == RB_RELEASED) {
          if (dx_t >= x_len &&  dy_t < -dx_t){ //slope has to be inferior to -1
            state = EXIT;
          }
          else {
            state = START;
          }
        }
        if(evt->type == LB_PRESSED || evt->type == BUTTON_EV){
          state = START; //second line must be drawn only with rb
        }
        if(evt->type == MOUSE_MOV) {
          dx_t += evt->delta_x;
          dy_t += evt->delta_y;
          if(evt->delta_x + tolerance < 0)
            state = START;
        }
        break;

      case EXIT: break;
    }
  return state;
}

int (mouse_test_gesture)(uint8_t x_len, uint8_t tolerance) {
  uint32_t mouse_irq_set;
  struct mouse_ev m_evt;
  bool mouse_event = false;

  state_t state = IDLE;
  int ipc_status, r;
  message msg;

  // Subscribing mouse interrupts
  if (mouse_subscribe_int(&mouse_irq_set) != 0) {
    printf("Erro em mouse_subscribe_int() \n");
    return -1;
  }
  // Disabling mouse interrupts (can also do sys_disable)
  if (mouse_int(false) != 0) {
    printf("Erro em mouse_int(false) \n");
    return -1;
  }
  if (mouse_set_mode(stream) != 0) {
    printf("Erro em set_mode(stream)\n");
    return -1;
  }
  if (mouse_data_report(true) != 0) {
    printf("Erro em mouse_data_report(true) \n");
    return -1;
  }
  if (mouse_int(true) != 0) {
    printf("Erro em mouse_int(true) \n");
    return -1;
  }

  while(state != EXIT) {
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { // received notification
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: // hardware interrupt notification
          if (msg.m_notify.interrupts & mouse_irq_set) { // subscribed interrupt
            mouse_ih();
            if(packet_store_counter > 2){
              mouse_print_packet(&pp);
              m_evt = mouse_event_handler(&pp);
              mouse_event = true;
            }
          }
          break;
        default:
          break; // no other notifications expected: do nothing
      }
    }
    if(mouse_event) {
      state = gesture_s_machine_inv_v(&m_evt, x_len, tolerance);
      mouse_event = false;
    }
  }

  if (mouse_int(false) != 0) {
    printf("Erro em mouse_int(false) \n");
    return -1;
  }
  if (mouse_data_report(false) != 0) {
    printf("Erro em mouse_data_report(false) \n");
    return -1;
  }
  if (mouse_int(true) != 0) {
    printf("Erro em mouse_int(false) \n");
    return -1;
  }
  if (mouse_unsubscribe_int() != 0) {
    printf("Erro em mouse_unsubscribe_int() \n");
    return -1;
  }
  return 0;
}
