#ifndef _LCOM_IBM_8042_H_
#define _LCOM_IBM_8042_H_

#define BIT(n) (0x01<<(n))

#define DELAY_US		20000

#define KBD_STATUS_PORT     0x64
#define KBC_CMD_REG 0x64
#define KBD_OUT_BUF     0x60
#define KBD_IN_BUF    0x60 //input buffer 0x64 or 0x60 (comands to kbc - 0x64) (arguments of kbc - 0x60)
#define KBD_CMD_BUF		0x64
#define KBD_DATA_BUF	0x60
#define WRITE 0x60
#define READ 0x20



#define OUT_BUF_FULL    0x01
#define INPT_BUF_FULL   0x02
#define SYS_FLAG		    0x04
#define CMD_DATA		    0x08
#define KEYBD_INH		    0x10
#define TR0
#define KCV_TMOUT		    0x40
#define PARITY_EVEN     0x80
#define INH_KEYBOARD    0x10

#define KBD_ENA			0xae
#define KBD_DIS			0xad
#define KBD_CHECK   0xab //check interface

// Keyboard Comands
#define KB_MENU			    0xf1
#define KB_ENABLE       0xf4
#define KB_MAKEBREAK    0xf7
#define KB_ECHO         0xfe
#define KB_RESET		    0xff
#define KB_LED_CMD		  0xed

// Keyboard Responses
#define KB_OK			      0xaa
#define KB_ACK          0xfa
#define KB_OVERRUN      0xff
#define KB_RESEND		    0xfe
#define KB_BREAK		    0xf0
#define KB_FA			      0x10
#define	KB_FE			      0x20
#define	KB_PR_LED		    0x40

#define ESC				    0x81
#define SPECIAL_KEY1	0xe0
#define SPECIAL_KEY2	0xe1

#define KBD_IRQ	        1

#define IBF				  BIT(1)
#define OBF				  BIT(0)
#define PAR_ERR			BIT(7)
#define TO_ERR			BIT(5)
#define LEDS_ON			BIT(0)|BIT(1)|BIT(2)

#endif
