/*
 * motor.h
 *
 *  Created on: ??þ/??þ/????
 *      Author: Nourhan Ehab
 */

#ifndef MOTOR_H_
#define MOTOR_H_

#include "std_types.h"




/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
/*
description :
initialization of motor by setting pins directions and stop it at the beginig
*/
void DCmotor_init(void);
/*
description :
function responsible for rotating the motor CW or A_CW or stop it based on the state input
and to send the duty cycle value to the PWM driver based on the required speed value*/


void DCmotor_Rotate_CW(void);

void DCmotor_Rotate_A_CW(void);

void DCmotor_deinit (void);

#endif /* MOTOR_H_ */
