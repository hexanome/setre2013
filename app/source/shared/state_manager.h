#ifndef __STATEMANAGER_H__
#define __STATEMANAGER_H__

#include "includes.h"

/*******************************************************************************
* Defines.
*******************************************************************************/

#define STATE_IDLE      0
#define STATE_RECORDING 1
#define STATE_LOADING   2
#define STATE_RESULT    3

/*******************************************************************************
* Variables.
*******************************************************************************/

static AppState _state;
extern INT32U recordStartTime;
extern int scrollPosition;

/*******************************************************************************
* Function prototypes.
*******************************************************************************/

// Public.
void SetupStateManager();
AppState GetState();
void SetState(AppState state);
void SetNextState();
void AddVerticalScrollPositionOffset(int direction);

#endif