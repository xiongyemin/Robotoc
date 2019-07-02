#include "usart.h"
#if SYSTEM_SUPPORT_OS
#include "FreeRTOS.h"					//os ʹ��	  
#endif
//ʹ��printf����
#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the EVAL_COM1 and Loop until the end of transmission */
  HAL_UART_Transmit(&huart6, (uint8_t *)&ch, 1, 0xFFFF);
  return ch;
}
u8 UART_Buffer[USART_LEN];
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart6;
DMA_HandleTypeDef hdma_usart1_rx;
QueueHandle_t Remote_msg;//ң��������
SemaphoreHandle_t Remote_Sem;//ң�����ź���
void MY_USART1_UART_Init(void)
{

  huart1.Instance = USART1;
  huart1.Init.BaudRate = 100000;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_EVEN;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  HAL_UART_Init(&huart1);
}

void MY_USART6_UART_Init(void){
  huart6.Instance = USART6;
  huart6.Init.BaudRate = 115200;
  huart6.Init.WordLength = UART_WORDLENGTH_8B;
  huart6.Init.StopBits = UART_STOPBITS_1;
  huart6.Init.Parity = UART_PARITY_NONE;
  huart6.Init.Mode = UART_MODE_TX_RX;
  huart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart6.Init.OverSampling = UART_OVERSAMPLING_16;
  HAL_UART_Init(&huart6);
  HAL_UART_Receive_IT_IDLE(&huart1,UART_Buffer,USART_LEN);   //�������ڽ���
}


/**e
  * @brief  ���ô���û��DMA�жϵĽ���
	*					����ʹ���˿����жϣ���������û�б�Ҫ�ٿ���DMA�жϣ�����HAL����������UART_Receive_DMA����Ĭ�Ͽ�����DMA�жϣ�
	*					����Ҫ�Լ�ʵ��һ��û���жϵ�DMA���պ�����
  *         
  * @param  
  * @retval HAL status
  */
HAL_StatusTypeDef UART_Receive_DMA_NoIT(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size)
{
 uint32_t *tmp;
  
  /* Check that a Rx process is not already ongoing */
  if(huart->RxState == HAL_UART_STATE_READY) 
  {
    if((pData == NULL) || (Size == 0U))
    {
      return HAL_ERROR;
    }

    /* Process Locked */
    __HAL_LOCK(huart);

    huart->pRxBuffPtr = pData;
    huart->RxXferSize = Size;

    huart->ErrorCode = HAL_UART_ERROR_NONE;
    huart->RxState = HAL_UART_STATE_BUSY_RX;
    /* Set the DMA abort callback */
    huart->hdmarx->XferAbortCallback = NULL;

    /* Enable the DMA channel */
    tmp = (uint32_t*)&pData;
    HAL_DMA_Start(huart->hdmarx, (uint32_t)&huart->Instance->DR, *(uint32_t*)tmp, Size);

    /* Clear the Overrun flag just before enabling the DMA Rx request: can be mandatory for the second transfer */
    __HAL_UART_CLEAR_OREFLAG(huart);

    /* Process Unlocked */
    __HAL_UNLOCK(huart);

    /* Enable the UART Parity Error Interrupt */
    SET_BIT(huart->Instance->CR1, USART_CR1_PEIE);

    /* Enable the UART Error Interrupt: (Frame error, noise error, overrun error) */
    SET_BIT(huart->Instance->CR3, USART_CR3_EIE);

    /* Enable the DMA transfer for the receiver request by setting the DMAR bit 
    in the UART CR3 register */
    SET_BIT(huart->Instance->CR3, USART_CR3_DMAR);

    return HAL_OK;
  }
  else
  {
    return HAL_BUSY;
  }
}




HAL_StatusTypeDef HAL_UART_Receive_IT_IDLE(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size){

	
		__HAL_UART_ENABLE_IT(huart, UART_IT_IDLE);//�������ڿ����ж�
	
		return UART_Receive_DMA_NoIT(huart,pData,Size);//����û��DMA�жϵ�DMA���պ���


}


void HAL_UART_IDLE_IRQHandler(UART_HandleTypeDef *huart)
{
		
		uint32_t DMA_FLAGS,tmp;
	BaseType_t Remote_HigherPriorityTaskWoken;
			
	if(__HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE)){
			
		tmp = huart->Instance->SR;
		tmp = huart->Instance->DR;
		tmp++;  
		DMA_FLAGS = __HAL_DMA_GET_TC_FLAG_INDEX(huart->hdmarx);
		
		//??DMA
	  __HAL_DMA_DISABLE(huart->hdmarx);
		__HAL_DMA_CLEAR_FLAG(huart->hdmarx,DMA_FLAGS);
		
		huart->hdmarx->Instance->NDTR = huart->RxXferSize;
		__HAL_DMA_ENABLE(huart->hdmarx);
		
	}
	//�ͷ��ź���	
	xSemaphoreGiveFromISR(Remote_Sem,&Remote_HigherPriorityTaskWoken);
	if(Remote_HigherPriorityTaskWoken==pdTRUE)portYIELD_FROM_ISR(Remote_HigherPriorityTaskWoken);
}

//����1�жϺ���������ң��������

void USART1_IRQHandler(void)
{
  /* USER CODE BEGIN USART1_IRQn 0 */
  
  /*< ע�⣡ ����ʹ���˴��ڿ����жϣ�HAL����Ĭ�ϵĻص������޷�����������Ҫ���������Զ���ص�������������Ҫ��Ĭ�ϻص����� >*/
  
	HAL_UART_IDLE_IRQHandler(&huart1);
    
  /* USER CODE END USART1_IRQn 0 */
  //HAL_UART_IRQHandler(&huart1);
  /* USER CODE BEGIN USART1_IRQn 1 */
	
  /* USER CODE END USART1_IRQn 1 */
}
