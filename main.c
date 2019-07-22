#include <msp430.h> 

#include "pwm.h"          //TA PWM输出程序库头文件
#include "keyboard.h"	  //按键扫描头文件
#include "led.h"

/*
 * main.c
 */

unsigned long int dutyTime = 200;
extern unsigned char flag;
extern unsigned char key;
extern unsigned char number[8];

void main()
{
    // Stop watchdog timer to prevent time out reset
    WDTCTL = WDTPW + WDTHOLD;
    //ClkInit();

    pwmInit('A',1,'P','P');   //将定时器TA初始化成为PWM发生器
                  //时钟源=ACLK ; 无分频;  通道1和通道2均设为高电平模式。
    pwmSetPeriod(50);        //通道1/2的PWM方波周期均设为50个时钟周期
    //pwmSetDuty(1,200);        //1通道 有效200个时钟周期
    //pwmSetPermill(2,200);     //2通道 20.0%

    keyInit();
    while(1)
    {
		switch (keyScan())
		{
		case 1:
			pwmSetPermill(2,500);
			break;
		case 2:
			pwmSetPermill(2,1000);
			break;
		case 3:
			dutyTime -= 10;
			pwmSetPermill(2, dutyTime);
			break;
		case 4:
			dutyTime += 10;
			pwmSetPermill(2, dutyTime);
			break;
		default:
			break;
		}
    }

    //LPM0;
}
