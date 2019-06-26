#include "Remote_Ctrl.h"
#include "Sanwai.h"
#include "usart.h"
RC_Type remote_control;
uint32_t  Latest_Remote_Control_Pack_Time = 0;
uint32_t  LED_Flash_Timer_remote_control = 0;
uint32_t dbus_time=0;//用于检测遥控器是否离线
TaskHandle_t RemoteTask_Handler;
void Callback_RC_Handle(RC_Type* rc, uint8_t* buff)
{
	rc->ch1 = (buff[0] | buff[1]<<8) & 0x07FF;
	rc->ch1 -= 1024;
	rc->ch2 = (buff[1]>>3 | buff[2]<<5 ) & 0x07FF;
	rc->ch2 -= 1024;
	rc->ch3 = (buff[2]>>6 | buff[3]<<2 | buff[4]<<10) & 0x07FF;
	rc->ch3 -= 1024;
	rc->ch4 = (buff[4]>>1 | buff[5]<<7) & 0x07FF;		
	rc->ch4 -= 1024;
	
	rc->switch_left = ( (buff[5] >> 4)& 0x000C ) >> 2;
	rc->switch_right =  (buff[5] >> 4)& 0x0003 ;
	
	rc->mouse.x = buff[6] | (buff[7] << 8);	//x axis
	rc->mouse.y = buff[8] | (buff[9] << 8);
	rc->mouse.z = buff[10]| (buff[11] << 8);
	
	rc->mouse.press_left 	= buff[12];	// is pressed?
	rc->mouse.press_right = buff[13];
	
	rc->keyBoard.key_code = buff[14] | buff[15] << 8; //key borad code
	
	Latest_Remote_Control_Pack_Time = HAL_GetTick();
	
	if(Latest_Remote_Control_Pack_Time - LED_Flash_Timer_remote_control>500) //让LED_T每1秒闪烁一次
		{
			
			HAL_GPIO_TogglePin(GPIOE,GPIO_PIN_11);
			
			LED_Flash_Timer_remote_control = Latest_Remote_Control_Pack_Time;
				
	}
	dbus_time=Latest_Remote_Control_Pack_Time;//获取时间
	HAL_GPIO_WritePin(GPIOG, GPIO_PIN_7,GPIO_PIN_SET);
}
void Remote_task(void *p_arg){
	RC_Type my_remote;
		while (1){
			if(xQueueReceive(Remote_msg,&my_remote,5)){
				wave_form_data[0]=(short)my_remote.ch1;
				wave_form_data[1]=(short)my_remote.ch2;
				wave_form_data[2]=(short)my_remote.ch3;
				wave_form_data[3]=(short)my_remote.ch4;
				wave_form_data[4]=(short)my_remote.switch_left;
				wave_form_data[5]=(short)my_remote.switch_right;
				shanwai_send_wave_form();
			}
	}
}
