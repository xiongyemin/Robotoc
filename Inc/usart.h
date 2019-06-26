#ifndef MY_USART
#define MY_USART
#include "main.h"
#include "Remote_Ctrl.h"
#define USART_LEN 100
#define USART_QUE_NUM 2
extern u8 UART_Buffer[USART_LEN];
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart6;
extern DMA_HandleTypeDef hdma_usart1_rx;
extern QueueHandle_t Remote_msg;
void MY_USART1_UART_Init(void);
void MY_USART6_UART_Init(void);
void HAL_UART_IDLE_IRQHandler(UART_HandleTypeDef *huart);
HAL_StatusTypeDef UART_Receive_DMA_NoIT(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef HAL_UART_Receive_IT_IDLE(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size);
#endif
