#include <msp430x14x.h>
#include "lcd.h"
#include "platform.h"

int main( void )
{
  unsigned int i;

  PF_Init_Ports();
  LCD_Init(); 
  LCD_Clear();
  LCD_Print_String("Test Tang");
  Delay(10000);
  while(1) ;
  return 0;
}

