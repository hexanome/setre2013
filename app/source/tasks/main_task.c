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
		InitializeButtons();
		
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
	OSTaskCreateExt(ButtonsTask,
									(void *)0,
									(OS_STK *)&ButtonsTaskStack[BUTTONS_TASK_STACK_SIZE - 1],
									BUTTONS_TASK_PRIORITY,
									BUTTONS_TASK_PRIORITY,
									(OS_STK *)&ButtonsTaskStack[0],
									BUTTONS_TASK_STACK_SIZE,
									(void *)0,
									OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);
	//TaskStart(ButtonsTask, BUTTONS_TASK_PRIORITY, ButtonsTaskStack, BUTTONS_TASK_STACK_SIZE);
  
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
	//TaskStart(RecordTask, RECORD_TASK_PRIORITY, RecordTaskStack, RECORD_TASK_STACK_SIZE);

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
	//TaskStart(LcdTask, LCD_TASK_PRIORITY, LcdTaskStack, LCD_TASK_STACK_SIZE);
}