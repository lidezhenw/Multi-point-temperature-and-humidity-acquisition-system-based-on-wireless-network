/*****************************************

��ʱ����

**************************************/
#include "Delay.h"
/***************************��ʱ����****************/
//T=tx2+5 uS
void Delay(unsigned int t)
{
 while(--t);
}


void DelayMs (unsigned int t)
{ 
 while(t--)//t>0��ѭ��
  {
     //������ʱ1mS
    Delay(245);
    Delay(245);
  }
}