#include <msp430.h> 

#include "pwm.h"          //TA PWM输出程序库头文件
#include "keyboard.h"	  //按键扫描头文件
#include "led.h"		  //led扫描头文件
#include "adc.h"		  //adc读取头文件
#include "pid.h"

/*
 * main.c
 */
//刷新频率
const int REFRESHFREQ = 100;
//占空比(低电平占比，最大为1000)
unsigned int dutyTime = 500;
//设定的气压值
unsigned int presentPressure = 0, standardPressure = 250;
//这个flag似乎目前没用
extern unsigned char flag;
//检测到的按键值
extern unsigned char key;
//led需要显示的数字
extern unsigned char number[8];

/*
 * 根据对应的按键去执行相应操作
 * 由于需要用到pwmSetPermill接口
 * 所以写在主程序中
 */
void keyEvent()
{
	switch (keyScan())
	{
	case 1:
		pwmSetPermill(2,500);
		break;
	case 2:
		pwmSetPermill(2,0);
		break;
	case 3:
		standardPressure += 10;
		// pwmSetPermill(2, dutyTime);
		break;
	case 4:
		standardPressure -= 10;
		// pwmSetPermill(2, dutyTime);
		break;
	default:
		break;
	}
}

/*
 * 根据当前设定的气压值和ADC读入的气压值
 * 来刷新number数组中待显示的数据
 * 由于需要用到adc接口
 * 所以写在主程序中
 */
void ledUpdate()
{
	unsigned int tmp, i;
	//先更新设定气压值
	tmp = standardPressure;
	for(i = 0; i <= 3; i++)
	{
		number[3 - i] = tmp % 10;
		tmp = tmp / 10;
	}
	//再更新实际气压值
	tmp = ADS7950GetPressure();
	for(i = 0; i <= 3; i++)
	{
		number[7 - i] = tmp % 10;
		tmp = tmp / 10;
	}
}

/*
 * 更新pwm占空比
 * 由于需要用到pid的接口
 * 所以写在主程序中
 */
void pwmUpdate()
{
	dutyTime = dutyTime + PIDControl(standardPressure, ADS7950GetPressure());
	// 更新pwm占空比
	pwmSetPermill(2, dutyTime);
}


void main()
{
    // Stop watchdog timer to prevent time out reset
    WDTCTL = WDTPW + WDTHOLD;
    //ClkInit();

    pwmInit('A',1,'P','P');   //将定时器TA初始化成为PWM发生器; 时钟源=ACLK; 无分频; 通道1和通道2均设为高电平模式。
    pwmSetPeriod(50);        //通道1/2的PWM方波周期均设为50个时钟周期

    //pwmSetDuty(1,200);        //1通道 有效200个时钟周期
    //pwmSetPermill(2,200);     //2通道 20.0%

    keyInit();
    ledInit();
    int i = 0;//用于控制刷新频率

    while(1)
    {
    	//根据按键执行相应操作
    	keyEvent();
    	//根据当前设定的气压值和ADC读入的气压值来刷新number数组中待显示的数据
    	if(i == 0)ledUpdate();
    	//led动态扫描
		ledShow();
		//PID控制
		pwmUpdate();
		//更新刷新频率控制变量i
		i = ++i % REFRESHFREQ;
    }

    //LPM0;
}
