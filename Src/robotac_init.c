#include "robotac_init.h"
TaskHandle_t StartTask_Handler;        //任务句柄
TaskHandle_t SanwaiTask_Handler;        //任务句柄
void Robotac_Init(void){
	HAL_Init();
	SystemClock_Config();
	delay_init(168);
	MY_GPIO_Init();
	MY_TIM4_Pwm_Init();
	MY_DMA_Init();
	MY_USART1_UART_Init();//遥控器使用串口1
	MY_USART6_UART_Init();//山外使用的串口6
	MX_CAN1_Init();
	all_pid_init();//电机PID初始化
	xTaskCreate(	(TaskFunction_t) Start_task,
							(const char *) "Start_task",		
							(const configSTACK_DEPTH_TYPE) ROBOTAC_STK_SIZE,
							(void * ) NULL,
							(UBaseType_t) ROBOTAC_TASK_PRIO,
							(TaskHandle_t *) &StartTask_Handler );
	vTaskStartScheduler();
}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 6;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
      /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

void Start_task(void *pvParameters){
	taskENTER_CRITICAL();
	Remote_msg=xQueueCreate(USART_QUE_NUM,sizeof(RC_Type));//创建遥控器信息队列
	Remote_Sem=xSemaphoreCreateBinary();//创建遥控器信号量
	xTaskCreate(	(TaskFunction_t) Remote_Info,
							(const char *) "Remote_Info",		
							(const configSTACK_DEPTH_TYPE) REMOTEINFO_STK_SIZE,
							(void * ) NULL,
							(UBaseType_t) REMOTEINFO_TASK_PRIO,
							(TaskHandle_t *) &RemoteInfo_Handler );
	xTaskCreate(	(TaskFunction_t) Remote_task,
							(const char *) "Remote_task",		
							(const configSTACK_DEPTH_TYPE) REMOTE_STK_SIZE,
							(void * ) NULL,
							(UBaseType_t) REMOTE_TASK_PRIO,
							(TaskHandle_t *) &RemoteTask_Handler );
	xTaskCreate(	(TaskFunction_t) Led1_task,
							(const char *) "Led1_task",		
							(const configSTACK_DEPTH_TYPE) LED1_STK_SIZE,
							(void * ) NULL,
							(UBaseType_t) LED1_TASK_PRIO,
							(TaskHandle_t *) &LED1_Handler );
	xTaskCreate(	(TaskFunction_t) Sanwai_task,
							(const char *) "Sanwai_task",		
							(const configSTACK_DEPTH_TYPE) SANWAI_STK_SIZE,
							(void * ) NULL,
							(UBaseType_t) SANWAI_TASK_PRIO,
							(TaskHandle_t *) &SanwaiTask_Handler );
	vTaskDelete(StartTask_Handler);
	taskEXIT_CRITICAL();
}

void Sanwai_task(void *p_arg){
	while (1){
	  wave_form_data[0]=(short)remote_control.ch1;
	  wave_form_data[1]=(short)remote_control.ch2;
	  wave_form_data[2]=(short)remote_control.ch3;
	  wave_form_data[3]=(short)remote_control.ch4;
	  wave_form_data[4]=(short)remote_control.switch_left;
	  wave_form_data[5]=(short)remote_control.switch_right;
	  shanwai_send_wave_form();
  }
}



