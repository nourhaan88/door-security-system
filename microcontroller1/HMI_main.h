/*
 * HMI_main.h
 *
 *  Created on: ??þ/??þ/????
 *      Author: Nourhan Ehab
 */

#ifndef HMI_MAIN_H_
#define HMI_MAIN_H_

#include "std_types.h"
#include "timer.h"
#include "uart.h"
/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

#define MATCH                    0X10
#define MISMATCH                 0X20
#define PASSWORD_MATCH           0X30
#define PASSWORD_NOT_MATCHED     0X40
#define CHECK_PASSWORD           0X50
#define OPEN_DOOR                0X60
#define CHECK_IF_SAVED           0X70
#define YES_SAVED                0X80
#define NOT_SAVED                0X90
#define SEND_PASSWORD            0XAA
#define PASSWORD_CONFIRMATION    0XBB
#define CHANGE_PASSWORD          0XCC
#define WRONG_PASSWORD           0XDD
#define MICRO_IS_READY           0XFF
#define MICRO_IS_DONE            0X02
#define MAX_WRONG_ATTEMPT        3
#define PASSWORD_ARRAY_SIZE      5
#define TIMER_TICKS_15SEC        60
#define TIMER_TICKS_30SEC        120
#define TIMER_TICKS_1MINUTE      240
#define TIMER_TICKS_1MINUTE_STOP 241
/*******************************************************************************
 *                                types declaration                                  *
 *******************************************************************************/
/*configure UART frame to
 * 1-send eight bits
 * 2-disable parity
 * 3-send one stop bit
 */
UART_configType uart_config = {eight_bits,disabled,one_bit};

/* configure timer driver
 * 1- F_timer= F_CPU/8
 * 2- mode : compare
 * */
Timer_ConfigType timer_config = {F_CPU_8,CTC_MODE,OC0,NORMAL,0,250};
/*******************************************************************************
 *                                Global Variables                                  *
 *******************************************************************************/
static volatile uint8 g_flag=0;
static volatile uint8 g_state=0;
uint8 g_key;
uint8 g_wrong=0;
uint8 g_tick=0;
uint8 command=0;
uint8 g_password[PASSWORD_ARRAY_SIZE];
uint8 g_cmd;
/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/

/*
 * Description :
 * take the new password twice and send it by uart to the control microcontroller
 *
 * Arguments : None
 *
 * Returns :   Void*/
void new_password(void);

/*
 * Description : display main menu
 *
 *Arguments : none
 *
 *Returns :   void  */

void main_menu(void);

/*Description : to create and store password
 *
 * arguments  : Array to fill the password in
 *
 * returns    : Void
 * */

void fillInPassword(uint8 a_arr[]);

/* Description : send password to the other micro-controller through UART
 *
 * Arguments   : Array of password to send
 *
 * Returns     : Void */

void SendPassword(uint8 a_arr[]);

/*Description : send commandS through UART
 *
 * Arguments  : the command to be sent
 *
 * Returns    : Void*/

void sendCommand(uint8 command);

/*Description : receive commandS through UART
 *
 * Arguments  : None
 *
 * Returns    : Void  */

uint8 recieveCommand(void);

/*Description :display alert screen for 1 minute
 *
 * Arguments  : None
 *
 * Returns    : Void */

void ALERTscreen(void);

/*Description :display opening and closing the door screen for 15 seconds
 *
 * Arguments  : None
 *
 * Returns    : Void */


void doorIsOpeningCloseingScreen(void);

/*Description :call back function of timer ISR to the motor timer
 *
 * Arguments  : None
 *
 * Returns    : Void */

void g_tickCounterMotor(void);

/*Description :call back function of timer ISR to the alert timer
 *
 * Arguments  : None
 *
 * Returns    : Void */

void g_tickCounterAlarm(void);

/* Description : check if there's saved password or it's the first time to enter password
 *
 * Arguments   : None
 *
 * Return      : Void
 */

void ifPassSaved(void);

#endif /* HMI_MAIN_H_ */
