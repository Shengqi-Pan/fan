/*
 * led.h
 *
 *  Created on: 2019年7月22日
 *      Author: Rookie
 */

#ifndef LED_H_
#define LED_H_

#include <msp430.h>

//初始化
void ledInit();
//点亮index下标的LED
void ledLight(unsigned char index);
//熄灭所有LED
void ledExtinguishAll();
//更新LED显示的数据为当前下标的数据
void ledUpdate(unsigned char index);
void ledShow();

#endif /* LED_H_ */
