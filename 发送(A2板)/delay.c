#include "delay.h"

void delay_us(uint i)   //��� 0us
{
    uchar a,b,c;
	for(c=i;c>0;c--)
	{
	    for(b=1;b>0;b--)
	        for(a=32;a>0;a--);
	}
}

void delay(uchar ms)
{         
	uchar i;    
    while(ms--)   
    {      
  		for(i=0;i<250;i++);  
    } 
}

void delay1()//��ʱ10us 
{  
    uchar i;
    i--;  
    i--;  
    i--;  
    i--;  
    i--;  
    i--;  
}

void longdelay(uchar s) //����ʱ 
{   
	while(s--)
    {      
    	delay(60);
    }
}