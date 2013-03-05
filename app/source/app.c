#include <includes.h>
#include "Tasks/main_task.h"

/*******************************************************************************
* Defines.
*******************************************************************************/

/*******************************************************************************
* Variables.
*******************************************************************************/

/*******************************************************************************
* Function prototypes.
*******************************************************************************/

/*******************************************************************************
* THE MAIN!
*******************************************************************************/

void main (void)
{
    // Disable interrupts until we are ready to accept them.
    BSP_IntDisAll();
    
    // Set up interrupt vector to Context Switch (TRAP #0).
    //*((int *)0x80) = (int)OSCtxSw;
    
    // Initialize uC/OS-II.
    OSInit();
    
    // Create the main task.
    OSTaskCreateExt(MainTask, 
                    (void *)0,
                    (OS_STK *)&MainTaskStack[MAIN_TASK_STACK_SIZE - 1],
                    MAIN_TASK_PRIORITY,
                    MAIN_TASK_PRIORITY,
                    (OS_STK *)&MainTaskStack[0],
                    MAIN_TASK_STACK_SIZE,
                    (void *)0,
                    OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);
    
    // Start uC/OS-II.
    OSStart();                    
}