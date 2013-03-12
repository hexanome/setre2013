#include "buttons_task.h"

/*******************************************************************************
* The Buttons Task.
* Responsible for:
*   Recording input and setting variables accordingly.
*******************************************************************************/

#define DATA_SIZE 1

static void* data [DATA_SIZE];


void ButtonsTask(void *args)
{
    // Define which buttons we're interested in.
    buttons = BUTTON_S1 | BUTTON_S2;
    
    // Setup the buttons interruptions.
    InitializeButtons();
    
    // Create the synchronization object.
    InitializeQueue();
    
    for (;;)
    {
        // Wait on the buttons queue.
        INT8U err;
        ButtonFlags pressedButtons = *(ButtonFlags *)OSQPend(qButtons, 0, &err);
        if ((pressedButtons & buttons) == 0) continue;
        
        // React to the pressed buttons.
        SetNextState();
    }
}

/*******************************************************************************
* Variable definition.
*******************************************************************************/

OS_STK ButtonsTaskStack[BUTTONS_TASK_STACK_SIZE];

/*******************************************************************************
* Method implementations.
*******************************************************************************/

void InitializeButtons()
{    
    // Initialize the buttons.
    halButtonsInit(buttons);
    
    // Enable interruptions for those buttons.
    halButtonsInterruptEnable(buttons);
}

void InitializeQueue()
{
    qButtons = OSQCreate(data,DATA_SIZE); 
}

/*******************************************************************************
* Interrupt routines.
*******************************************************************************/

#pragma vector=BUTTON_PORT_VECTOR
__interrupt void BUTTON_PORT_ISR(void)
{    
    ButtonFlags data[1] = { BUTTON_PORT_IFG };
    OSQPost(qButtons, &data[0]);
    
    BUTTON_PORT_IFG = 0;
}
