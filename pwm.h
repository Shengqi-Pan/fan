/*
 * pwm.h
 *
 *  Created on: 2019Äê7ÔÂ21ÈÕ
 *      Author: Rookie
 */

#ifndef PWM_H_
#define PWM_H_

#include <msp430.h>

void pwmSetPeriod(unsigned int Period);
void pwmSetDuty(char Channel,unsigned int Duty);
void pwmSetPermill(char Channel,unsigned long int Percent);
char pwmInit(char Clk,char Div,char Mode1,char Mode2);

#endif /* PWM_H_ */
