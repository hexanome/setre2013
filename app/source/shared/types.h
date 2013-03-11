#ifndef __APPTYPES_H__
#define __APPTYPES_H__

typedef OS_STK TaskStack;
typedef OS_EVENT Queue;
typedef char AppState;
typedef unsigned char ButtonFlags;

typedef struct {
   unsigned long startAddr;
   unsigned long endAddr;
} audioChunk;

#endif