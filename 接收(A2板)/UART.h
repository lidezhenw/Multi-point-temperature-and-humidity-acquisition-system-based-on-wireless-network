/*****************************************

串口函数头文件

*****************************************/
#ifndef _Uart_H_
#define _Uart_H_
/*****************************************/
void COMConfiguration();
void COM_Tc(char c); //传输单个字符
void COM_Ts(char s[]); //传输一个字符串


#endif