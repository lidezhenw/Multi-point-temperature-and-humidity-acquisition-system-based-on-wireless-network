#ifndef __DHT11_H_
#define __DHT11_H_

#include<reg52.h>
#include <intrins.h>

#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint 
#define uint unsigned int
#endif

sbit DATA = P3^6;

void DHT11_START(void);
uchar DHT11_receive_byte(void);
void DHT11_receive(void); 

#endif