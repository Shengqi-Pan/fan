/*
 * keyboard.c
 *
 *  Created on: 2019年7月22日
 *      Author: Rookie
 */

#include "keyboard.h"
#include "pwm.h"
#include "pid.h"

extern float e0, e1, e2;



void P2IODect(unsigned int* pstate, unsigned int* pstandardPressure)
{
	unsigned int Push_Key = 0;
	//-----排除输出IO的干扰后，锁定唯一被触发的中断标志位-----
	Push_Key = P2IFG & (~P2DIR);
	//-----延时一段时间，避开机械抖动区域-----
	__delay_cycles(10000);   //消抖延时
	//----判断按键状态是否与延时前一致-----
	if((P2IN & Push_Key) == 0)          //如果该次按键确实有效
	switch (Push_Key)
	{
	case BIT4:
		pwmSetPermill(2, 500);
		*pstate = 0;
		break;
	case BIT5:
		pwmSetPermill(2,0);
		e0 = 0;
		e1 = 0;
		e2 = 0;
		*pstate = 1;
		break;
	case 64:
		*pstandardPressure += 10;
		break;
	case BIT7:
		*pstandardPressure -= 10;
		break;
	default:
		break;
	}
}
