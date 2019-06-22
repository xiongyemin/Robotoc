#ifndef MYTIM
#define MYTIM
#include "main.h"
#include "usart.h"
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim4;
void MY_TIM2_Init(void);
void MY_TIM4_Pwm_Init(void);
#endif
