/*
 * motor.c
 *
 *  Created on: ??þ/??þ/????
 *      Author: Nourhan Ehab
 */

#include<avr/io.h>
#include "motor.h"
#include "std_types.h"
#include "gpio.h"
#include "common_macros.h"

void DCmotor_init(void){
	/*Setup the motor pins as output pins*/
	GPIO_setupPinDirection(PORTB_ID,PIN0_ID,PIN_OUTPUT);
	GPIO_setupPinDirection(PORTB_ID,PIN1_ID,PIN_OUTPUT);
	/*stop the DC motor at the beginning*/
	PORTB = PORTB & 0XFC;

}

void DCmotor_Rotate_CW(void){
	/*rotating the motor CW*/
	SET_BIT(PORTB,0);
	CLEAR_BIT(PORTB,1);

}

void DCmotor_Rotate_A_CW(void){
	/*rotating the motor A_CW*/
	SET_BIT(PORTB,1);
	CLEAR_BIT(PORTB,0);
}

void DCmotor_deinit(void){
	GPIO_writePin(PORTB_ID,PIN0_ID,LOGIC_LOW);
	GPIO_writePin(PORTB_ID,PIN1_ID,LOGIC_LOW);


}
