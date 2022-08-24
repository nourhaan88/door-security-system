/*
 * buzzer.c
 *
 *  Created on: ??þ/??þ/????
 *      Author: Nourhan Ehab
 */
#include "buzzer.h"
#include "gpio.h"
#include "common_macros.h"
#include "std_types.h"
#include "avr\io.h"

/*initialize the buzzer by making the buzzer pin output*/

void buzzer_init(void){
	GPIO_setupPinDirection(BUZZER_PORT_ID,BUZZER_PIN_ID,PIN_OUTPUT);
	GPIO_writePin(BUZZER_PORT_ID,BUZZER_PIN_ID,LOGIC_LOW);
}
/*turn on the buzzer*/
void buzzer_on(void){
	SET_BIT(PORTC,BUZZER_PIN_ID);
}
 /*turn off the buzzer*/

void buzzer_off(void){
	GPIO_writePin(BUZZER_PORT_ID,BUZZER_PIN_ID,LOGIC_LOW);

}
