#ifndef LED
#define LED
#include "main.h"
//LED1����
#define LED1_TASK_PRIO 3              //�������ȼ�
#define LED1_STK_SIZE 50              //�����ջ��С
extern TaskHandle_t LED1_Handler;     //������
void Led1_task(void *p_arg);          //������
//LED2����
#define LED2_TASK_PRIO 3              //�������ȼ�
#define LED2_STK_SIZE 50              //�����ջ��С
extern TaskHandle_t LED2_Handler;     //������
void Led2_task(void *p_arg);          //������
//LED3����
#define LED3_TASK_PRIO 3              //�������ȼ�
#define LED3_STK_SIZE 50              //�����ջ��С
extern TaskHandle_t LED3_Handler;     //������
void Led3_task(void *p_arg);          //������
//LED4����
#define LED4_TASK_PRIO 3              //�������ȼ�
#define LED4_STK_SIZE 50              //�����ջ��С
extern TaskHandle_t LED4_Handler;     //������
void Led4_task(void *p_arg);          //������
#endif
