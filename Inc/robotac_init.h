#ifndef ROBOTOC_INIT
#define ROBOTOC_INIT
#include "main.h"
#include "tim.h"
#include "gpio.h"
#include "usart.h"
#include "Sanwai.h"
#include "dma.h"
#include "led.h"
#include "pid.h"
#include "bsp_can.h"
#include "pwm.h"
void Robotac_Init(void);
void SystemClock_Config(void);
#define ROBOTAC_TASK_PRIO 1            //任务优先级
#define ROBOTAC_STK_SIZE 128           //任务堆栈大小
extern TaskHandle_t StartTask_Handler;        //任务句柄
void Start_task(void *pvParameters);   //任务函数

//山外上位机发送任务
#define SANWAI_TASK_PRIO 0              //任务优先级
#define SANWAI_STK_SIZE 30             //任务堆栈大小
extern TaskHandle_t SanwaiTask_Handler;        //任务句柄
void Sanwai_task(void *p_arg);          //任务函数
#endif

