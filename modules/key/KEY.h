#ifndef __KEY_H__
#define __KEY_H__

#include "gpio.h"


// 按键事件
typedef enum _KEY_EventList_TypeDef 
{
	KEY_Event_Null 		   = 0x00,  // 无事件
	KEY_Event_SingleClick  = 0x01,  // 单击
	KEY_Event_DoubleClick  = 0x02,  // 双击
	KEY_Event_LongPress    = 0x04   // 长按
}KEY_EventList_TypeDef;


// 按键状态
typedef enum _KEY_StatusList_TypeDef 
{
	KEY_Status_Idle = 0				, // 空闲
	KEY_Status_Debounce   		    , // 消抖
	KEY_Status_ConfirmPress		    , // 确认按下	
	KEY_Status_ConfirmPressLong		, // 确认长按着	
	KEY_Status_WaiteAgain		    , // 等待再次按下
	KEY_Status_SecondPress          , // 第二次按下
}KEY_StatusList_TypeDef;


// 按键动作，按下、释放
typedef enum
{ 
		KEY_Action_Release = 0,
	KEY_Action_Press

}KEY_Action_TypeDef;

// 按键引脚的电平
typedef enum
{ 
	KKEY_PinLevel_Low = 0,
	KEY_PinLevel_High
}KEY_PinLevel_TypeDef;


// 按键配置结构体
typedef struct _KEY_Configure_TypeDef 
{
	uint16_t                        KEY_Count;        //按键长按计数
	KEY_Action_TypeDef             KEY_Action;        //按键动作，按下或抬起
	KEY_StatusList_TypeDef         KEY_Status;        //按键状态
	KEY_EventList_TypeDef          KEY_Event;          //按键事件
	KEY_PinLevel_TypeDef          (*KEY_ReadPin_Fcn)(void);    //读IO电平函数
}KEY_Configure_TypeDef;


void KEY_ReadStateMachine(void);



























//#include "stm32f1xx_hal.h"

//#define KEY1 1
//#define KEY2 2
//#define KEY3 3
//#define KEY4 4
//#define KEY5 5  
//#define KEY6 6

//typedef enum
//{ 
//	KEY_RESERT = 0,
//	KEY_SET,
//	
//}KEY_STATE;

//typedef enum
//{	
//    KEY1_SINGLE_CLICK = 1,
//    KEY1_DOUBLE_CLICK,
//    KEY1_LONG_PRESS,
//    KEY2_SINGLE_CLICK,
//    KEY2_DOUBLE_CLICK,
//    KEY2_LONG_PRESS,
//    KEY3_SINGLE_CLICK,
//    KEY3_DOUBLE_CLICK,
//    KEY3_LONG_PRESS,
//    KEY4_SINGLE_CLICK,
//    KEY4_DOUBLE_CLICK,
//    KEY4_LONG_PRESS,
//    KEY5_SINGLE_CLICK,
//    KEY5_DOUBLE_CLICK,
//    KEY5_LONG_PRESS,
//    KEY6_SINGLE_CLICK,
//    KEY6_DOUBLE_CLICK,
//    KEY6_LONG_PRESS,
//} KEY_EVENT;

//uint8_t read_button_GPIO(uint8_t KEY_nun);

//void KEY_Init(void);


//uint8_t button1_callback(void );
//uint8_t button2_callback(void );
//uint8_t button3_callback(void );
//uint8_t button4_callback(void );
//uint8_t button5_callback(void );
//uint8_t button6_callback(void );


#endif





