#ifndef  __APP_CONFIG_H__
#define  __APP_CONFIG_H__

// Task priorities.

#define OS_TASK_TMR_PRIO                (OS_LOWEST_PRIO - 4)
#define MAIN_TASK_PRIORITY              5
#define RECORD_TASK_PRIORITY            10
#define BUTTONS_TASK_PRIORITY           30
#define LCD_TASK_PRIORITY               50

#define RX_TASK_PRIORITY                35
#define TX_TASK_PRIORITY                40

// Task stack sizes.

#define MAIN_TASK_STACK_SIZE            256
#define RECORD_TASK_STACK_SIZE          256
#define BUTTONS_TASK_STACK_SIZE         256
#define LCD_TASK_STACK_SIZE             768

#define RX_TASK_STACK_SIZE              256
#define TX_TASK_STACK_SIZE              256

// Buffer sizes

#define TX_BUFFER_SIZE                  256

#endif