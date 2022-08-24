/*
 * buzzer.h
 *
 *  Created on: ??þ/??þ/????
 *      Author: Ehab
 */

#ifndef BUZZER_H_
#define BUZZER_H_

#include "gpio.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

#define BUZZER_PORT_ID   PORTC_ID
#define BUZZER_PIN_ID    PIN2_ID

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*initialize buzzer */

void buzzer_init(void);

/*turn on the buzzer*/

void buzzer_on(void);

/*turn off the buzzer*/

void buzzer_off(void);

#endif /* BUZZER_H_ */
