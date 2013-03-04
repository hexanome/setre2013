#ifndef  __APP_CONFIG_H__
#define  __APP_CONFIG_H__

// Task priorities.

#define OS_TASK_TMR_PRIO                (OS_LOWEST_PRIO - 4)
#define MAIN_TASK_PRIORITY              5
#define RECORD_TASK_PRIORITY            10
#define LCD_TASK_PRIORITY               50

// Task stack sizes.

#define MAIN_TASK_STACK_SIZE            256
#define RECORD_TASK_STACK_SIZE          256
#define LCD_TASK_STACK_SIZE             256

#endif