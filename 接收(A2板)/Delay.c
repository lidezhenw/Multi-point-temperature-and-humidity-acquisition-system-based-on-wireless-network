/*****************************************

延时函数

**************************************/
#include "Delay.h"
/***************************延时函数****************/
//T=tx2+5 uS
void Delay(unsigned int t)
{
 while(--t);
}


void DelayMs (unsigned int t)
{ 
 while(t--)//t>0则循环
  {
     //大致延时1mS
    Delay(245);
    Delay(245);
  }
}