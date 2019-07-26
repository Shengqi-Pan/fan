#include <msp430.h> 

#include "pwm.h"          //TA PWM��������ͷ�ļ�
#include "keyboard.h"	  //����ɨ��ͷ�ļ�
#include "led.h"		  //ledɨ��ͷ�ļ�
#include "adc.h"		  //adc��ȡͷ�ļ�
#include "pid.h"		  //pid����ͷ�ļ�
#include "buzzer.h"		  //������ͷ�ļ�

/*
 * main.c
 */
//ˢ��Ƶ��
const int REFRESHFREQ = 20;
const int PIDPERIOD = 150;
//ռ�ձ�(�͵�ƽռ�ȣ����Ϊ1000)
int dutyTime = 500;
//�趨����ѹֵ
unsigned int presentPressure = 0, standardPressure = 350;

unsigned int state = 0;
unsigned int j, a[1000];


/*
 * �ĸ�������
 * �ⲿ�жϴ�������
 */
#pragma vector = PORT2_VECTOR                      //�˿�2���ж�����
__interrupt void PORT2_ISR(void)
{
	P2IODect(&state, &standardPressure);       //�����¼���������
	if (standardPressure > 60000)  standardPressure = 0;
	if (standardPressure >= 450)  standardPressure = 450;
	beep();
	P2IFG &= ~(BIT4 + BIT5 + BIT6 + BIT7);	//����жϱ�־
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

/*
 * �����жϳ�ʼ��
 */
void IOInterruptInit()
{
	P2DIR &= ~(BIT4 + BIT5 + BIT6 + BIT7);	//����Ϊ����
	P2REN |= BIT4 + BIT5 + BIT6 + BIT7;	//���õ���
	P2OUT |= BIT4 + BIT5 + BIT6 + BIT7;	//������������
	P2IES |= BIT4 + BIT5 + BIT6 + BIT7;	//������ʽ��Ϊ�½��ش���
	P2IE |= BIT4 + BIT5 + BIT6 + BIT7;	//�ж�ʹ��
	P2IFG &= ~(BIT4 + BIT5 + BIT6 + BIT7);	//����жϱ�־
}


void main()
{
    // Stop watchdog timer to prevent time out reset
    WDTCTL = WDTPW + WDTHOLD;

    initClock();
    TA0InterInit();		//��ʱ��TIMERA0�ж�
    pwmInit('A',1,'P','P');   //����ʱ��TA��ʼ����ΪPWM������; ʱ��Դ=ACLK; �޷�Ƶ; ͨ��1��ͨ��2����Ϊ�ߵ�ƽģʽ��
    pwmSetPeriod(50);        //ͨ��1/2��PWM�������ھ���Ϊ50��ʱ������

    IOInterruptInit();//�����жϳ�ʼ��
    ledInit();
    pid_init();
    buzzerInit();
    _enable_interrupts(); //�����ж�
    int i = 0;//���ڿ���ˢ��Ƶ��

    while(1)
    {
    	ledShow();
    	//���ݵ�ǰ�趨����ѹֵ��ADC�������ѹֵ��ˢ��number�����д���ʾ������
    	if(i == 0)	ledUpdatePresent(ADS7950GetPressure());
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
	unsigned int pre;
	//PID����
	ledShow();
	if (!state)
	{
		pre =  ADS7950GetPressure();
		pwmUpdate(&dutyTime, standardPressure, pre);
		if (j < 1000)
		{
			j++;
			a[j] = pre;
		}
	}

}
