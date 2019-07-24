#include <msp430.h> 

#include "pwm.h"          //TA PWM��������ͷ�ļ�
#include "keyboard.h"	  //����ɨ��ͷ�ļ�
#include "led.h"		  //ledɨ��ͷ�ļ�
#include "adc.h"		  //adc��ȡͷ�ļ�
#include "pid.h"

/*
 * main.c
 */
//ˢ��Ƶ��
const int REFRESHFREQ = 30;
const int PIDPERIOD = 2000;
//ռ�ձ�(�͵�ƽռ�ȣ����Ϊ1000)
unsigned int dutyTime = 500;
//�趨����ѹֵ
unsigned int presentPressure = 0, standardPressure = 250;

unsigned int state = 0;

/*
 * �ĸ�������
 * �ⲿ�жϴ�������
 */
#pragma vector = PORT2_VECTOR                      //�˿�2���ж�����
__interrupt void PORT2_ISR(void)
{
	P2IODect(&state, &standardPressure);       //�����¼���������
	P2IFG &= ~(BIT4 + BIT5 + BIT6 + BIT7);	//����жϱ�־
}

/*
 * ����pwmռ�ձ�
 * ������Ҫ�õ�pid�Ľӿ�
 * ����д����������
 */
void pwmUpdate()
{
	dutyTime = dutyTime + PIDControl(standardPressure, ADS7950GetPressure());
	// ����pwmռ�ձ�
	pwmSetPermill(2, dutyTime);
}


/*
 * ��ʱ���жϳ�ʼ��
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

    TA0InterInit();		//��ʱ��TIMERA0�ж�
    pwmInit('A',1,'P','P');   //����ʱ��TA��ʼ����ΪPWM������; ʱ��Դ=ACLK; �޷�Ƶ; ͨ��1��ͨ��2����Ϊ�ߵ�ƽģʽ��
    pwmSetPeriod(50);        //ͨ��1/2��PWM�������ھ���Ϊ50��ʱ������

    IOInterruptInit();//�����жϳ�ʼ��
    ledInit();
    pid_init();
    _enable_interrupts(); //�����ж�
    int i = 0;//���ڿ���ˢ��Ƶ��

    while(1)
    {
    	ledShow();
    	//���ݵ�ǰ�趨����ѹֵ��ADC�������ѹֵ��ˢ��number�����д���ʾ������
    	if(i == 0)ledUpdatePresent(ADS7950GetPressure());
    	ledUpdateSet(standardPressure);
    	//led��̬ɨ��
		ledShow();
		//����ˢ��Ƶ�ʿ��Ʊ���i
		i = ++i % REFRESHFREQ;
    }

    //LPM0;
}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void TIMER_A0(void)
{
	//PID����
	ledShow();
	if (!state)
		pwmUpdate();
}
