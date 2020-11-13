#include <reg52.h>
#include "LCD1602.h"
#include "DHT11.h"
#include "NRF24L01.h"
#include "delay.h"

sbit LED = P2^0;
uchar TxBuf[8];
extern uchar RH, RL, TH, TL;

void UartInit(){
 	PCON = 0x80;//�������Ƿ�ӱ�
	SCON = 0x50;//������ʽ1
	TMOD = 0x20;//��ʱ��1������8λ�Զ���װģʽ��
	TH1 = 0xFa;
	TL1 = 0XFa;//��ֵ
	EA = 1;//�ܿ���
	TR1 = 1;//��ʱ��1����
}


void Mode()
{
	delay(1500);
	TxPacket(TxBuf);// ����	TxBuf �е�����
	while(CheckACK());
	LED=0;// ����LED������ʾ
	delay(100);      
	LED=1;// Ϩ��LED
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
		TxBuf[0]=RH/10;		 //��ʪ�����ݸ�ֵ������
		TxBuf[1]=RH%10;
		TxBuf[2]=TH/10;		 //���¶����ݸ�ֵ������
		TxBuf[3]=TH%10;
		Mode();
		delay_us(10);	   //��ʱ10us���ȴ��������
		delay_us(10);	   //��ʱ10us���ȴ��������
		
		tm=TxBuf[0]*1000+TxBuf[1]*100+TxBuf[2]*10+TxBuf[3];
		sprintf(str1,"B: T:%2dC   H:%2d%%\n",tm%100,tm/100);
//		delay_us(10);	   //��ʱ10us���ȴ��������
//		delay_us(10);	   //��ʱ10us���ȴ��������	
//		for(i = 0; i < 30; i++)
//		{//���з��͵�ʱ��Ҫ�رմ����ж�λES����Ȼ�Ļ�����ַ���һ���ַ��ͻ�����жϣ����׳���
//			SBUF = *(str + i);
//			while(TI != 1);
//			TI = 0;
//		}

		
		
		LCD_Write_String(0,0,str2);
		LCD_Write_String(0,1,str1);
				         
	}
}