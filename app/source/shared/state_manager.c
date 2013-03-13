#include "state_manager.h"

INT32U recordStartTime = 0;

/*******************************************************************************
* Method implementation.
*******************************************************************************/

void SetupStateManager()
{
    // Set the default state.
    _state = STATE_IDLE;
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
            // Set the time when the recording begun.
            recordStartTime = OSTimeGet();            
            Trigger(qToggleRecord);            
            break;
        case STATE_RECORDING:
            Trigger(qToggleRecord);
            break;
        case STATE_LOADING:
            break;
    }
    
    // Update the state value.
    _state = state;
} 

void SetNextState()
{
    AppState newState;
    
    if (_state == STATE_LOADING)
        newState = STATE_IDLE;
    else
        newState = _state + 1;
    
    SetState(newState);
}