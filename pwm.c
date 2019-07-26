/*
 * pwm.c
 *
 *  Created on: 2019年7月21日
 *      Author: Rookie
 */
#include"pwm.h"

/*
 * 设置周期
 */
void pwmSetPeriod(unsigned int Period)
{
    TA1CCR0 = Period;
}

void pwmSetDuty(char Channel,unsigned int Duty)
{
    switch(Channel)
    {
        case 1: TA1CCR1=Duty; break;
        case 2: TA1CCR2=Duty; break;
    }
}
/*
* 入口参数：Channel: 当前设置的通道号  1/2
            Percent: PWM有效时间的千分比 (0~1000)
* 出口参数：无
* 说    明: 1000=100.0%  500=50.0% ，依次类推
* 范    例: pwmSetPermill(1,300)设置PWM通道1方波的占空比为30.0%
        pwmSetPermill(2,825)设置PWM通道2方波的占空比为82.5%
            */
void pwmSetPermill(char Channel,unsigned long int Percent)
{
    unsigned long int Period;
    unsigned int Duty;
    Percent = 1000 - Percent;    
    Period = TA1CCR0;
    Duty = Period * Percent / 1000;
    pwmSetDuty(Channel,Duty);
}

char pwmInit(char Clk,char Div,char Mode1,char Mode2)
{
    TA1CTL = 0;                  //清除以前设置
    TA1CTL |= MC_1;              //定时器TA设为增计数模式
    switch(Clk)                 //选择时钟源
    {
        case 'A': case 'a':  TA1CTL|=TASSEL_1; break;    //ACLK
        case 'S': case 's':  TA1CTL|=TASSEL_2; break;    //SMCLK
        case 'E':            TA1CTL|=TASSEL_0; break;    //外部输入(TA1CLK)
        case 'e':            TA1CTL|=TASSEL_3; break;    //外部输入(TA1CLK取反)
        default :  return(0);                           //参数有误
    }
    switch(Div)                 //选择分频系数
    {
        case 1:   TA1CTL|=ID_0; break;   //1
        case 2:   TA1CTL|=ID_1; break;   //2
        case 4:   TA1CTL|=ID_2; break;   //4
        case 8:   TA1CTL|=ID_3; break;   //8
        default :  return(0);           //参数有误
    }
//    switch(Mode1)               //设置PWM通道1的输出模式。
//    {
//        case 'P':case 'p':          //如果设置为高电平模式
//            TA1CCTL1 = OUTMOD_7;     //高电平PWM输出
//            P1SEL |= BIT2;          //从P1.2输出 (不同型号单片机可能不一样)
//            P1DIR |= BIT2;          //从P1.2输出 (不同型号单片机可能不一样)
//            break;
//        case 'N':case 'n':          //如果设置为低电平模式
//            TA1CCTL1 = OUTMOD_3;     //低电平PWM输出
//            P1SEL |= BIT2;          //从P1.2输出 (不同型号单片机可能不一样)
//            P1DIR |= BIT2;          //从P1.2输出 (不同型号单片机可能不一样)
//            break;
//        case '0':case 0:            //如果设置为禁用
//            P1SEL &= ~BIT2;         //P1.2恢复为普通IO口
//            break;
//        default :  return(0);       //参数有误
//    }
	switch(Mode2)                   //设置PWM通道1的输出模式。
	{
		case 'P':case 'p':          //如果设置为高电平模式
			TA1CCTL2 =OUTMOD_7;      //高电平PWM输出
			P2SEL |= BIT3;          //从P1.3输出 (不同型号单片机可能不一样)
			P2DIR |= BIT3;          //从P1.3输出 (不同型号单片机可能不一样)
			break;
		case 'N':case 'n':          //如果设置为低电平模式
			TA1CCTL2 =OUTMOD_3;      //低电平PWM输出
			P2SEL |= BIT3;          //从P1.3输出 (不同型号单片机可能不一样)
			P2DIR |= BIT3;          //从P1.3输出 (不同型号单片机可能不一样)
			break;
		case '0':case 0:            //如果设置为禁用
			P2SEL &= ~BIT3;         //P1.3恢复为普通IO口
			break;
		default :  return(0);       //参数有误
	}
    return(1);
}
