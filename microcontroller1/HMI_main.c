/*
 * HMI_main.c
 *
 *  Created on: ??þ/??þ/????
 *      Author: Nourhan Ehab
 */
#include "HMI_main.h"
#include "keypad.h"
#include "lcd.h"
#include "timer.h"
#include "uart.h"
#include <avr/io.h> /*to use SREG REGISTER*/
#include <util/delay.h>

int main (void){
	SREG |= (1<<7);       /* Enable I-Bit for Interrupts */

	LCD_init();

	UART_init(9600,&uart_config); /*baud rate = 9600*/

	LCD_clearScreen();

	LCD_displayStringRowColumn(0,1,"DOOR LOCKER");

	_delay_ms(1000);

    ifPassSaved(); /*check if there is a saved password*/

	while(1){
		if (g_state ==0){
			new_password();
		}
		else if (g_state ==1 && g_flag==5){
					g_wrong++;
					g_flag=3;
					//g_state=1;
					//main_menu();
				}
		else if (g_wrong==MAX_WRONG_ATTEMPT){
					ALERTscreen();
				}
		else if (g_state==1){
			main_menu();
		}
		else if(g_state==2){
		   LCD_clearScreen();

		   sendCommand(OPEN_DOOR);

		   doorIsOpeningCloseingScreen();
		}



		/*switch(g_state){
		case '0' :
			new_password();

			break;
		case '1' :
			main_menu();
			break;
		case '2' :
			sendCommand(OPEN_DOOR);
			doorIsOpeningCloseingScreen();
			break;
		default :
			main_menu();
			break;
		}
		if(g_flag==5){
			g_wrong++;
		}
		if(g_wrong==MAX_WRONG_ATTEMPT){
			ALERTscreen();
		}

	}*/

 }
}
/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/

/*
 * Description :
 * take the new password twice and send it by UART to the control micro-controller
 *
 *  Arguments  : None
 *
 * Returns     : Void*/

void new_password(void){
	/* first entry :
	 * save the password in array then send it through UART
	 * */
	if(g_flag==0){
	LCD_clearScreen();
	LCD_displayStringRowColumn(0,1,"please enter pass");
	LCD_moveCursor(3,1);
	fillInPassword(g_password);
	sendCommand(SEND_PASSWORD);
	SendPassword(g_password);
	g_flag=1;
	}
	/*second entry
	 *save the password in array then send it through UART again
	 **/

	else if(g_flag==1){
	LCD_clearScreen();
	LCD_displayStringRowColumn(0,1,"please re-enter pass :");
	LCD_moveCursor(3,1);
	fillInPassword(g_password);
	sendCommand(PASSWORD_CONFIRMATION);
	SendPassword(g_password);
	g_flag=2;
		}
	/* when the other micro-controller compare the 2 entries it send a command
	 * MATCH : go to main menu
	 * MISMATCH : returns to new password screen
	 */
	else if(g_flag==2){

		switch(recieveCommand())
		{
		case MATCH:
			LCD_clearScreen();
			LCD_displayStringRowColumn(0,1,"password match");
			_delay_ms(2000);
			g_flag=3;
			g_state=1;
			break;

		case MISMATCH:
			LCD_clearScreen();
			LCD_displayStringRowColumn(0,1,"password did not match");
			g_flag=0;
			_delay_ms(1000);
			break;
		/*default :
			LCD_clearScreen();
			LCD_displayStringRowColumn(0,1,"error");
			break;*/
		}
	}
	/*else
		g_state =1 ;*/

}
/*
 * Description : display main menu
 *
 * Arguments  : None
 *
 * Returns     : Void*/

