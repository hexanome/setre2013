/*
********************************************************************************************************
;                                               uC/OS-II
;                                         The Real-Time Kernel
;
;                              (c) Copyright 2002, Micrium, Inc., Weston, FL
;                                          All Rights Reserved
;
;                                               TI MSP430
;
;
; File         : OS_CPU_A.S43
; By           : Jian Chen (yenger@hotmail.com)
;                Jean J. Labrosse
;
; Modified By  : Ryan A. Sherry for mspgcc
;********************************************************************************************************
*/

#include <io.h>
#include <signal.h>
#include <msp430/timera.h>

/*
;********************************************************************************************************
;                                           MACRO DEFINITIONS
;********************************************************************************************************
*/

	.macro	PUSHALL
            push     r4
            push     r5
            push     r6
            push     r7
            push     r8
            push     r9
            push     r10
            push     r11
            push     r12
            push     r13
            push     r14
            push     r15         
	.endm

	.macro	POPALL
            pop      r15
            pop      r14
            pop      r13
            pop      r12
            pop      r11
            pop      r10
            pop      r9
            pop      r8
            pop      r7
            pop      r6
            pop      r5
            pop      r4          
	.endm

/*            
;********************************************************************************************************
;                                  PUBLIC AND EXTERNAL DECLARATIONS
;********************************************************************************************************
*/

	.extern  OSIntExit
	.extern  OSIntNesting

	.extern  OSISRStkPtr

	.extern  OSPrioCur
	.extern  OSPrioHighRdy

	.extern  OSRunning

	.extern  OSTCBCur
	.extern  OSTCBHighRdy

	.extern  OSTaskSwHook
	.extern  OSTimeTick

	.global  OSCtxSw
	.global  OSCPURestoreSR
	.global  OSCPUSaveSR
	.global  OSIntCtxSw
	.global  OSStartHighRdy

;********************************************************************************************************
;                                  START HIGHEST PRIORITY READY TASK
;
; Description: This function is called by OSStart() to start the highest priority task that is ready to run.
;
; Note       : OSStartHighRdy() MUST:
;                 a) Call OSTaskSwHook() then,
;                 b) Set OSRunning to TRUE,
;                 c) Switch to the highest priority task.
;********************************************************************************************************

	.text

	.global OSStartHighRdy
	.type OSStartHighRdy, @function

OSStartHighRdy:	
            call     #OSTaskSwHook

            mov.b    #1, &OSRunning         /* kernel running */

            mov.w    r1, &OSISRStkPtr       /* save interrupt stack  */            

            mov.w    &OSTCBHighRdy, r13     /* load highest ready task stack  */
            mov.w    @r13, r1

            POPALL							/* Restore all the registers.  */
                    
            reti                            /* emulate return from interrupt  */
 

/*
;********************************************************************************************************
;                                     TASK LEVEL CONTEXT SWITCH
;
; Description: This function is called by OS_Sched() to perform a task level context switch.
;
; Note       : OSCtxSw() MUST:
;                 a) Save the current task's registers onto the current task stack
;                 b) Save the SP into the current task's OS_TCB
;                 c) Call OSTaskSwHook()
;                 d) Copy OSPrioHighRdy to OSPrioCur
;                 e) Copy OSTCBHighRdy to OSTCBCur
;                 f) Load the SP with OSTCBHighRdy->OSTCBStkPtr
;                 g) Restore all the registers from the high priority task stack
;                 h) Perform a return from interrupt
;********************************************************************************************************
*/

.global OSCtxSw
OSCtxSw:
            push      r2                    /* emulate interrupt by also saving the SR */
            
            PUSHALL							/* Save all the task registers.  */
            
            mov.w     &OSTCBCur, r13        /* OSTCBCur->OSTCBStkPtr = SP  */
            mov.w     r1, 0(r13)

            call      #OSTaskSwHook

            mov.b     &OSPrioHighRdy, r13   /*  OSPrioCur = OSPrioHighRdy  */
            mov.b     r13, &OSPrioCur       /*                             */
                          
            mov.w     &OSTCBHighRdy, r13    /* OSTCBCur  = OSTCBHighRdy    */
            mov.w     r13, &OSTCBCur        /*                             */
                          
            mov.w     @r13, r1              /*  SP        = OSTCBHighRdy->OSTCBStkPtr  */
             
            POPALL							/* Restore the new task registers.  */
            
            reti                            /* return from interrup  */


/*
;********************************************************************************************************
;                                       ISR LEVEL CONTEXT SWITCH
;
; Description: This function is called by OSIntExit() to perform an ISR level context switch.
;
; Note       : OSIntCtxSw() MUST:
;                 a) Call OSTaskSwHook()
;                 b) Copy OSPrioHighRdy to OSPrioCur
;                 c) Copy OSTCBHighRdy to OSTCBCur
;                 d) Load the SP with OSTCBHighRdy->OSTCBStkPtr
;                 e) Restore all the registers from the high priority task stack
;                 f) Perform a return from interrupt
;********************************************************************************************************
*/

.global OSIntCtxSw

