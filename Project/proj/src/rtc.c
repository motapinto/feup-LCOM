#include <lcom/lcf.h>
#include "rtc.h"
#include "ibm_8042.h"

int rtc_hook_id=7;
extern game_info *info; //from proj.c

int rtc_subscribe_int(uint8_t *bit_no)
{
	if (sys_irqsetpolicy(RTC_IRQ, IRQ_REENABLE , &rtc_hook_id)!=OK){
		printf("Erro em rtc_subscribe_int()\n");
		return FUNCTION_FAIL;
	}
	*bit_no = BIT(7);
	return 0;
}

int rtc_unsubscribe_int()
{
	if (sys_irqrmpolicy(&rtc_hook_id)!=OK){
		printf("Erro em rtc_unsubscribe_int\n");
		return FUNCTION_FAIL;
	}
	return 0;
}

uint32_t rtc_read(uint8_t port)
{
	uint32_t data;
	if (sys_irqdisable(&rtc_hook_id)!=OK){
		printf("Erro em rtc_unsubscribe_int\n");
}
	if(sys_outb(RTC_REG, port)!=OK){
		printf("Erro em rtc_read() ao fazer sys_outb()\n");
	}
	if(sys_inb(RTC_DATA_REG, &data) !=OK){
		printf("Erro em rtc_read() ao fazer sys_outb()\n");
}
	if (sys_irqenable(&rtc_hook_id)!=OK){
		printf("Erro em rtc_unsubscribe_int\n");
}
	return data;
}

bool bcd()
{
	uint32_t Reg_B = 0;

	sys_outb(RTC_REG, REG_B);
	sys_inb(RTC_DATA_REG, &Reg_B);
	if (!(Reg_B & REGB_DM))
		return true;
	else
		return false;
}

void bcd_to_binary(uint32_t* num)
{
	*num = (((*num) & 0xF0) >> 4) * 10 + ((*num) & 0x0F); //https://www.tutorialspoint.com/computer_logical_organization/codes_conversion.htm
}

int UIP() //is updating
{
	uint32_t	Reg_A = 0;
	sys_outb(RTC_REG, REG_A);
	sys_inb(RTC_DATA_REG, &Reg_A);

	if ((Reg_A & REGA_UIP) != 0) //uip flag
		return FUNCTION_FAIL;
	return 0;
}

void read_time()
{
	while(UIP()!=0) {}
  info->seg  = rtc_read(SEG);
  info->min  = rtc_read(MINUTOS);
  info->hour = rtc_read(HOUR);
  if(bcd()) {
    bcd_to_binary(&info->hour);
    bcd_to_binary(&info->min);
    bcd_to_binary(&info->seg);
  }
}

void read_date()
{
	while(UIP()!=0) {}
  info->day   = rtc_read(DAY);
  info->month = rtc_read(MONTH);
  info->year  = rtc_read(YEAR);
  if(bcd()) {
    bcd_to_binary(&info->day);
    bcd_to_binary(&info->month);
    bcd_to_binary(&info->year);
  }
}
