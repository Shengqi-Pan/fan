/*
 * led.h
 *
 *  Created on: 2019Äê7ÔÂ22ÈÕ
 *      Author: Rookie
 */

#ifndef LED_H_
#define LED_H_

#include <msp430.h>

void ledInit();
void ledLight(unsigned char index);
void ledExtinguishAll();
void ledRefresh(unsigned char index);
void ledShow();
void ledUpdateSet(unsigned int standardPressure);
void ledUpdatePresent(unsigned int presentPressure);

#endif /* LED_H_ */
