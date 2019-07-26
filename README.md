# BladelessFanController
a BladelessFanController based on MSP430seriesMCU produced by TI

本是项目基于TI的MSP430单片机开发的无叶风扇控制器

浙江大学电气工程学院爱迪生班小学期”电子系统综合设计“课程设计



## 系统时钟参数



## PWM波发生

### 计数器初始化

计数器初始化主要包括计数器选择、计数器计数模式的初始化、时钟源的初始化、分频系数初始化以及输出模式的初始化。初始化参数如下表所示:

| 寄存器   | 寄存器功能     | 初始化参数   | 初始化参数意义        |
| -------- | -------------- | ------------ | --------------------- |
| TA1CTL   | 计数器控制     | = 0          | 清除之前的设置        |
|          |                | \|= MC_1     | 设为增计数模式        |
|          |                | \|= TASSEL_1 | 时钟源采用ACLK        |
|          |                | \|= ID_0     | 分频系数为1           |
| TA1CCTL2 | 计数器比较控制 | = OUTMOD_7   | PWM波以高电平模式输出 |

#### 计数器选择

PWM波的发生采用的是TA1，增计数模式

```C
TA1CTL = 0;                  //清除以前设置
TA1CTL |= MC_1;              //定时器TA设为增计数模式
```

#### 时钟源选择

在PWM初始化函数中加入了时钟源选择接口，可以在调用时钟初始化函数时通过接口灵活改变时钟源

```c
switch(Clk)                 //选择时钟源
{
    case 'A': case 'a':  TA1CTL|=TASSEL_1; break;    //ACLK
    case 'S': case 's':  TA1CTL|=TASSEL_2; break;    //SMCLK
    case 'E':            TA1CTL|=TASSEL_0; break;    //外部输入(TA1CLK)
    case 'e':            TA1CTL|=TASSEL_3; break;    //外部输入(TA1CLK取反)
    default :  return(0);                           //参数有误
}
```

在主程序初调用PWM始化函数时，我将时钟源设为ACLK，32.768kHz

#### 分频系数选择

和时钟源的选择相同，PWM初始化函数中也有分频系数选择接口，可以在调用时钟初始化函数时通过接口灵活改变分频系数

```c
switch(Div)                 //选择分频系数
{
    case 1:   TA1CTL|=ID_0; break;   //1
    case 2:   TA1CTL|=ID_1; break;   //2
    case 4:   TA1CTL|=ID_2; break;   //4
    case 8:   TA1CTL|=ID_3; break;   //8
    default :  return(0);           //参数有误
}
```

在主程序调用PWM初始化函数时，我将分频系数Div设为1

#### 输出模式选择

同样的，函数中也有PWM输出模式选择的接口，$P$表示高电平输出模式，$N$表示低电平输出模式，0表示不输出PWM

```c
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
        P2DIR |= BIT3;
        P2OUT |= BIT3;
		break;
	default :  return(0);       //参数有误
}
```

### PWM调节



#### PWM频率设置



#### PWM占空比设置



## ADC读取



## pid算法