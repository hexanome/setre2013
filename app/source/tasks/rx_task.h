#ifndef __RX_TASK__
#define __RX_TASK__

#include <includes.h>

#define QUEUE_RX_BUFFER_LENGTH 255
#define TEXT_SIZE 512
/*-------------------------------------------------------------
 *                  Variables
 * ------------------------------------------------------------*/
// Tasks.
static OS_STK RxTaskStack[RX_TASK_STACK_SIZE];

// The reception buffer is symbolized by a message queue
static OS_EVENT* qRxBuffer = NULL;
static void* bufferRx [QUEUE_RX_BUFFER_LENGTH];

extern char textToRead [TEXT_SIZE];

/*-------------------------------------------------------------
 *                  Function Prototypes
 * ------------------------------------------------------------*/
// Public.
void RxTask(void *args);
void InitializeQRxBuffer(void);

#endif