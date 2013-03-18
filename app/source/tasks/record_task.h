#ifndef __RECORDTASK_H__
#define __RECORDTASK_H__

#include <includes.h>

/*******************************************************************************
* Defines.
*******************************************************************************/
/*-------------------------------------------------------------
 *                  USB global definitions
 * ------------------------------------------------------------*/
#include "hal_MSP-EXP430F5438.h"

#define USB_PORT_OUT      P5OUT
#define USB_PORT_SEL      P5SEL
#define USB_PORT_DIR      P5DIR
#define USB_PORT_REN      P5REN
#define USB_PIN_TXD       BIT6
#define USB_PIN_RXD       BIT7

#define QUEUE_SYNCDMA_LENGTH    1
#define SIZE_OF_AUDIO_BUFFER    512
#define NUMBER_OF_BUFFER 				(sizeof(audioBuffers)/sizeof(*audioBuffers))
#define TIMERB_COMP_VALUE 			3051

/*******************************************************************************
* Variables.
*******************************************************************************/

// Tasks.
extern OS_STK RecordTaskStack[RECORD_TASK_STACK_SIZE];

// Queues.
static void* qSyncDMAData[QUEUE_SYNCDMA_LENGTH];
static OS_EVENT* qSyncDMA;

/*******************************************************************************
* Function prototypes.
*******************************************************************************/

// Public.
void RecordTask(void *args);
void InitializeQSyncDMA(void);
extern void halUsbInit(void);
extern void halUsbShutDown(void);

// Private.
static void setupRecord(void);
static void stopRecord(void);
static void halUsbSendChar(char character);
static void halUsbSendStringINT16U(unsigned char string[], INT16U length);

#endif