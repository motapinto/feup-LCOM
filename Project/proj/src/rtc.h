#ifndef RTC_H
#define RTC_H

/** @defgroup rtc rtc
 * @{
 *
 */

#include "game.h"

#define RTC_IRQ						0X08
#define RTC_DATA_REG			0X71
#define RTC_REG						0X70
//TIME
#define SEG								0X00
#define	MINUTOS						0X02
#define	HOUR							0X04
//DATE
#define	DAY				       	0x07
#define	MONTH							0X08
#define	YEAR							0X09

#define REG_A							10
#define REG_B							11
#define REG_C							12
#define REG_D							13

#define REGA_UIP					BIT(7)
#define REGB_DM				    BIT(2) //0x04

/**
* @brief Subscribes rtc interruptions
*/
int rtc_subscribe_int(uint8_t *bit_no);
/**
* @brief Unsubscribes rtc interruptions
*/
int rtc_unsubscribe_int();
/**
* @brief Reads from RTC_DATA_REG what is in port
* @param port One of the possible registers from rtc
*/
uint32_t rtc_read(uint8_t port);
/**
* @brief Checks if it is in bcd
*/
bool bcd();
/**
* @brief Converts num to binary if and only if it is in bcd
* @param num number to be converted
*/
void bcd_to_binary(uint32_t* num);
/**
* @brief Checks if rtc is updating
*/
int UIP();
/**
* @brief Reads time from the rtc only if rtc is not updating, also, if
* it is bcd converts to bcd
*/
void read_time();
/**
* @brief Reads date from the rtc only if rtc is not updating, also, if
* it is bcd converts to bcd
*/
void read_date();

#endif
