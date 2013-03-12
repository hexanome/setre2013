#include "lcd_task.h"
#include "hal_lcd.h"


/*******************************************************************************
* The LCD Task.
* Responsible for:
*   Display information about the current recording state.
*   Display the voice recognition result sent by the computer.      
*******************************************************************************/

void LcdTask(void *args)
{
	// TODO: Do something.
	while (1)
	{
		halLcdPrintLine("Loading", 0, OVERWRITE_TEXT);
  	// Delay 100ms.
		OSTimeDlyHMSM(0, 0, 0, 100);
	}
}

/*******************************************************************************
* Variable definition.
*******************************************************************************/

OS_STK LcdTaskStack[LCD_TASK_STACK_SIZE];