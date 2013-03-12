#include <includes.h>

#include "shared/task_helpers.h"
#include "tasks/main_task.h"

/*******************************************************************************
* Defines.
*******************************************************************************/

/*******************************************************************************
* Variables.
*******************************************************************************/
unsigned char recordEn = 0;

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
    
    // Initialize uC/OS-II.
    OSInit();
    
    // Create the main task.
    TaskStart(MainTask, MAIN_TASK_PRIORITY, MainTaskStack, MAIN_TASK_STACK_SIZE);
    
    // Start uC/OS-II.
    OSStart();                    
}