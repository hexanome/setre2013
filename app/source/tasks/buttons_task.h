#ifndef __BUTTONSTASK_H__
#define __BUTTONSTASK_H__

#include <includes.h>
#include <hal_buttons.h>

/*******************************************************************************
* Defines.
*******************************************************************************/

#define BUTTON_PORT_VECTOR PORT2_VECTOR

/*******************************************************************************
* Variables.
*******************************************************************************/

// Tasks.
extern OS_STK ButtonsTaskStack[BUTTONS_TASK_STACK_SIZE];

// Values.
static ButtonFlags buttons;

// Synchronization.
static void *qButtonsData[1];
static OS_EVENT *qButtons;

/*******************************************************************************
* Function prototypes.
*******************************************************************************/

// Public.
void ButtonsTask(void *args);

// Private.
static void InitializeButtons();
static void InitializeQueue();

#endif