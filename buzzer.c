/*
 * buzzer.c
 *
 *  Created on: 2019年7月25日
 *      Author: Rookie
 */

#include "buzzer.h"

/*
 * 初始化
 */
void buzzerInit()
{
	P9SEL &= ~BIT6;   //设为IO口
	P9DIR |= BIT6;   //设为输出
}

void beep()
{
//	long int i = 100000;
	P9OUT |= BIT6;
//	while(i--);
	__delay_cycles(500000);//延迟10ms
	P9OUT &= ~BIT6;
}
