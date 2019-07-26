/*
 * pwm.c
 *
 *  Created on: 2019��7��21��
 *      Author: Rookie
 */
#include"pwm.h"

/*
 * ��������
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
* ��ڲ�����Channel: ��ǰ���õ�ͨ����  1/2
            Percent: PWM��Чʱ���ǧ�ֱ� (0~1000)
* ���ڲ�������
* ˵    ��: 1000=100.0%  500=50.0% ����������
* ��    ��: pwmSetPermill(1,300)����PWMͨ��1������ռ�ձ�Ϊ30.0%
        pwmSetPermill(2,825)����PWMͨ��2������ռ�ձ�Ϊ82.5%
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
    TA1CTL = 0;                  //�����ǰ����
    TA1CTL |= MC_1;              //��ʱ��TA��Ϊ������ģʽ
    switch(Clk)                 //ѡ��ʱ��Դ
    {
        case 'A': case 'a':  TA1CTL|=TASSEL_1; break;    //ACLK
        case 'S': case 's':  TA1CTL|=TASSEL_2; break;    //SMCLK
        case 'E':            TA1CTL|=TASSEL_0; break;    //�ⲿ����(TA1CLK)
        case 'e':            TA1CTL|=TASSEL_3; break;    //�ⲿ����(TA1CLKȡ��)
        default :  return(0);                           //��������
    }
    switch(Div)                 //ѡ���Ƶϵ��
    {
        case 1:   TA1CTL|=ID_0; break;   //1
        case 2:   TA1CTL|=ID_1; break;   //2
        case 4:   TA1CTL|=ID_2; break;   //4
        case 8:   TA1CTL|=ID_3; break;   //8
        default :  return(0);           //��������
    }
//    switch(Mode1)               //����PWMͨ��1�����ģʽ��
//    {
//        case 'P':case 'p':          //�������Ϊ�ߵ�ƽģʽ
//            TA1CCTL1 = OUTMOD_7;     //�ߵ�ƽPWM���
//            P1SEL |= BIT2;          //��P1.2��� (��ͬ�ͺŵ�Ƭ�����ܲ�һ��)
//            P1DIR |= BIT2;          //��P1.2��� (��ͬ�ͺŵ�Ƭ�����ܲ�һ��)
//            break;
//        case 'N':case 'n':          //�������Ϊ�͵�ƽģʽ
//            TA1CCTL1 = OUTMOD_3;     //�͵�ƽPWM���
//            P1SEL |= BIT2;          //��P1.2��� (��ͬ�ͺŵ�Ƭ�����ܲ�һ��)
//            P1DIR |= BIT2;          //��P1.2��� (��ͬ�ͺŵ�Ƭ�����ܲ�һ��)
//            break;
//        case '0':case 0:            //�������Ϊ����
//            P1SEL &= ~BIT2;         //P1.2�ָ�Ϊ��ͨIO��
//            break;
//        default :  return(0);       //��������
//    }
	switch(Mode2)                   //����PWMͨ��1�����ģʽ��
	{
		case 'P':case 'p':          //�������Ϊ�ߵ�ƽģʽ
			TA1CCTL2 =OUTMOD_7;      //�ߵ�ƽPWM���
			P2SEL |= BIT3;          //��P1.3��� (��ͬ�ͺŵ�Ƭ�����ܲ�һ��)
			P2DIR |= BIT3;          //��P1.3��� (��ͬ�ͺŵ�Ƭ�����ܲ�һ��)
			break;
		case 'N':case 'n':          //�������Ϊ�͵�ƽģʽ
			TA1CCTL2 =OUTMOD_3;      //�͵�ƽPWM���
			P2SEL |= BIT3;          //��P1.3��� (��ͬ�ͺŵ�Ƭ�����ܲ�һ��)
			P2DIR |= BIT3;          //��P1.3��� (��ͬ�ͺŵ�Ƭ�����ܲ�һ��)
			break;
		case '0':case 0:            //�������Ϊ����
			P2SEL &= ~BIT3;         //P1.3�ָ�Ϊ��ͨIO��
			break;
		default :  return(0);       //��������
	}
    return(1);
}
