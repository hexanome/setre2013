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
    
    // Setup the shared components.
    SetupStateManager();
    SetupSynchronization();
    
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
    // Start the buttons task.
    TaskStart(ButtonsTask, BUTTONS_TASK_PRIORITY, ButtonsTaskStack, BUTTONS_TASK_STACK_SIZE);
    
    // Start the Microphone Recording task.
    TaskStart(RecordTask, RECORD_TASK_PRIORITY, RecordTaskStack, RECORD_TASK_STACK_SIZE);
    
    // Start the LCD task.
    TaskStart(LcdTask, LCD_TASK_PRIORITY, LcdTaskStack, LCD_TASK_STACK_SIZE);
}

void TaskStart(void (*task)(void *), int priority, TaskStack *stack, int stackSize)
{
    OSTaskCreateExt(task, 
                    (void *)0,
                    (OS_STK *)&stack[stackSize - 1],
                    priority,
                    priority,
                    (OS_STK *)&stack[0],
                    stackSize,
                    (void *)0,
                    OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);
}