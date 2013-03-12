#ifndef __APPTYPES_H__
#define __APPTYPES_H__

#define true 1
#define false 0

typedef void* QueueData;
typedef OS_EVENT* Queue;
typedef char AppState;
typedef unsigned char ButtonFlags;
typedef int bool;

typedef struct {
   unsigned long startAddr;
   unsigned long endAddr;
} audioChunk;

#endif