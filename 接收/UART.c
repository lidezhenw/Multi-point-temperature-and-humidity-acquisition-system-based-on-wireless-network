 
/**********************************************

����ͨ�� ������2400����żУ�龧��12MHz

***********************************************/
#include "Uart.h"
#include <reg52.h>
//#include"pubile.h"
#include"NRF24L01.h"

/******************���ڳ�ʼ��*******************/

#include <reg52.h>
#include <string.h>

void COMConfiguration()
{
    TMOD = 0x20;   //���ü�����������ʽ2
	TH1 = 0xFD;	   //��������ʼֵ���ã�������Ϊ4800
	TL1 = 0xFD;
	TR1 = 1;	   //�򿪼�����
	SCON = 0x50;   //����Ϊ������ʽ1
}
void COM_Tc(char c)
{
    SBUF = c;
	while(!TI);
	TI = 0;
}
void COM_Ts(char s[])
{
    int i;
	int max=strlen(s);
	for(i = 0; i<max;i++)
	COM_Tc(s[i]);
}

