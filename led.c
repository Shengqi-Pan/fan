/*
 * led.c
 *
 *  Created on: 2019年7月22日
 *      Author: Rookie
 */

/*
 * led显示模块使用说明
 * 使用时需要先调用ledInit函数以初始化led的输出端口
 * 再改变number数组中的数据从左到右依次从高到低
 * 最后调用ledShow即可
 */

#include"led.h"

unsigned char number[8] = {1, 2, 3, 4, 5, 6, 7, 8};

void ledInit()
{
	//位选口初始化
	P3SEL &= ~(BIT0 + BIT5 + BIT4);   //设为IO口
	P3DIR |= BIT0 + BIT4 + BIT5;   //设为输出

	P11SEL &= ~(BIT0 + BIT1 + BIT2);   //设为IO口
	P11DIR |= BIT0 + BIT1 + BIT2;   //设为输出

	P1SEL &= ~(BIT6 + BIT7);   //设为IO口
	P1DIR |= BIT6 + BIT7;   //设为输出

	//段选口初始化
	P8SEL &= 0x00;	//设为IO口
	P8DIR |= 0xff;	//设为输出
}

void ledLight(unsigned char index)
{
	switch(index)
	{
		case 0://第一位亮（最高位，就是最左边）
			P3OUT &= ~BIT0;//置高
			break;
		case 1:
			P3OUT &= ~BIT5;
			break;
		case 2:
			P3OUT &= ~BIT4;
			break;
		case 3:
			P11OUT &= ~BIT0;
			break;
		case 4:
			P11OUT &= ~BIT1;
			break;
		case 5:
			P11OUT &= ~BIT2;
			break;
		case 6:
			P1OUT &= ~BIT6;
			break;
		case 7:
			P1OUT &= ~BIT7;
			break;
		default:
			break;
	}
}

void ledExtinguishAll()
{
	P3OUT |= BIT0 + BIT5 + BIT4;   //输出1
	P11OUT |= BIT0 + BIT1 + BIT2;   //输出1
	P1OUT |= BIT6 + BIT7;   //输出1
}

/*
 * 将下一个待显示的数据
 * 进行译码并送I/O口
 */
void ledRefresh(unsigned char index)
{
	static unsigned char decode[10] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90};
	//对应进行译码
	P8OUT = decode[number[index]];
}

void ledShow()
{
	static unsigned char index = 0;//用于取数组的下标
	int i;
	for(i = 0; i <8; i++)
	{
		//先熄灭所有LED
		ledExtinguishAll();
		//更新数据
		ledRefresh(index);
		//点亮指定的LED
		ledLight(index);
		//下标顺次移位
		index = (++index % 8);
		__delay_cycles(1000);//延迟0.02ms
	}
	ledExtinguishAll();
}
