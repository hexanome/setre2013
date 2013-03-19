#include "state_manager.h"

INT32U recordStartTime = 0;
int scrollPosition = 0;

/*******************************************************************************
* Method implementation.
*******************************************************************************/

void SetupStateManager()
{
    // Set the default state.
    _state = STATE_IDLE;
    
    // Setup LEDs.
    LED_PORT_DIR |= LED_1 | LED_2;
}

AppState GetState()
{
    return _state;
}

void SetState(AppState state)
{
    // If the state is no different, nothing do to here.
    if (state == _state) return;
    
    // Do something depending on the new state value.
    switch (state)
    {
        case STATE_IDLE:
            // Enable the LED.
            LED_PORT_OUT &= ~LED_2;
            break;
        case STATE_RECORDING:    
            // Set the time when the recording begun.
            recordStartTime = OSTimeGet();      
            
            // Start the recording.
            Trigger(qToggleRecord);   
            
            // Enable the LED.
            LED_PORT_OUT |= LED_1;
            break;
        case STATE_LOADING:          
            // End the recording.
            Trigger(qToggleRecord);
            
            // Disable the LED.
            LED_PORT_OUT &= ~LED_1;
            break;
        case STATE_RESULT:
            // Enable the LED.
            LED_PORT_OUT |= LED_2;
            break;
    }
    
    // Update the state value.
    _state = state;
} 

void SetNextState()
{    
    AppState newState;
    
    if (_state == STATE_RESULT)
        newState = STATE_IDLE;
    else
        newState = _state + 1;
    
    SetState(newState);
}

void AddVerticalScrollPositionOffset(int direction)
{
    scrollPosition -= (direction * 5);
}