#include "buttons_task.h"

/*******************************************************************************
* The Buttons Task.
* Responsible for:
*   Recording input and setting variables accordingly.
*******************************************************************************/

void ButtonsTask(void *args)
{
    // Setup the buttons interruptions.
    InitializeButtons();
    
    while (1) {
        // Delay 100ms.
        OSTimeDlyHMSM(0, 0, 0, 100);
    }
}

/*******************************************************************************
* Method implementations.
*******************************************************************************/

void InitializeButtons()
{
    // Choose which buttons we're interrested in.
    unsigned char buttonsMask = BUTTON_S1 | BUTTON_S2;
    
    // Initialize the buttons.
    halButtonsInit(buttonsMask);
    
    // Enable interruptions for those buttons.
    halButtonsInterruptEnable(buttonsMask);
}

/*******************************************************************************
* Interrupt routines.
*******************************************************************************/

#pragma vector=BUTTON_PORT_VECTOR
__interrupt void BUTTON_PORT_ISR(void)
{
    
}
