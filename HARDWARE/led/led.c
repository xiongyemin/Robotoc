#include "led.h"
TaskHandle_t LED1_Handler;
TaskHandle_t LED2_Handler;
TaskHandle_t LED3_Handler;
TaskHandle_t LED4_Handler;
void Led1_task(void *p_arg){
	while(1){
		HAL_GPIO_TogglePin(GPIOG,GPIO_PIN_1|GPIO_PIN_2);
		vTaskDelay(1000);
	}
}
void Led2_task(void *p_arg){
	while(1){
		HAL_GPIO_TogglePin(GPIOG,GPIO_PIN_2);
		vTaskDelay(1000);
	}
}
void Led3_task(void *p_arg){
	while(1){
		HAL_GPIO_TogglePin(GPIOG,GPIO_PIN_3);
		vTaskDelay(1000);
	}
}
void Led4_task(void *p_arg){
	u8 cnt=0;
	while(1){
		if(cnt==5){
			vTaskDelete(LED3_Handler);
			cnt++;
		}
		HAL_GPIO_TogglePin(GPIOG,GPIO_PIN_4);
		if(cnt<5)cnt++;
		vTaskDelay(1000);
	}
}
