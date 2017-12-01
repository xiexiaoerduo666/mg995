#include <reg52.h>
sbit PWMOUT = P0^0;
unsigned char HighRH = 0;
unsigned char HighRL = 0;
unsigned char LowRH = 0;
unsigned char LowRL = 0;
void ConfigPWM(unsigned int fr, unsigned char dc);
void ClosePWM();

void main()
{
    unsigned int i;

    EA = 1;     //开总中断 
    while (1)
    {
        ConfigPWM(50, 10);    //频率50Hz，占空比10%
        for (i=0; i<40000; i++);
        ClosePWM();

        ConfigPWM(50, 40);    //频率50Hz，占空比40%
        for (i=0; i<40000; i++);
        ClosePWM();

        ConfigPWM(50, 90);    //频率50Hz，占空比90%
        for (i=0; i<40000; i++);
        ClosePWM();            //关闭PWM，相当于占空比100%

        for (i=0; i<40000; i++);
    }
}


void ConfigPWM(unsigned int fr, unsigned char dc)
{
	unsigned long tmp;
	unsigned int high, low;

	tmp = 11059200/12/fr;  //实际计数值20ms周期
	high = (tmp*dc)/100;  //高电平周期时间
	low = tmp - high;	  //低电平周期时间
	high = 65536 - high + 12; //高电平初值
	low = 65536 - low + 12;	  //低电平初值
	HighRH = (unsigned char)(high >>8);	//相当于除以256
	HighRL = (unsigned char)high; //剩余
	LowRH = (unsigned char)(low >> 8); //相当于除以256
	LowRL = (unsigned char)low;	//剩余
	TMOD &= 0xF0;	//选择工作方式1
	TMOD |= 0x01;
	TH0 = HighRH;	//初值
	TL0 = HighRL;	//初值
	ET0 = 1;		//开中断定时器0
	TR0 = 1;		//打开定时器0
	PWMOUT = 1;	
}


void ClosePWM()
{
	TR0 = 0;	  //关定时器
	ET0 = 0;	  //关定时器中断
	PWMOUT = 1;
}


void InterruptTimer0() interrupt 1
{
    if (PWMOUT == 1)  //当前输出为高电平时，装载低电平值并输出低电平
    {
        TH0 = LowRH;
        TL0 = LowRL;
        PWMOUT = 0;
    }
    else              //当前输出为低电平时，装载高电平值并输出高电平
    {
        TH0 = HighRH;
        TL0 = HighRL;
        PWMOUT = 1;
    }
}



///*
//单片机控制舵机，让舵机到中间位置后，左转15度，延迟2ms,右转15度。
//舵机为0.5~2.5ms，晶振12M。
//*/
//#include<reg52.h>
//#include<intrins.h>
//#define uchar unsigned char
//#define uint  unsigned int
//uint pwm;
//uchar flag;
//sbit P11=P1^1;
//
///*	名称：定时器初始化
//    功能：20ms定时，11.0592M晶振
//	20ms初值=65536-11059200*0.02/12=65536-18432=47104=0xB800
//	TH0=B8
//	TL0=00         
//	0.1ms初值=65536-11059200*0.0001/12=65536-92.16=6443=0xFFA3
//*/
//void Timer0Init()//定时器0初始化 
//{
//  P11=1;	   //先是高电平
//  TMOD=0x01;   //定时器的工作方式1
//  pwm=1500;	  //1.5ms高电平时是0度
//  TH0=pwm/256;//初值
//  TL0=pwm%256; //初值
//  EA=1;		 //开总中断
//  ET0=1;	 //定时器0中断允许
//  TR0=1;	 //启动定时器0
//}
//
//void Timer1Init()//定时器1初始化 2ms  11.0592晶振
//{
// TMOD=0x01;	 //设置定时器1为工作方式1
// TH1=0xCD;	 //装初值
// TL1=0xF8;	 //装初值
// EA=1;		 //开总中断
// ET1=1;      //定时器1中断允许
// TR1=1;      //启动定时器1
//}
//
//
//
//void main()
//{	 
//   Timer0Init();
//   Timer1Init();
//	while(1);
//}
//
//
//void timer0()interrupt 1 using 1	 //完成剩余20MS周期内的低电平
//{
//  P11=!P11;				//反转
//  pwm=20000-pwm;		//周期20ms
//  TH0=pwm/256;			//剩余是周期内的低电平
//  TL0=pwm%256;			//初值
//}
//
//void timer1()interrupt 3 using 1	  //进入1.75MS的高电平
//{
// ET1=0;	 //2ms后关闭定时器1中断
// ET0=0;	//关定时器0
// pwm=1750;	//1.75ms高电平时是右转15度
// TH0=pwm/256;  //装初值
// TL0=pwm%256;  //装初值
// ET0=1;	// 打开定时器0中断
// TR0=1;	//开启定时器0
//}
//
//
//




// void Delay10000ms()		//@11.0592MHz
//{
//	unsigned char i, j, k;
//
//	_nop_();
//	i = 71;
//	j = 10;
//	k = 171;
//	do
//	{
//		do
//		{
//			while (--k);
//		} while (--j);
//	} while (--i);
//}
//
//  void Delay1500us()		//@11.0592MHz
//{
//	unsigned char i, j;
//
//	i = 3;
//	j = 173;
//	do
//	{
//		while (--j);
//	} while (--i);
//}
//
//void Delay18500us()		//@11.0592MHz
//{
//	unsigned char i, j;
//
//	_nop_();
//	i = 34;
//	j = 39;
//	do
//	{
//		while (--j);
//	} while (--i);
//}
//
//void Delay2000us()		//@11.0592MHz
//{
//	unsigned char i, j;
//
//	_nop_();
//	i = 4;
//	j = 146;
//	do
//	{
//		while (--j);
//	} while (--i);
//}
// 
//void Delay18000us()		//@11.0592MHz
//{
//	unsigned char i, j;
//
//	i = 33;
//	j = 66;
//	do
//	{
//		while (--j);
//	} while (--i);
//}