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
#define ROBOTAC_TASK_PRIO 1            //�������ȼ�
#define ROBOTAC_STK_SIZE 128           //�����ջ��С
extern TaskHandle_t StartTask_Handler;        //������
void Start_task(void *pvParameters);   //������

//ɽ����λ����������
#define SANWAI_TASK_PRIO 0              //�������ȼ�
#define SANWAI_STK_SIZE 30             //�����ջ��С
extern TaskHandle_t SanwaiTask_Handler;        //������
void Sanwai_task(void *p_arg);          //������
#endif

