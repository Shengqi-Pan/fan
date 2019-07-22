#include <msp430.h> 

/*
 * main.c
 */
#include "pwm.h"          //TA PWM输出程序库头文件
/*
void ClkInit()                 //时钟1MHz
{
  DCOCTL|=CAL_DCO_1MHZ;
  BCSCTL1|=CAL_BC1_1MHZ;
}
*/

void main()
{
    // Stop watchdog timer to prevent time out reset
    WDTCTL = WDTPW + WDTHOLD;
    //ClkInit();

    pwmInit('A',1,'P','P');   //将定时器TA初始化成为PWM发生器
                  //时钟源=ACLK ; 无分频;  通道1和通道2均设为高电平模式。
    pwmSetPeriod(500);        //通道1/2的PWM方波周期均设为500个时钟周期
    pwmSetDuty(1,200);        //1通道 有效200个时钟周期
    pwmSetPermill(2,200);     //2通道 20.0%

    LPM0;
}
