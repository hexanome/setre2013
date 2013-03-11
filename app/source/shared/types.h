#ifndef __APPTYPES_H__
#define __APPTYPES_H__

typedef void* QueueData;
typedef OS_EVENT* Queue;
typedef char AppState;
typedef unsigned char ButtonFlags;

typedef struct {
   unsigned long startAddr;
   unsigned long endAddr;
} audioChunk;

#endif