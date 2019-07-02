#ifndef REMOTE_CONTROL
#define REMOTE_CONTROL
#include "main.h"
typedef struct {
	int16_t ch1;	//each ch value from -364 -- +364
	int16_t ch2;
	int16_t ch3;
	int16_t ch4;
	
	uint8_t switch_left;	//3 value
	uint8_t switch_right;
	
	struct {
		int16_t x;
		int16_t y;
		int16_t z;
	
		uint8_t press_left;
		uint8_t press_right;
	}mouse;
	
	struct {
		uint16_t key_code;
/**********************************************************************************
   * 键盘通道:15      14     13    12     11    10    9     8     7    6      5         4   3   2   1
   *          V       C      X	   Z      G     F     R     E     Q    CTRL   SHIFT     D   A   S   W   
		     16384  8192   4096   2048   1024   512   256   128   64   32     16        8   4   2   1       
************************************************************************************/

	}keyBoard;
	

}RC_Type;
extern RC_Type remote_control;
void Callback_RC_Handle(RC_Type* rc, uint8_t* buff);
#define TICKTOWAIT_QUE 5
#define TICKTOWAIT_SEM 5
//主要根据遥控器数据处理电机
#define REMOTE_TASK_PRIO 4            //任务优先级
#define REMOTE_STK_SIZE 128           //任务堆栈大小
extern TaskHandle_t RemoteTask_Handler;        //任务句柄
void Remote_task(void *p_arg);   //任务函数
#define REMOTEINFO_TASK_PRIO 4 
#define REMOTEINFO_STK_SIZE 256
extern TaskHandle_t RemoteInfo_Handler;
void Remote_Info(void *p_arg);
#endif

