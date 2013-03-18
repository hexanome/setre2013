#ifndef __RX_TASK__
#define __RX_TASK__

#include <includes.h>

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

#define QUEUE_RX_BUFFER_LENGTH 255

/*-------------------------------------------------------------
 *                  Function Prototypes
 * ------------------------------------------------------------*/
extern void halUsbInit(void);
extern void halUsbShutDown(void);
extern void halUsbSendChar(char character);


void halUsbSendString2(char string[], INT16U length);
void halUsbReceiveString(char string[], unsigned char *length);


/*-------------------------------------------------------------
 *                  Variables
 * ------------------------------------------------------------*/
// Tasks.
static OS_STK RxTaskStack[RX_TASK_STACK_SIZE];

// The reception buffer is symbolized by a message queue
static OS_EVENT* qRxBuffer = NULL;
static void* bufferRx [QUEUE_RX_BUFFER_LENGTH];

/*-------------------------------------------------------------
 *                  Function Prototypes
 * ------------------------------------------------------------*/
// Public.
void RxTask(void *args);

void InitializeQRxBuffer(void);

// Private.

#endif