void main_menu(void){
	/* + : take the password and send it to the other micro-controller
	 * to check if its a match open the door
	 * - : take the password and send it to other micro-controller to check if its a match
	 * to change to new password
	 * */
	LCD_clearScreen();
	LCD_displayStringRowColumn(0,1,"+ : open the door");
	LCD_displayStringRowColumn(3,1,"- : change pass");

	 if (KEYPAD_getPressedKey()=='+'){
		LCD_clearScreen();
		LCD_displayStringRowColumn(0,1,"please enter pass :");
		LCD_moveCursor(3,1);
		fillInPassword(g_password);
		 LCD_clearScreen();
		 LCD_displayStringRowColumn(0,1,"checking..");
		_delay_ms(1000);


		sendCommand(CHECK_PASSWORD);
		SendPassword(g_password);
		g_cmd = recieveCommand();
		 if (g_cmd==MATCH)
				{

			 LCD_clearScreen();
			 LCD_displayStringRowColumn(0,1,"password match");
					g_flag=4;
					g_state=2;
					_delay_ms(1000);
				}
		 else if( g_cmd==MISMATCH){
					LCD_clearScreen();
					LCD_displayStringRowColumn(0,1,"password did not match");
					sendCommand(WRONG_PASSWORD);
					g_flag=5;
					_delay_ms(1000);
		         }
	}
	else if (KEYPAD_getPressedKey()=='-'){
		LCD_clearScreen();
		LCD_displayStringRowColumn(0,1,"please enter pass :");
		LCD_moveCursor(3,1);
				fillInPassword(g_password);
				sendCommand(CHECK_PASSWORD);
				SendPassword(g_password);
				if (recieveCommand()==MATCH)
				 {


				     g_state=0;
				     g_flag=0;
				  }
				else if( recieveCommand()==MISMATCH ){
			    	  LCD_clearScreen();
			       	  LCD_displayStringRowColumn(0,1,"password did not match");
					  sendCommand(WRONG_PASSWORD);
			    	  g_flag=5;
					  g_state=1;
			    	  _delay_ms(1000);

			    	  	  }

	}


}
/*Description : to create and store password
 *
 * arguments  : Array to fill the password in
 *
 * returns    : Void
 * */


void fillInPassword(uint8 a_arr[])
{
	uint8 counter=0;


	do {
		g_key=KEYPAD_getPressedKey();
		if( ( g_key != '+' && g_key != '-' && g_key != '*' && g_key != '%' && g_key != '=') || (g_key>= 0 && g_key<= 9) )
		{
			if(counter < PASSWORD_ARRAY_SIZE)
			{
				a_arr[counter]=KEYPAD_getPressedKey();
				LCD_displayCharacter('*');
				counter++;

			}
		}
		_delay_ms(1500);
	} while((g_key != '=') ); /*15 is ASCII FOR '='*/

}

/* Description : send password to the other micro-controller through UART
 *
 * Arguments   : Array of password to send
 *
 * Returns     : Void */

void SendPassword(uint8 a_arr[])
{
	UART_sendByte(MICRO_IS_READY);
	while(UART_recieveByte() != MICRO_IS_READY){};

	for(uint8 i=0;i<PASSWORD_ARRAY_SIZE;i++)
	{
		UART_sendByte(a_arr[i]);
	}
	while(UART_recieveByte() != MICRO_IS_DONE){};
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
/*Description :display alert screen for 1 minute
 *
 * Arguments  : None
 *
 * Returns    : Void */

void ALERTscreen(void)
{

	LCD_clearScreen();
    LCD_displayStringRowColumn(0,1,"ALERT!!!");
	_delay_ms(60000);
		g_wrong=0;
		g_state=1;

}

/*Description :display opening and closing the door screen for 15 seconds
 *
 * Arguments  : None
 *
 * Returns    : Void */

void doorIsOpeningCloseingScreen(void)
{

	LCD_clearScreen();


	LCD_displayStringRowColumn(0,1,"DOOR IS UNLOCKING..");
	_delay_ms(15000);

		LCD_clearScreen();

		_delay_ms(2000);

		LCD_displayStringRowColumn(0,1,"DOOR IS LOCKING..");
		_delay_ms(15000);
		g_state=1;

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

void ifPassSaved(void)
{
	/*
	 * 1. SEND A COMMAND TO CONTROL_ECU TO CHECK IF IT HAS A SAVED PASSWORD IN EEPROM OR NOT YET
	 * 2.WAITING UNTIL THE CONTROL_ECU SEND COMMAND TOO
	 * IF YES_SAVED COMMAD:
	 *                    GO TO STATE 1 FOR THE MAIN FUNCTION
	 * IF NO_SAVED_PASSWORD:
	 *                     GO TO STATE 0 TO ENTER A NEW PASSWORD
	 */
	sendCommand(CHECK_IF_SAVED);
	switch (recieveCommand())

	if(recieveCommand()== YES_SAVED){
		g_state=1;
	}
	else if (recieveCommand()== NOT_SAVED){

		g_state=0;
	}



}



