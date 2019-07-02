#include "Remote_Ctrl.h"
#include "Sanwai.h"
#include "usart.h"
#include "bsp_can.h"
#include "pid.h"
RC_Type remote_control;
uint32_t  Latest_Remote_Control_Pack_Time = 0;
uint32_t  LED_Flash_Timer_remote_control = 0;
uint32_t dbus_time=0;//用于检测遥控器是否离线
TaskHandle_t RemoteTask_Handler;
TaskHandle_t RemoteInfo_Handler;
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
	int32_t set_spd = 0;
		while (1){
			if(xQueueReceive(Remote_msg,&my_remote,TICKTOWAIT_QUE)){
			set_spd = my_remote.ch4*8000/660;
			for(int i=0; i<4; i++)
				{	
					motor_pid[i].target = set_spd; 																							
					motor_pid[i].f_cal_pid(&motor_pid[i],moto_chassis[i].speed_rpm);    //根据设定值进行PID计算。
				}
				set_moto_current(&hcan1, &My_TxHeader,motor_pid[0].output,   //将PID的计算结果通过CAN发送到电机
                        motor_pid[1].output,
                        motor_pid[2].output,
                        motor_pid[3].output);
		}
	}
}
void Remote_Info(void *p_arg){
	//获取信号量
	while(1){
		if(xSemaphoreTake(Remote_Sem,TICKTOWAIT_SEM)){
			Callback_RC_Handle(&remote_control,huart1.pRxBuffPtr);
			xQueueSend(Remote_msg,&remote_control,TICKTOWAIT_SEM);
		}
	}
}
