/*
 * adc.c
 *
 *  Created on: 2019年7月21日
 *      Author: Rookie
 */

//******************************************************************************
//   Interfacing ADS7950 with MSP430F543xA - USCI_B1, SPI 3-Wire Master Demo
//
//   Description:
//   SPI master talks to SPI slave using 3-wire mode. Operation
//   in Manual mode of ADS7950 and read data from channel 3 for test.
//   ACLK = ~32.768kHz, MCLK = SMCLK = DCO ~ 1048kHz.  BRCLK = SMCLK/4
//
//
//                   MSP430F5438A
//                 -----------------
//             /|\|                 |
//              | |                 |
//              --|RST              |
//                |                 |
//                |             P3.7|-> Data Out (UCB1SIMO)
//                |                 |
//                |             P5.4|<- Data In (UCB1SOMI)
//                |                 |
//  		\CS <-|P1.1         P5.5|-> Serial Clock Out (UCB1CLK)
//
//
//   Shuyang Zhong
//   U-Program China, Texas Instruments Inc.
//   Mar 2013
//   Built with CCE Version: 5.1.1
//******************************************************************************


#include "adc_drive.h"


/****************************************************************************
 ADS7950             MSP430

 SCLK| <-----------| P5.5/UCB1CLK
 SDO | ------------| P5.4/UCB1SOMI
 SDI | <-----------| P3.7/UCB1SIMO
 CS  | <-----------| P1.1          P3.6/UCB1STE
     |             |
*****************************************************************************/
#define SCLK         (BIT5)    //sclk
#define SDO          (BIT4)    //sdo
#define SDI          (BIT7)    //sdi
#define CS           (BIT1)    //cs

#define ADS7950_SCLK_HIGH  (P5OUT |= SCLK)
#define ADS7950_SCLK_LOW   (P5OUT &= ~SCLK)
#define ADS7950_SDI_HIGH   (P3OUT |= SDI)
#define ADS7950_SDI_LOW    (P3OUT &= ~SDI)
#define ADS7950_CS_HIGH    (P1OUT |= CS)
#define ADS7950_CS_LOW     (P1OUT &= ~CS)
#define ADS7950_IN         (P5IN)

#define SET_PIN_DIR_ADS7950_SCLK_OUT (P5DIR |= SCLK)
#define SET_PIN_DIR_ADS7950_SDO_IN   (P5DIR &= ~SDO)
#define SET_PIN_DIR_ADS7950_SDI_OUT  (P3DIR |= SDI)
#define SET_PIN_DIR_ADS7950_CS_OUT   (P1DIR |= CS)

#define MODE_MANUAL_CH0        0x1800
#define MODE_MANUAL_CH1        0x1880
#define MODE_MANUAL_CH2        0x1900
#define MODE_MANUAL_CH3        0x1980


unsigned int ADS7950GetPressure(void)
{
	volatile unsigned int spi_result;
	volatile unsigned int sample_result;
	volatile unsigned int sample_channel;
	volatile unsigned long int voltage, pressure_display;
	//unsigned int ModeControlRegister;

	__delay_cycles(100);
	P1DIR |= BIT2+BIT3+BIT4+BIT5;
	P1OUT |= BIT2+BIT3+BIT4+BIT5;

	ADS7950_PORT_Init();

	spi_result = ADS7950_ReadResultAndSetNextSample(MODE_MANUAL_CH3);	//读出ADC的返回值
	sample_result = spi_result & 0xFFF;
	sample_channel = spi_result >> 12;

	//voltage =  sample_result[times]*2500/4096    //--->2500/4096=0.610
	voltage = sample_result * (unsigned long int)2500/4096;   //进入ADC的电压值，单位是mV
	pressure_display = voltage / 3;		//气压值整定，如果调节过差分放大电路的电位器，该值需要重新整定

	__delay_cycles(1000);

    //_NOP(); //set breakpoint here, watch the variables
    return pressure_display;
}

/*
 * ADS7950端口初始化
 */
void ADS7950_PORT_Init()
{
	SET_PIN_DIR_ADS7950_SCLK_OUT; //set pin output,connect to ads7950 sclk
	SET_PIN_DIR_ADS7950_SDI_OUT;  //set pin output,connect to ads7950 sdi
    SET_PIN_DIR_ADS7950_SDO_IN;   //set pin input, connect to ads7950 sdo
    SET_PIN_DIR_ADS7950_CS_OUT;   //set pin output,connect to ads7950 cs

}
/*
 * 模拟SPI串口通信，读写ADS7960
 *
 * SDO--The first bit clocks out at the falling edge of  CS;
 *    --next 15 bits clocks out at the falling edge of SCLK;
 * SDI--latch data on the rising edge of SCLK;
 *
 * input: 送至ADS7950的16位SDI
 * output: 16位数据，包含4路通道选择地址和12位AD转换结果
 * */

unsigned int ADS7950_ReadResultAndSetNextSample(unsigned int uiSendData)
{
	  unsigned int uiSendTimes,result;

	  result=0;

	  ADS7950_CS_HIGH;
	  ADS7950_SCLK_LOW;
	  __delay_cycles(30);//delay 20ns at least
	  ADS7950_CS_LOW;
	  __delay_cycles(30);//delay 40ns at least for the first SDO

	  for(uiSendTimes = 16; uiSendTimes > 0; uiSendTimes--)
	  {
		  //prepare one bit to send
		  //高位先行原则
		  if((uiSendData & 0x8000)== 0x8000)
			  ADS7950_SDI_HIGH;
	      else
	    	  ADS7950_SDI_LOW;

		  uiSendData <<=1;

		  //read one bit
		  result<<=1;
		  if(( ADS7950_IN & SDO)== SDO) result |= 0x0001;

		  //delay 20ns at least to set sclk high
		  __delay_cycles(50);

		  ADS7950_SCLK_HIGH; //rising edge of SCLK, latch one bit
		  __delay_cycles(50);
		  ADS7950_SCLK_LOW;  //falling edge of SCLK, SDO clocks on one bit
		  __delay_cycles(50);
	  }

	  ADS7950_CS_HIGH;
	 // __delay_cycles(50);

	  return result;

}

//int main(void)
//{
//	unsigned int voltage_display;
//	voltage_display = ADS7950GetVoltage()
//}
