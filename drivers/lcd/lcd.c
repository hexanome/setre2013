/******************************************************************
 *****                                                        *****
 *****  Name: display.c                                       *****
 *****  modified version of Andreas Dannenberg                *****
 *****  Includes only display specific code.		      *****                                                        *****
 ******************************************************************/


#include <msp430x14x.h>
#include "lcd.h"
#include "platform.h"

#define bitset(var,bitno) ((var) |= 1 << (bitno))
#define bitclr(var,bitno) ((var) &= ~(1 << (bitno)))

#define         DISP_ON                 0x0c            //LCD control constants
#define         DISP_OFF                0x08            //
#define         CLR_DISP                0x01            //
#define         CUR_HOME                0x02            //
#define         ENTRY_INC               0x06            //
#define         DD_RAM_ADDR             0x80            //
#define         DD_RAM_ADDR2            0xc0            //
#define         DD_RAM_ADDR3            0x28            //
#define         CG_RAM_ADDR             0x40            //

// Necessaire a cause d'un bug d'insight
int variableGlobalePourBSS;
int variableGlobalePourDATA=1;



void _E(void)
{
        bitset(P2OUT,E);		//toggle E for LCD
	Delay(_10us);
	bitclr(P2OUT,E);
}

static char HexDigit(int digitvalue)
{
  if (digitvalue < 10)
    return(digitvalue + '0');
  else
    return(digitvalue + 'A' - 10);
}

static void Send_Cmd (unsigned char e)
{
        int temp;
	Delayx100us(10);                //10ms
	temp = e & 0xf0;		//get upper nibble	
	LCD_Data &= 0x0f;
	LCD_Data |= temp;               //send CMD to LCD
	bitclr(P2OUT,RS);     	        //set LCD to CMD mode
	_E();                           //toggle E for LCD
	temp = e & 0x0f;
	temp = temp << 4;               //get down nibble
	LCD_Data &= 0x0f;
	LCD_Data |= temp;
	bitclr(P2OUT,RS);   	        //set LCD to CMD mode
	_E();                           //toggle E for LCD
}

static void Send_Char (unsigned char d)
{
        int temp;
	Delayx100us(5);                 //.5ms	
	temp = d & 0xf0;		//get upper nibble	
	LCD_Data &= 0x0f;
	LCD_Data |= temp;
	bitset(P2OUT,RS);     	        //set LCD to data mode
	_E();                           //toggle E for LCD
	temp = d & 0x0f;
	temp = temp << 4;               //get down nibble
	LCD_Data &= 0x0f;
	LCD_Data |= temp;
	bitset(P2OUT,RS);   	        //set LCD to data mode
	_E();                           //toggle E for LCD
}


void LCD_Clear(void)
{
    Send_Cmd(CLR_DISP);
    Delayx100us(10);
}

void LCD_First_Line(void)
{
//    Send_Cmd(CLR_DISP);
    Send_Cmd(DD_RAM_ADDR);
}

void LCD_Second_Line(void)
{
//    Send_Cmd(CLR_DISP);
    Send_Cmd(DD_RAM_ADDR2);
}

void LCD_Print_String(char *string)
{
	while(*string)
		Send_Char(*string++);
}

void LCD_Print_Byte(unsigned int theByte)
{
	char HexBuffer[3];
	HexBuffer[2] = 0;
	HexBuffer[1] = HexDigit(theByte & 0x000f);
	theByte = theByte >> 4;
	HexBuffer[0] = HexDigit(theByte & 0x000f);
	LCD_Print_String(HexBuffer);
}

void LCD_Print_Hex(unsigned int number)
{
	char hexBuffer[5];
	hexBuffer[4] = 0;
	hexBuffer[3] = HexDigit(number & 0x000f);
	number = number >> 4;
	hexBuffer[2] = HexDigit(number & 0x000f);
	number = number >> 4;
	hexBuffer[1] = HexDigit(number & 0x000f);
	number = number >> 4;
	hexBuffer[0] = HexDigit(number & 0x000f);
	LCD_Print_String(hexBuffer);
}

void LCD_Print_Decimal(int number)
{
  // need to move to long int to account for
  // negative 32768
  char DecimalBuffer[7];
  long lNumber = number;
  DecimalBuffer[6] = 0;
  if (lNumber < 0) {
    DecimalBuffer[0] = '-';
    lNumber = -lNumber;
  } else
    DecimalBuffer[0] = '+';
  DecimalBuffer[5] = (lNumber % 10)+'0';
  lNumber = lNumber / 10;
  DecimalBuffer[4] = (lNumber % 10)+'0';
  lNumber = lNumber / 10;
  DecimalBuffer[3] = (lNumber % 10)+'0';
  lNumber = lNumber / 10;
  DecimalBuffer[2] = (lNumber % 10)+'0';
  lNumber = lNumber / 10;
  DecimalBuffer[1] = (lNumber % 10)+'0';
  LCD_Print_String(DecimalBuffer);
}

void LCD_Init(void)
{
    bitclr(P2OUT,RS);
    Delayx100us(250);                   //Delay 100ms
    Delayx100us(250);
    Delayx100us(250);
    Delayx100us(250);
    LCD_Data |= BIT4 | BIT5;            //D7-D4 = 0011
    LCD_Data &= ~BIT6 & ~BIT7;
    _E();                               //toggle E for LCD
    Delayx100us(100);                   //10ms
    _E();                               //toggle E for LCD
    Delayx100us(100);                   //10ms
    _E();                               //toggle E for LCD
    Delayx100us(100);                   //10ms
    LCD_Data &= ~BIT4;
    _E();                               //toggle E for LCD

    Send_Cmd(DISP_ON);
    Send_Cmd(CLR_DISP);
    Delayx100us(250);
    Delayx100us(250);
    Delayx100us(250);
    Delayx100us(250);
}



