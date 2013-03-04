#include "main_task.h"

/*******************************************************************************
* The Main Task.
* Responsible for:
*   Hardware initialization.
*   Starting other tasks.
*******************************************************************************/

void MainTask(void *args)
{    
    // Initialize BSP functions.
    BSP_Init();
    
    // Start the child tasks.
    TasksCreate();
    
    // Continue running but do nothing.
    while (1) {
        // Delay 100s.
        OSTimeDlyHMSM(0, 0, 100, 0);
    }
}

/*******************************************************************************
* Utility methods for the Main Task.
*******************************************************************************/

// Start the different child tasks.
void TasksCreate()
{
    // Start the Microphone Recording task.
    OSTaskCreateExt(RecordTask, 
                    (void *)0,
                    (OS_STK *)&RecordTaskStack[RECORD_TASK_STACK_SIZE - 1],
                    RECORD_TASK_PRIORITY,
                    RECORD_TASK_PRIORITY,
                    (OS_STK *)&RecordTaskStack[0],
                    RECORD_TASK_STACK_SIZE,
                    (void *)0,
                    OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);
    
    // Start the LCD task.
    OSTaskCreateExt(LcdTask, 
                    (void *)0,
                    (OS_STK *)&LcdTaskStack[LCD_TASK_STACK_SIZE - 1],
                    LCD_TASK_PRIORITY,
                    LCD_TASK_PRIORITY,
                    (OS_STK *)&LcdTaskStack[0],
                    LCD_TASK_STACK_SIZE,
                    (void *)0,
                    OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);
}