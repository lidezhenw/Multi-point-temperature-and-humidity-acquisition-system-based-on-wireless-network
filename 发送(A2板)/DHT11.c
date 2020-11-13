#include "DHT11.h"
#include "intrins.h"

uchar RH, RL, TH, TL;

void DHT11_delay20ms(void)   //误差 0us
{
    unsigned char a,b;
    for(b=215;b>0;b--)
        for(a=45;a>0;a--);
    _nop_();  //if Keil,require use intrins.h
    _nop_();  //if Keil,require use intrins.h
}

void DHT11_delay10us(void)   //误差 0us
{
    unsigned char a,b;
    for(b=1;b>0;b--)
        for(a=2;a>0;a--);
}

void DHT11_START()//开始信号 
{  
	DATA=1; 
	DHT11_delay10us();  
	DATA=0;   
	DHT11_delay20ms();//>18ms  
	DATA=1;   
	DHT11_delay10us();//20-40us  
	DHT11_delay10us();  
	DHT11_delay10us();   
}

uchar DHT11_receive_byte()//接收一个字节 
{ 
  uchar data_byte;	
	uchar i,temp;  
	for(i=0;i<8;i++)  
	{   
		while(!DATA);//等待50us低电平结束   
		temp=0;    
		DHT11_delay10us();
		DHT11_delay10us();
		DHT11_delay10us();
		DHT11_delay10us();
		if(DATA==1)
			temp=1;     
		while(DATA);  
			data_byte<<=1;    
			data_byte|=temp;  
	}   
	return data_byte;
}

void DHT11_receive()//接收数据 
{  
	uchar T_H,T_L,R_H,R_L,check,num_check; 
	DHT11_START();//开始信号  
	DATA=1;  
	if(!DATA)//读取DHT11响应信号 
	{
		while(!DATA);//DHT11高电平80us是否结束
		while(DATA);  
		R_H=DHT11_receive_byte(); 
		R_L=DHT11_receive_byte();  
		T_H=DHT11_receive_byte();  
		T_L=DHT11_receive_byte();  
		check=DHT11_receive_byte();  
		DATA=0;//拉低延时50us   
		DHT11_delay10us();
		DHT11_delay10us();
		DHT11_delay10us();
		DHT11_delay10us();
		DHT11_delay10us();  
		DATA=1;   
		num_check=R_H+R_L+T_H+T_L;  
		if(num_check=check)  
		{   
			 RH=R_H;   
			 RL=R_L;   
			 TH=T_H;   
			 TL=T_L;    
			 check=num_check;  
		} 
  }
}