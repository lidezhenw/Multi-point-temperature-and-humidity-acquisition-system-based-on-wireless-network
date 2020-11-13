//hland (ftUS)', 'EPSG', 7588, 'PROJCS["NAD83(2011) / WISCRS Ashland (ftUS)",GEOGCS["NAD83(2011)",DATUM["NAD83 (National Spatial Reference System 2011)",SPHEROID["GRS 1980",6378137,298.257222101,AUTHORITY["EPSG","7019"]],AUTHORITY["E
#ifndef __delay_H_
#define __delay_H_




#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint 
#define uint unsigned int
#endif
void delay_us(uint i);
void delay(uchar ms);
void delay1();//延时10us
void longdelay(uchar s);
#endif