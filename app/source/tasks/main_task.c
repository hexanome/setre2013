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
		
    // Initialize needed underlying layer
    //InitializeButtons();
    
    // Initialize the differents IPC (mainly message queues)
    InitializeQButtons();
    InitializeQSyncDMA();
    InitializeQSyncDMA1();
    InitializeQRxBuffer();
		
    // Start the child tasks.
    TasksCreate();
    
    // Continue running but do nothing.
    while (1)
    {
        // Delay 100s.
        OSTimeDlyHMSM(0, 0, 100, 0);
    }
}

/*******************************************************************************
* Variable definition.
*******************************************************************************/

OS_STK MainTaskStack[MAIN_TASK_STACK_SIZE];

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