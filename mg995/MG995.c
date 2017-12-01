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

    EA = 1;     //�����ж� 
    while (1)
    {
        ConfigPWM(50, 10);    //Ƶ��50Hz��ռ�ձ�10%
        for (i=0; i<40000; i++);
        ClosePWM();

        ConfigPWM(50, 40);    //Ƶ��50Hz��ռ�ձ�40%
        for (i=0; i<40000; i++);
        ClosePWM();

        ConfigPWM(50, 90);    //Ƶ��50Hz��ռ�ձ�90%
        for (i=0; i<40000; i++);
        ClosePWM();            //�ر�PWM���൱��ռ�ձ�100%

        for (i=0; i<40000; i++);
    }
}


void ConfigPWM(unsigned int fr, unsigned char dc)
{
	unsigned long tmp;
	unsigned int high, low;

	tmp = 11059200/12/fr;  //ʵ�ʼ���ֵ20ms����
	high = (tmp*dc)/100;  //�ߵ�ƽ����ʱ��
	low = tmp - high;	  //�͵�ƽ����ʱ��
	high = 65536 - high + 12; //�ߵ�ƽ��ֵ
	low = 65536 - low + 12;	  //�͵�ƽ��ֵ
	HighRH = (unsigned char)(high >>8);	//�൱�ڳ���256
	HighRL = (unsigned char)high; //ʣ��
	LowRH = (unsigned char)(low >> 8); //�൱�ڳ���256
	LowRL = (unsigned char)low;	//ʣ��
	TMOD &= 0xF0;	//ѡ������ʽ1
	TMOD |= 0x01;
	TH0 = HighRH;	//��ֵ
	TL0 = HighRL;	//��ֵ
	ET0 = 1;		//���ж϶�ʱ��0
	TR0 = 1;		//�򿪶�ʱ��0
	PWMOUT = 1;	
}


void ClosePWM()
{
	TR0 = 0;	  //�ض�ʱ��
	ET0 = 0;	  //�ض�ʱ���ж�
	PWMOUT = 1;
}


void InterruptTimer0() interrupt 1
{
    if (PWMOUT == 1)  //��ǰ���Ϊ�ߵ�ƽʱ��װ�ص͵�ƽֵ������͵�ƽ
    {
        TH0 = LowRH;
        TL0 = LowRL;
        PWMOUT = 0;
    }
    else              //��ǰ���Ϊ�͵�ƽʱ��װ�ظߵ�ƽֵ������ߵ�ƽ
    {
        TH0 = HighRH;
        TL0 = HighRL;
        PWMOUT = 1;
    }
}



///*
//��Ƭ�����ƶ�����ö�����м�λ�ú���ת15�ȣ��ӳ�2ms,��ת15�ȡ�
//���Ϊ0.5~2.5ms������12M��
//*/
//#include<reg52.h>
//#include<intrins.h>
//#define uchar unsigned char
//#define uint  unsigned int
//uint pwm;
//uchar flag;
//sbit P11=P1^1;
//
///*	���ƣ���ʱ����ʼ��
//    ���ܣ�20ms��ʱ��11.0592M����
//	20ms��ֵ=65536-11059200*0.02/12=65536-18432=47104=0xB800
//	TH0=B8
//	TL0=00         
//	0.1ms��ֵ=65536-11059200*0.0001/12=65536-92.16=6443=0xFFA3
//*/
//void Timer0Init()//��ʱ��0��ʼ�� 
//{
//  P11=1;	   //���Ǹߵ�ƽ
//  TMOD=0x01;   //��ʱ���Ĺ�����ʽ1
//  pwm=1500;	  //1.5ms�ߵ�ƽʱ��0��
//  TH0=pwm/256;//��ֵ
//  TL0=pwm%256; //��ֵ
//  EA=1;		 //�����ж�
//  ET0=1;	 //��ʱ��0�ж�����
//  TR0=1;	 //������ʱ��0
//}
//
//void Timer1Init()//��ʱ��1��ʼ�� 2ms  11.0592����
//{
// TMOD=0x01;	 //���ö�ʱ��1Ϊ������ʽ1
// TH1=0xCD;	 //װ��ֵ
// TL1=0xF8;	 //װ��ֵ
// EA=1;		 //�����ж�
// ET1=1;      //��ʱ��1�ж�����
// TR1=1;      //������ʱ��1
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
//void timer0()interrupt 1 using 1	 //���ʣ��20MS�����ڵĵ͵�ƽ
//{
//  P11=!P11;				//��ת
//  pwm=20000-pwm;		//����20ms
//  TH0=pwm/256;			//ʣ���������ڵĵ͵�ƽ
//  TL0=pwm%256;			//��ֵ
//}
//
//void timer1()interrupt 3 using 1	  //����1.75MS�ĸߵ�ƽ
//{
// ET1=0;	 //2ms��رն�ʱ��1�ж�
// ET0=0;	//�ض�ʱ��0
// pwm=1750;	//1.75ms�ߵ�ƽʱ����ת15��
// TH0=pwm/256;  //װ��ֵ
// TL0=pwm%256;  //װ��ֵ
// ET0=1;	// �򿪶�ʱ��0�ж�
// TR0=1;	//������ʱ��0
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