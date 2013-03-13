#include "buttons_task.h"

/*******************************************************************************
* The Buttons Task.
* Responsible for:
*   Recording input and setting variables accordingly.
*******************************************************************************/

void ButtonsTask(void *args)
{
    unsigned char pressedButtons;
    
    // Define which buttons we're interested in.
    buttons = BUTTON_S1 | BUTTON_S2;

    // Setup the buttons interruptions.
    InitializeButtons();

    // Create the synchronization object.
    //InitializeQueue();
    
    for (;;)
    {
        // Wait on the buttons queue.
        INT8U err;
        pressedButtons = (unsigned char) OSQPend(qButtons, 0, &err);
        
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

void InitializeQButtons()
{
    qButtons = OSQCreate(qButtonsData, QUEUE_BUTTONS_LENGTH); 
}

/*******************************************************************************
* Interrupt routines.
*******************************************************************************/

#pragma vector=BUTTON_PORT_VECTOR
__interrupt void BUTTON_PORT_ISR(void)
{    
    unsigned char pressedButtons = BUTTON_PORT_IFG;
    
    // Acknowledge IT
    BUTTON_PORT_IFG = 0;
    
    // Signals the buttons task
    OSQPost(qButtons, (void *)pressedButtons);
}
