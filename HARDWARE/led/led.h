#ifndef LED
#define LED
#include "main.h"
//LED1任务
#define LED1_TASK_PRIO 3              //任务优先级
#define LED1_STK_SIZE 50              //任务堆栈大小
extern TaskHandle_t LED1_Handler;     //任务句柄
void Led1_task(void *p_arg);          //任务函数
//LED2任务
#define LED2_TASK_PRIO 3              //任务优先级
#define LED2_STK_SIZE 50              //任务堆栈大小
extern TaskHandle_t LED2_Handler;     //任务句柄
void Led2_task(void *p_arg);          //任务函数
//LED3任务
#define LED3_TASK_PRIO 3              //任务优先级
#define LED3_STK_SIZE 50              //任务堆栈大小
extern TaskHandle_t LED3_Handler;     //任务句柄
void Led3_task(void *p_arg);          //任务函数
//LED4任务
#define LED4_TASK_PRIO 3              //任务优先级
#define LED4_STK_SIZE 50              //任务堆栈大小
extern TaskHandle_t LED4_Handler;     //任务句柄
void Led4_task(void *p_arg);          //任务函数
#endif
