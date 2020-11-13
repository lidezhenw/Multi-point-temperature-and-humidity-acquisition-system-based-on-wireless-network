#ifndef __LCD1602_H_
#define __LCD1602_H_

#include<reg52.h>
#include<intrins.h>

 //bit LCD_Check_Busy(void) ;
 void LCD_Write_Com(unsigned char com) ;
 
 void LCD_Write_Data(unsigned char Data) ;

 void LCD_Clear(void) ;

 void LCD_Write_String(unsigned char x,unsigned char y,unsigned char *s) ;

 void LCD_Write_Char(unsigned char x,unsigned char y,unsigned char Data) ;

 void LCD_Init(void) ;

 void Lcd_User_Chr(void);



#endif