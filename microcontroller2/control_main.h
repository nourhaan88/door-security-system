/*
 * control_main.h
 *
 *  Created on: ??þ/??þ/????
 *      Author: Nourhan Ehab
 */

#ifndef CONTROL_MAIN_H_
#define CONTROL_MAIN_H_

#include "uart.h"
#include "motor.h"
#include "buzzer.h"
#include "timer.h"
#include "external_eeprom.h"
#include "std_types.h"
#include "common_macros.h"
#include <util/delay.h>
#include <avr/io.h>

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
#define TIMER_TICKS_15SECS                                      60
#define TIMER_TICKS_30SEC                                       120
#define TIMER_TICKS_1MINUTE_STOP                                240
#define DEFUALT_VALUE_OF_EEPROM   1
/*******************************************************************************
 *                                types declaration                                  *
 *******************************************************************************/
/*configure UART frame to
 * 1-send eight bits
 * 2-disable parity
 * 3-send one stop bit
 */
UART_configType uart_config={eight_bits,disabled,one_bit};

/* configure timer driver
 * 1- F_timer= F_CPU/8
 * 2- mode : compare
 * */
Timer_ConfigType timer_config = {F_CPU_8,CTC_MODE,OC0,NORMAL,0,250};
/*******************************************************************************
 *                                Global Variables                                  *
 *******************************************************************************/
uint8 password_1[PASSWORD_ARRAY_SIZE];
uint8 password_2 [PASSWORD_ARRAY_SIZE];
uint8 savedpass[PASSWORD_ARRAY_SIZE];
uint8 command;
uint8 g_tick=0;
uint8 g_wrong=0;
/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/
/*
 * Description :
 * take the new password twice from micro-controller 1 and check if it's a match
 *
 * Arguments   : Array to fill
 *
 * Returns     : Void*/
void recievePassword(uint8 a_arr[]);


uint8 CheckMatching(uint8 a_arr1[],uint8 a_arr2[]);


void storePasswordInMemory(void);


void getSavedPassword(uint8 a_arr[]);

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

/*Description :initiate alarm in case of wrong password and de-initiate it after 1 minute
 *
 * Arguments  : None
 *
 * Returns    : Void */

void alarmStartStop(void);
/*Description :open and close the door in case of right password
 *
 * Arguments  : None
 *
 * Returns    : Void */
void DCmotorStartStop(void);

#endif /* CONTROL_MAIN_H_ */
