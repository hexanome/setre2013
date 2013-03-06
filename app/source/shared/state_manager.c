#include "state_manager.h"

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
    
    // Update the state value.
    _state = state;
} 