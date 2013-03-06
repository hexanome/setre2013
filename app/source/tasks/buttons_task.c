#include "buttons_task.h"

/*******************************************************************************
* The Buttons Task.
* Responsible for:
*   Recording input and setting variables accordingly.
*******************************************************************************/

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
        
    }
}

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
    qButtons = OSQCreate(&qButtonsData[0], 1); 
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
    
//    char error[1];
//    if (err == OS_ERR_NONE)
//    {
//        error[0] = 'A';
//    }
//    else if (err == OS_ERR_Q_FULL)
//    {
//        error[0] = 'B';
//    }
//    else if (err == OS_ERR_EVENT_TYPE)
//    {
//        error[0] = 'C';
//    }
//    else if (err == OS_ERR_PEVENT_NULL)
//    {
//        error[0] = 'D';
//    }
//    else
//    {
//        error[0] = 'E';
//    }
}
