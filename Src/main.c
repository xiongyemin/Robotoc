#include "main.h"
#include "robotac_init.h"

int main(void)
{
  Robotac_Init();//所有的初始化
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




/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