OSIntCtxSw:
             
            call      #OSTaskSwHook

            mov.b     &OSPrioHighRdy, r13   /* OSPrioCur = OSPrioHighRdy */
            mov.b     r13, &OSPrioCur       
                          
            mov.w     &OSTCBHighRdy, r13    /* OSTCBCur  = OSTCBHighRdy  */
            mov.w     r13, &OSTCBCur
                          
            mov.w     @r13, r1              /* SP        = OSTCBHighRdy->OSTCBStkPtr */
             
            POPALL							/* Restore the registers.  */
             
            reti                            /* return from interrup */

/*
;********************************************************************************************************
;                                              TICK ISR
;
; Description: This ISR handles tick interrupts.  This ISR uses TIMERA CCR0 timer as the tick source.
;
; Notes      : 1) The following C pseudo-code describes the operations being performed in the code below.
;
;                 Save all the CPU registers
;                 Reload the TIMERA CCR0 counter register
;                 if (OSIntNesting == 0) {
;                     OSTCBCur->OSTCBStkPtr = SP;
;                     SP                    = OSISRStkPtr;  ;Use the ISR stack from now on           
;                 }
;                 OSIntNesting++;
;                 Enable interrupt nesting;                 ;Allow nesting of interrupts (if needed) 
;                 Clear the interrupt source;
;                 OSTimeTick();                             ;Call uC/OS-II's tick handler            
;                 DISABLE general interrupts;               ;Must DI before calling OSIntExit()      
;                 OSIntExit();
;                 if (OSIntNesting == 0) {
;                     SP = OSTCBHighRdy->OSTCBStkPtr;       ;Restore the current task's stack        
;                 }
;                 Restore the CPU registers
;                 Return from interrupt.
;
;              2) ALL ISRs should be written like this!
;
;              3) You MUST disable general interrupts BEFORE you call OSIntExit() because an interrupt 
;                 COULD occur just as OSIntExit() returns and thus, the new ISR would save the SP of 
;                 the ISR stack and NOT the SP of the task stack.  This of course will most likely cause
;                 the code to crash.  By disabling interrupts BEFORE OSIntExit(), interrupts would be
;                 disabled when OSIntExit() would return.  This assumes that you are using OS_CRITICAL_METHOD
;                 #3 (which is the prefered method).
;
;              4) If you DON'T use a separate ISR stack then you don't need to disable general interrupts 
;                 just before calling OSIntExit().  The pseudo-code for an ISR would thus look like this:
;
;                 Save all the CPU registers
;                 Reload the TIMERA CCR0 counter register
;                 if (OSIntNesting == 0) {
;                     OSTCBCur->OSTCBStkPtr = SP;
;                 }
;                 OSIntNesting++;
;                 Enable interrupt nesting;                 ; Allow nesting of interrupts (if needed) 
;                 Clear the interrupt source;
;                 OSTimeTick();                             ; Call uC/OS-II's tick handler            
;                 OSIntExit();
;                 Restore the CPU registers
;                 Return from interrupt.
;********************************************************************************************************
*/

interrupt(TIMERA0_VECTOR)
TA0_ISR:                                    /* TIMERA0 timer ISR */
            PUSHALL
                        
            add.w	 #2304, &TACCR0			/* Reset the Timer Period*/
                         
            cmp.b    #0, &OSIntNesting      /* if (OSIntNesting == 0)  */
            jne      L1
                               
            mov.w    &OSTCBCur, r13         /* save task stack */
            mov.w    r1, 0(r13)

            mov.w    &OSISRStkPtr, r1       /* load interrupt stack  */

L1:
            inc.b    &OSIntNesting          /* increase OSIntNesting */
             
            EINT                           /* enable general interrupt to allow for interrupt nesting */

            call     #OSTimeTick            /* call ticks routine       */

            DINT                           /* IMPORTANT: disable general interrupt BEFORE calling OSIntExit() */

            call     #OSIntExit             /* call ticks routine */

            cmp.b    #0, &OSIntNesting      /* if (OSIntNesting == 0) */ 
            jne      L2

            mov.w    &OSTCBHighRdy, r13     /* restore task stack SP */
            mov.w    @r13, r1
                       
L2:
			POPALL

            reti                            ; return from interrupt
                                           
/*
;********************************************************************************************************
;                             SAVE AND RESTORE THE CPU'S STATUS REGISTER
;
; Description: These functions are used to implement OS_CRITICAL_METHOD #3 by saving the status register
;              in a local variable of the calling function and then, disables interrupts. 
;
; Notes      : R15 is assumed to hold the argument passed to OSCPUSaveSR() and also, the value returned
;              by OSCPURestoreSR().
;********************************************************************************************************
*/

.global OSCPUSaveSR
.type OSCPUSaveSR, @function
OSCPUSaveSR:
            MOV.W    r2,r15			/* gcc returns the value of the sr in r15 */
            DINT
            RET

.global OSCPURestoreSR
.type OSCPURestoreSR, @function
OSCPURestoreSR:
            MOV.W    r15,r2			/* gcc passes the sr in r15 */
            RET

