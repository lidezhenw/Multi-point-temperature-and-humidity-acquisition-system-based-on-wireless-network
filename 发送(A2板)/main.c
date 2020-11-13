#include <reg52.h>
#include "LCD1602.h"
#include "DHT11.h"
#include "NRF24L01.h"
#include "delay.h"

sbit LED = P2^0;
uchar TxBuf[8];
extern uchar RH, RL, TH, TL;

void UartInit(){
 	PCON = 0x80;//波特率是否加倍
	SCON = 0x50;//工作方式1
	TMOD = 0x20;//定时器1工作在8位自动重装模式下
	TH1 = 0xFa;
	TL1 = 0XFa;//初值
	EA = 1;//总开关
	TR1 = 1;//定时器1开关
}


void Mode()
{
	delay(1500);
	TxPacket(TxBuf);// 发送	TxBuf 中的数据
	while(CheckACK());
	LED=0;// 点亮LED以做提示
	delay(100);      
	LED=1;// 熄灭LED
	delay(100);										
} 

void main()
{
	char str1[30];
	char str2[30];
	int i; 
	int tm;
	//UartInit();	
	LCD_Init();
	init_NRF24L01();
    sprintf(str2,"1802-JKS-LWC-LDZ");
	while(1)  
	{    
		DHT11_receive(); 
		TxBuf[0]=RH/10;		 //把湿度数据赋值给数组
		TxBuf[1]=RH%10;
		TxBuf[2]=TH/10;		 //把温度数据赋值给数组
		TxBuf[3]=TH%10;
		Mode();
		delay_us(10);	   //延时10us，等待发送完毕
		delay_us(10);	   //延时10us，等待发送完毕
		
		tm=TxBuf[0]*1000+TxBuf[1]*100+TxBuf[2]*10+TxBuf[3];
		sprintf(str1,"B: T:%2dC   H:%2d%%\n",tm%100,tm/100);
//		delay_us(10);	   //延时10us，等待发送完毕
//		delay_us(10);	   //延时10us，等待发送完毕	
//		for(i = 0; i < 30; i++)
//		{//进行发送的时候要关闭串口中断位ES，不然的话会出现发送一个字符就会进入中断，容易出错
//			SBUF = *(str + i);
//			while(TI != 1);
//			TI = 0;
//		}

		
		
		LCD_Write_String(0,0,str2);
		LCD_Write_String(0,1,str1);
				         
	}
}