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
const int REFRESHFREQ = 30;
//占空比(低电平占比，最大为1000)
unsigned int dutyTime = 500;
//设定的气压值
unsigned int presentPressure = 0, standardPressure = 250;

unsigned int state = 0;

/*
 * 四个按键的
 * 外部中断处理程序
 */
#pragma vector = PORT2_VECTOR                      //端口2的中断向量
__interrupt void PORT2_ISR(void)
{
	P2IODect(&state, &standardPressure);       //调用事件处理函数
	P2IFG &= ~(BIT4 + BIT5 + BIT6 + BIT7);	//清除中断标志
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


/*
 * 定时器中断初始化
 */
void TA0InterInit()
{
	TA0CCR0 = PIDPERIOD;
	TA0CCTL0 = CCIE;

	TA0CTL |= TASSEL_1 + MC_1 + TACLR;

	_enable_interrupts();
}

void main()
{
    // Stop watchdog timer to prevent time out reset
    WDTCTL = WDTPW + WDTHOLD;
    //ClkInit();

    TA0InterInit();		//定时器TIMERA0中断
    pwmInit('A',1,'P','P');   //将定时器TA初始化成为PWM发生器; 时钟源=ACLK; 无分频; 通道1和通道2均设为高电平模式。
    pwmSetPeriod(50);        //通道1/2的PWM方波周期均设为50个时钟周期

    IOInterruptInit();//按键中断初始化
    ledInit();
    pid_init();
    _enable_interrupts(); //开总中断
    int i = 0;//用于控制刷新频率

    while(1)
    {
    	//根据当前设定的气压值和ADC读入的气压值来刷新number数组中待显示的数据
    	if(i == 0)ledUpdatePresent(ADS7950GetPressure());
    	ledUpdateSet(standardPressure);
    	//led动态扫描
		ledShow();
		//更新刷新频率控制变量i
		i = ++i % REFRESHFREQ;
    }

    //LPM0;
}

#pragma vector = TIMER0_A1_VECTOR
__interrupt void TIMER_A1(void)
{
	//PID控制
	if (!state)
		pwmUpdate();
}
