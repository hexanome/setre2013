#ifndef __RECORDTASK_H__
#define __RECORDTASK_H__

#include <includes.h>

/*******************************************************************************
* Defines.
*******************************************************************************/

#define Memstart          0x10000
#define Memend            0x45BFF
#define MemstartTest      0x40000 
#define REC_TERM          1
#define REC_START         0
#define REC_SEND          2

/*******************************************************************************
* Variables.
*******************************************************************************/

// Tasks.
static OS_STK RecordTaskStack[RECORD_TASK_STACK_SIZE];

static unsigned long AUDIO_MEM_START[] = {0x10000, 0x20000, 0x30000, 0x40000, 0x46000};
static unsigned long lastAudioByte = 0;
static unsigned long PlaybackPtr;
extern unsigned char recordEn;

/*******************************************************************************
* Function prototypes.
*******************************************************************************/

// Public.
void RecordTask(void *args);

// Private.
static void setupRecord(void);
static void stopRecord(void);
static void record(void);

#endif