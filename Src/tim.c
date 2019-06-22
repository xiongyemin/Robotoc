#include "tim.h"
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim4;
u8 cnt=0;
char s[]="abs";
void MY_TIM2_Init(void)
{
	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};
	htim2.Instance = TIM2;
	htim2.Init.Prescaler = 83;
	htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim2.Init.Period = 4999;
	htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	HAL_TIM_Base_Init(&htim2);
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig);
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig);
	HAL_TIM_Base_Start_IT(&htim2);
}
void MY_TIM4_Pwm_Init(void){
	TIM_OC_InitTypeDef sConfigOC;
	htim4.Instance = TIM4;
	htim4.Init.Prescaler = 83;
	htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim4.Init.Period = 19999;
	htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	HAL_TIM_PWM_Init(&htim4);
	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = 9999;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	HAL_TIM_PWM_ConfigChannel(&htim4,&sConfigOC,TIM_CHANNEL_1);
	HAL_TIM_PWM_ConfigChannel(&htim4,&sConfigOC,TIM_CHANNEL_2);
	HAL_TIM_PWM_ConfigChannel(&htim4,&sConfigOC,TIM_CHANNEL_3);
	HAL_TIM_PWM_ConfigChannel(&htim4,&sConfigOC,TIM_CHANNEL_4);
	HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_4);
}

//TIM2中断函数,主要任务调度
void TIM2_IRQHandler(void){
	HAL_TIM_IRQHandler(&htim2);
	if(cnt<200)cnt++;
	else {
		HAL_GPIO_TogglePin(GPIOG,GPIO_PIN_1|GPIO_PIN_2);
		cnt=0;
	}
 //printf("hello");
//	HAL_UART_Transmit(&huart6,s,1,10);
}
