 
/**********************************************

串口通信 波特率2400无奇偶校验晶振12MHz

***********************************************/
#include "Uart.h"
#include <reg52.h>
//#include"pubile.h"
#include"NRF24L01.h"

/******************串口初始化*******************/

#include <reg52.h>
#include <string.h>

void COMConfiguration()
{
    TMOD = 0x20;   //设置计数器工作方式2
	TH1 = 0xFD;	   //计数器初始值设置，波特率为4800
	TL1 = 0xFD;
	TR1 = 1;	   //打开计数器
	SCON = 0x50;   //设置为工作方式1
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

