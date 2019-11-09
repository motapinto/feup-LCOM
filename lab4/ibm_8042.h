#ifndef _LCOM_IBM_8042_H_
#define _LCOM_IBM_8042_H_

#define BIT(n) (0x01<<(n))
#define FUNCTION_FAIL -2;
#define PARAMETROS_FAIL -3;
#define TIME_OUT -4; //time out in polls

#define DELAY_US		         20000
#define KBD_IRQ	             1
#define MOUSE_IRQ            12
#define MOUSE_IRQ_LINE       4


#define KBD_STATUS_PORT     0x64
#define KBC_CMD_REG         0x64
#define KBD_OUT_BUF         0x60
#define KBD_IN_BUF          0x60 //input buffer 0x64 or 0x60 (comands to kbc - 0x64) (arguments of kbc - 0x60)
#define KBD_CMD_BUF		      0x64
#define KBD_DATA_BUF	      0x60

#define PAR_ERR        BIT(7)
#define TO_ERR         BIT(6)

// Keyboard Comands
#define KB_MENU			    0xf1
#define KB_ENABLE       0xf4
#define KB_MAKEBREAK    0xf7
#define KB_RESET		    0xff

#define ESC				      0x81
#define SPECIAL_KEY1	  0xe0
#define SPECIAL_KEY2	  0xe1

// (KBC “Command Byte”)
#define INT_K				      BIT(0) //INT 1: enable interrupt on OBF, from keyboard
#define INT_M				      BIT(1) //INT2 1: enable interrupt on OBF, from mouse;
#define DIS_M             BIT(5) //DIS2 1: disable mouse
#define DIS_K             BIT(4) //DIS 1: disable keyboard

//Status Register
#define OBF				           BIT(0)
#define IBF				           BIT(1)
#define SYS_FLAG		         BIT(2) //System flag: 0 if system in power-on reset, 1 if system already initialized
#define INH_FLAG             BIT(3) //Inhibit flag: 0 if keyboard is inhibited
#define MOUSE_AUX            BIT(5) //Mouse data
#define TIME_OUT_ERR	     	 BIT(6) //Timeout error - invalid data
#define PAR_ERR			         BIT(7)

// PS-2 Mouse Commands (Commands passed as arguments of command 0xD4; Responses to these commands, if any, are put in the KBC’s OUT_BUF and should be read via port 0x60)
#define DISABLE_REP         0xF5 //In stream mode, should be sent before any other command
#define ENABLE_REP          0xF4 //In stream mode only
#define SET_STREAM          0xEA //STREAM MODE
#define SET_REMOTE          0xF0 //REMOTE MODE
#define READ_DATA           0XEB //send data pcket request
#define STATUS_REQUEST      0xE9 //Get mouse configuration (3 bytes)
#define ACK                 0xFA //if everything OK
#define NACK                0xFE //if invalid byte (may be because of a serial communication error)
#define ERROR               0xFC //second consecutive invalid byte
#define MOUSE_RESET		      0xFF
#define MOUSE_DEF		        0XFE //Set default values
#define SCAL_21             0xE7 //Set Scaling 2:1 Acceleration mode
#define SCAL_11             0xE6 //Set Scaling 1:1 Linear mode

// PS-2 Mouse-Related KBC Commands (These commands are for the KBC and must be written to port 0x64 ; Arguments and return values are passed via port 0x60)
#define WRITE               0x60
#define READ                0x20
#define ENABLE_MOUSE        0xA8
#define DIS_MOUSE           0XA7
#define MOUSE_INT           0XA9 //Mouse interface: Returns 0, if OK
#define WRITE_TO_MOUSE      0xD4 //Write Byte to Mouse (0xD4 commands the KBC to forward its argument to the mouse without any interpretation)

//PS-2 MOUSE BYTE 1 (byte2: X delta |||||| byte3: Y delta)
#define MOUSE_LB            BIT(0)
#define MOUSE_RB            BIT(1)
#define MOUSE_MB            BIT(2)
#define MOUSE_X_SIGN        BIT(4)
#define MOUSE_Y_SIGN        BIT(5)
#define MOUSE_X_OV          BIT(6)
#define MOUSE_Y_OV          BIT(7)

#endif
