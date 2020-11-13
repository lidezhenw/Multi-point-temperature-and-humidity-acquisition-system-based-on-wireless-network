#include <reg52.h>
#include <stdio.h>
#include "NRF24L01.h"
#include "Delay.h"
#include "1602.h"
#include <string.h>
#include "Uart.h"

sbit L1=P2^0;//灯
sbit L2=P2^1;//灯
sbit L3=P2^2;
int tm;
int tm1;

unsigned char RxBuf0[8];//RF 接收缓存温度用于显示
unsigned char RxBuf1[8];//RF 接收缓存温度用于显示
void main()
{
	char str[25];
	char str1[25];
	char str2[20];
 	init_NRF24L01();
 	COMConfiguration();
    LCD_Init();
	DelayMs(20);
	LCD_Clear();	
 	while (1)
  { 
		RX_Mode();
		L3=0;			
		if(RxPacket(RxBuf0)==0x00)
		 {
	  		L1=0;
	  		DelayMs(100);
	  		L1=1;
	  		DelayMs(10);
			tm=RxBuf0[0]*1000+RxBuf0[1]*100+RxBuf0[2]*10+RxBuf0[3];
			sprintf(str,"A:  T:%2dC  H:%2d%%\n",tm%100,tm/100);
			COM_Ts(str);			
			LCD_Write_String(0,0,str);	    			
			}	
		DelayMs(100);
		if(RxPacket(RxBuf1)==0x02)
		{
	  		L2=0;
	  		DelayMs(100);
	  		L2=1;
	  		DelayMs(100);
			tm1=RxBuf1[0]*1000+RxBuf1[1]*100+RxBuf1[2]*10+RxBuf1[3];
			sprintf(str1,"B:  T:%2dC  H:%2d%%\n",tm1%100,tm1/100);
			COM_Ts(str1);			
			LCD_Write_String(0,1,str1);	    		
		}
		
		
		DelayMs(10);
	}
}


