#ifndef __BUTTONSTASK_H__
#define __BUTTONSTASK_H__

#include <includes.h>
#include <hal_buttons.h>

/*******************************************************************************
* Defines.
*******************************************************************************/

#define BUTTON_PORT_VECTOR      PORT2_VECTOR
#define QUEUE_BUTTONS_LENGTH    1
#define BUTTON_DCLICK_DELAY     0.05

/*******************************************************************************
* Variables.
*******************************************************************************/

// Tasks.
extern OS_STK ButtonsTaskStack[BUTTONS_TASK_STACK_SIZE];

// Values.
static unsigned char buttons;
static INT32U clickTime = 0;
static INT32U lastClickTime = 0;

// Synchronization.
static void* qButtonsData[QUEUE_BUTTONS_LENGTH];
static OS_EVENT* qButtons;

/*******************************************************************************
* Function prototypes.
*******************************************************************************/

// Public.
void ButtonsTask(void *args);
void InitializeButtons();
void InitializeQButtons();

// Private.

#endif