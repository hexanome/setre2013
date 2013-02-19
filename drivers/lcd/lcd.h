#ifndef _DISPLAY_H_
#define _DISPLAY_H_

/* 
Clear the LCD
*/
void LCD_Clear(void);

/* 
Move the LCD cursor to the begin of first line
*/
void LCD_First_Line(void);

/* 
Move the LCD cursor to the begin of second line
*/
void LCD_Second_Line(void);

/* 
Print the string "string" starting from the segment pointed by the LCD cursor.
"string" must be ended with a '\0' char. All chars after the 16th
will be discarded.
*/
void LCD_Print_String(char *string);

/* 
Print the decimal "number" with the following format: +00000 (sign and 5 digits)
*/
void LCD_Print_Decimal(int number);

/* 
Print the number "number" in hex format on 4 digits.
*/
void LCD_Print_Hex(unsigned int number);


/* 
Print the byte "number" in hex format on 2 digits.
*/
void LCD_Print_Hex(unsigned int number);
void LCD_Print_Byte(unsigned int theByte);

/* 
Init the LCD. Must be called before the other functions.
*/
void LCD_Init(void);

#endif
