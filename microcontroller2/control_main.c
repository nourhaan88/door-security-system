/*
 * control_main.c
 *
 *  Created on: ??þ/??þ/????
 *      Author: Nourhan Ehab
 */

#include "control_main.h"


int main (void){

	SREG |= (1<<7);       /* Enable I-Bit for Interrupts */

	/*baud rate = 9600*/
UART_init(9600,&uart_config);

//DCmotor_init();




  while(1){
	  switch(recieveCommand()){
	   case SEND_PASSWORD :
		   recievePassword(password_1);
		   break;
	   case PASSWORD_CONFIRMATION:
		   recievePassword(password_2);
		   if(CheckMatching(password_1,password_2)){
			   storePasswordInMemory();
			   getSavedPassword(savedpass);
			   sendCommand(MATCH);
		   }
		   else {
			   sendCommand(MISMATCH);
		   }
		   break;
	   case CHECK_PASSWORD :
		   recievePassword(password_1);
		   getSavedPassword(savedpass);
		    if(CheckMatching(password_1,savedpass)){
		    	sendCommand(MATCH);
		    	g_wrong=0;
		    }
		    else {
		    	sendCommand(MISMATCH);

		    }
		    break;
	   case OPEN_DOOR :
		   DCmotorStartStop();

		   //DCmotor_deinit();
		   break;
	   case WRONG_PASSWORD :
		   g_wrong++;
		   			if(g_wrong==MAX_WRONG_ATTEMPT)
		   			{
		   				alarmStartStop();
		   			}
		   			break;
	   case CHECK_IF_SAVED :
		   getSavedPassword(savedpass);
		   			uint8 counter=0;
		   			for(uint8 i = 0 ; i < PASSWORD_ARRAY_SIZE; i++)
		   			{
		   				if(savedpass[i] == DEFUALT_VALUE_OF_EEPROM)
		   				{
		   					counter++;
		   				}

		   			}
		   			if (counter==PASSWORD_ARRAY_SIZE)
		   			{
		   				sendCommand(NOT_SAVED);
		   			}
		   			else
		   			{
		   				sendCommand(YES_SAVED);
		   			}
		   			break;

	  }
  }

}

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
void recievePassword(uint8 a_arr[])
{
	while(UART_recieveByte() != MICRO_IS_READY){};
	UART_sendByte(MICRO_IS_READY);
	for(int i=0 ; i<PASSWORD_ARRAY_SIZE ; i++){
		a_arr[i]=UART_recieveByte();
	}
	UART_sendByte(MICRO_IS_DONE);
}

uint8 CheckMatching(uint8 a_arr1[],uint8 a_arr2[])
{
	uint8 counter=0;
	for(uint8 i=0 ; i<PASSWORD_ARRAY_SIZE ; i++)
	{
		if(a_arr1[i]==a_arr2[i]){
			counter++;
		}
	}
	if(counter==PASSWORD_ARRAY_SIZE){
		return TRUE;
	}
	else{
		return FALSE;
	}
}

void storePasswordInMemory(void)
{


	for(uint8 i=0;i<PASSWORD_ARRAY_SIZE;i++){
		EEPROM_writeByte(0x0311+i,password_1[i]);
		_delay_ms(250);
	}

}

void getSavedPassword(uint8 a_arr[])
{

	for(uint8 i=0 ; i<PASSWORD_ARRAY_SIZE ; i++){
		EEPROM_readByte(0x0311+i, &a_arr[i]);
		_delay_ms(1000);
	}
}

/*Description : send commandS through UART
 *
 * Arguments  : the command to be sent
 *
 * Returns    : Void*/

void sendCommand(uint8 command)
{


	UART_sendByte(MICRO_IS_READY);
	while(UART_recieveByte() != MICRO_IS_READY){};
	UART_sendByte(command);
	while(UART_recieveByte() != MICRO_IS_DONE){};

}

/*Description : receive commandS through UART
 *
 * Arguments  : the command to be sent
 *
 * Returns    : Void*/

uint8 recieveCommand(void)
{
	while(UART_recieveByte() != MICRO_IS_READY){};
	UART_sendByte(MICRO_IS_READY);
	command=UART_recieveByte();
	UART_sendByte(MICRO_IS_DONE);
	return command;
}

/*Description :call back function of timer ISR to the motor timer
 *
 * Arguments  : None
 *
 * Returns    : Void */

void g_tickCounterMotor(void)
{
	g_tick++;

	if(g_tick==TIMER_TICKS_30SEC)
	{

		DCmotor_deinit();
		g_tick=0;
	}
}

/*Description :call back function of timer ISR to the alert timer
 *
 * Arguments  : None
 *
 * Returns    : Void */

void g_tickCounterAlarm(void)
{
	g_tick++;
	if(g_tick == TIMER_TICKS_1MINUTE_STOP)
	{

		g_tick=0;

	}
}

void alarmStartStop(void){
	buzzer_init();

	buzzer_on();
	_delay_ms(60000);

	g_wrong=0;
	buzzer_off();

}

void DCmotorStartStop(void){
	 DCmotor_init();

	 DCmotor_Rotate_CW();
	 _delay_ms(15000);

	  DCmotor_deinit();
	 _delay_ms(2000);

	DCmotor_Rotate_A_CW();
	_delay_ms(15000);

	DCmotor_deinit();

}
