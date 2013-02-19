#ifndef _DISPLAY_H_
#define _DISPLAY_H_

void LCD_Clear(void);

void LCD_First_Line(void);

void LCD_Second_Line(void);

void LCD_Print_String(char *string);

void LCD_Print_Decimal(int number);

void LCD_Print_Hex(unsigned int number);

void LCD_Print_Byte(unsigned int theByte);

void LCD_Init(void);
#endif
