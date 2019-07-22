/*
 * keyboard.h
 *
 *  Created on: 2019Äê7ÔÂ22ÈÕ
 *      Author: Rookie
 */

#ifndef KEYBOARD_H_
#define KEYBOARD_H_

#include <msp430.h>

void delay_ms(unsigned int MS);
void keyInit();
char keyScan();
void ledScan();


#endif /* KEYBOARD_H_ */
