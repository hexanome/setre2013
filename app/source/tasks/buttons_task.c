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
    unsigned char stateButtons = BUTTON_S1 | BUTTON_S2;
    unsigned char joystickButtons = BUTTON_DOWN | BUTTON_UP;
    
    buttons = stateButtons | joystickButtons;

    // Setup the buttons interruptions.
    InitializeButtons();
    
    for (;;)
    {        
        // Wait on the buttons queue.
        INT8U err;
        pressedButtons = (unsigned char)OSQPend(qButtons, 100, &err);
        
        // If there was an interrupt, regular process.
        if (err == OS_ERR_NONE)
        {
            // Prevent hardware error.
            clickTime = OSTimeGet();
            if (((float)(clickTime - lastClickTime) / OS_TICKS_PER_SEC) < BUTTON_DCLICK_DELAY)
            {
                continue;
            }
            
            lastClickTime = clickTime;
        
            // React to the pressed buttons.
            if ((pressedButtons & stateButtons) != 0 && GetState() != STATE_LOADING)
            {
                SetNextState();
            }
        }
        // Otherwise, and only for the joystick, poll the buttons.
        else
        {
            pressedButtons = halButtonsPressed();
        }

        // React to the joystick.
        if ((pressedButtons & BUTTON_DOWN) != 0)
        {
            AddVerticalScrollPositionOffset(-1);
        }
        
        if ((pressedButtons & BUTTON_UP) != 0)
        {
            AddVerticalScrollPositionOffset(1);
        }
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
