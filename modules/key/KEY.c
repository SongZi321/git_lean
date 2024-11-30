#include "KEY.h"
#include "stdio.h"
#include "usart.h"

static KEY_PinLevel_TypeDef KEY_ReadPin(void) ;



/**************************************************************************************************** 
*                             长按、单击、双击定义
* 长按事件：任何大于 KEY_LONG_PRESS_TIME 
* 单击事件：按下时间不超过 KEY_LONG_PRESS_TIME 且 释放后 KEY_WAIT_DOUBLE_TIME 内无再次按下的操作
* 双击事件：俩次短按时间间隔小于KEY_WAIT_DOUBLE_TIME，俩次短按操作合并为一次双击事件。
* 特殊说明：
*          1.短按和长按时间间隔小于 KEY_WAIT_DOUBLE_TIME，响应一次单击和长按事件，不响应双击事件
*          2.连续2n次短按，且时间间隔小于 KEY_WAIT_DOUBLE_TIME，响应为n次双击
*          3.连续2n+1次短按，且时间间隔小于 KEY_WAIT_DOUBLE_TIME，且最后一次KEY_WAIT_DOUBLE_TIME内无操作，
*				响应为n次双击 和 一次单击事件
****************************************************************************************************/
#define KEY_LONG_PRESS_TIME    30//50 // 20ms*50 = 1s  
#define KEY_WAIT_DOUBLE_TIME  15//25// 20ms*25 = 500   
#define KEY_PRESSED_LEVEL     0     //  按键按下是电平为低    
                                                    
/**************************************************************************************************** 
*                            按键配置信息的全局结构体变量
****************************************************************************************************/
/**************************************************************************************************** 
*                             全局变量
*******************************************
*********************************************************/
KEY_Configure_TypeDef KeyCfg={		
		0,						//按键长按计数
		KEY_Action_Release,		//按键动作，按下或者抬起
		KEY_Status_Idle,        //按键状态
		KEY_Event_Null,         //按键事件
		KEY_ReadPin             //读IO电平函数
};


/**************************************************************************************************** 
*                             函数定义
****************************************************************************************************/
// 按键读取按键的电平函数，更具实际情况修改
static KEY_PinLevel_TypeDef KEY_ReadPin(void) //按键读取函数
{
  return (KEY_PinLevel_TypeDef) HAL_GPIO_ReadPin(KEY1_GPIO_Port,KEY1_Pin);
}
             

// 获取按键动作，按下或释放，保存到结构体
static void KEY_GetAction(void) 
{
	if(KeyCfg.KEY_ReadPin_Fcn() == KEY_PRESSED_LEVEL)
	{
		KeyCfg.KEY_Action = KEY_Action_Press;
	}
	else
	{
		KeyCfg.KEY_Action =  KEY_Action_Release;
	}
 
}




/**************************************************************************************************** 
*                             读取按键状态机
****************************************************************************************************/
void KEY_ReadStateMachine(void)
{
	
    KEY_GetAction();
	
	switch(KeyCfg.KEY_Status)
	{
		//状态：没有按键按下
		case KEY_Status_Idle:
			if(KeyCfg.KEY_Action == KEY_Action_Press)
			{
				KeyCfg.KEY_Status = KEY_Status_Debounce;
				KeyCfg.KEY_Event = KEY_Event_Null;
			}
			else
			{
				KeyCfg.KEY_Status = KEY_Status_Idle;
				KeyCfg.KEY_Event = KEY_Event_Null;
				  KeyCfg.KEY_Count = 0;
			}
			break;
			
		//状态：消抖
		case KEY_Status_Debounce:

			if(KeyCfg.KEY_Action == KEY_Action_Press)
			{
				KeyCfg.KEY_Status = KEY_Status_ConfirmPress;
				KeyCfg.KEY_Event = KEY_Event_Null;
				
			}
			else
			{
				KeyCfg.KEY_Status = KEY_Status_Idle;
				KeyCfg.KEY_Event = KEY_Event_Null;
				  KeyCfg.KEY_Count = 0;
			}
			break;	


		//状态：继续按下
		case KEY_Status_ConfirmPress:
			if( (KeyCfg.KEY_Action == KEY_Action_Press) && ( KeyCfg.KEY_Count >= KEY_LONG_PRESS_TIME))
			{
				KeyCfg.KEY_Count = 0;
				KeyCfg.KEY_Status = KEY_Status_ConfirmPressLong;
				KeyCfg.KEY_Event = KEY_Event_Null;
			}
			else if( (KeyCfg.KEY_Action == KEY_Action_Press) && (KeyCfg.KEY_Count < KEY_LONG_PRESS_TIME))
			{
				KeyCfg.KEY_Count++;
				KeyCfg.KEY_Status = KEY_Status_ConfirmPress;
				KeyCfg.KEY_Event = KEY_Event_Null;
			}
			else
			{
				KeyCfg.KEY_Count = 0;
				KeyCfg.KEY_Status = KEY_Status_WaiteAgain;// 按短了后释放
				KeyCfg.KEY_Event = KEY_Event_Null;

			}
			break;	
			
		//状态：一直长按着
		case KEY_Status_ConfirmPressLong:
			if(KeyCfg.KEY_Action == KEY_Action_Press) 
			{   // 一直等待其放开
				KeyCfg.KEY_Status = KEY_Status_ConfirmPressLong;
				KeyCfg.KEY_Event = KEY_Event_Null;
				KeyCfg.KEY_Count = 0;
			}
			else
			{
				KeyCfg.KEY_Status = KEY_Status_Idle;
				KeyCfg.KEY_Event = KEY_Event_LongPress;
				KeyCfg.KEY_Count = 0;
			}
			break;	
			
		//状态：等待是否再次按下
		case KEY_Status_WaiteAgain:
			if((KeyCfg.KEY_Action != KEY_Action_Press) && ( KeyCfg.KEY_Count >= KEY_WAIT_DOUBLE_TIME))
			{   // 第一次短按,且释放时间大于KEY_WAIT_DOUBLE_TIME
				KeyCfg.KEY_Count = 0;
				KeyCfg.KEY_Status = KEY_Status_Idle;  
				KeyCfg.KEY_Event = KEY_Event_SingleClick;// 响应单击
				
			}
			else if((KeyCfg.KEY_Action != KEY_Action_Press) && ( KeyCfg.KEY_Count < KEY_WAIT_DOUBLE_TIME))
			{// 第一次短按,且释放时间还没到KEY_WAIT_DOUBLE_TIME
				KeyCfg.KEY_Count ++;
				KeyCfg.KEY_Status = KEY_Status_WaiteAgain;// 继续等待
				KeyCfg.KEY_Event = KEY_Event_Null;
				
			}
			else // 第一次短按,且还没到KEY_WAIT_DOUBLE_TIME 第二次被按下
			{
				KeyCfg.KEY_Status = KEY_Status_SecondPress;// 第二次按下
				KeyCfg.KEY_Event = KEY_Event_Null;
				KeyCfg.KEY_Count = 0;
			}
			break;		
		case KEY_Status_SecondPress:
			if( (KeyCfg.KEY_Action == KEY_Action_Press) && ( KeyCfg.KEY_Count >= KEY_LONG_PRESS_TIME))
			{
				KeyCfg.KEY_Status = KEY_Status_Idle;// 第二次按的时间大于 KEY_LONG_PRESS_TIME    ///
				KeyCfg.KEY_Event = KEY_Event_SingleClick; // 先响应单击
				KeyCfg.KEY_Count = 0;
			}
			else if( (KeyCfg.KEY_Action == KEY_Action_Press) && ( KeyCfg.KEY_Count < KEY_LONG_PRESS_TIME))
			{
                KeyCfg.KEY_Count ++;
				KeyCfg.KEY_Status = KEY_Status_SecondPress;
				KeyCfg.KEY_Event = KEY_Event_Null;
			}
            else 
            {// 第二次按下后在 KEY_LONG_PRESS_TIME内释放
                KeyCfg.KEY_Count = 0;
				KeyCfg.KEY_Status = KEY_Status_Idle;
				KeyCfg.KEY_Event = KEY_Event_DoubleClick; // 响应双击
            }
			break;	
		default:
			break;
	}

}

















//#include "stm32f1xx_hal.h"

//#include "main.h"
//#include "multi_button.h"
//#include "stdio.h"
//struct Button KEY_1;
//struct Button KEY_2;
//struct Button KEY_3;
//struct Button KEY_4;
//struct Button KEY_5;
//struct Button KEY_6;

//uint8_t KEY_flag=0;

//uint8_t read_button1_GPIO(void)
//{
//  return HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin);
//}
//uint8_t read_button2_GPIO(void)
//{
//  return HAL_GPIO_ReadPin(KEY2_GPIO_Port, KEY2_Pin);
//}
//uint8_t read_button3_GPIO(void)
//{
// return HAL_GPIO_ReadPin(KEY3_GPIO_Port, KEY3_Pin);
//}
//uint8_t read_button4_GPIO(void)
//{
// return HAL_GPIO_ReadPin(KEY4_GPIO_Port, KEY4_Pin);
//}
//uint8_t read_button5_GPIO(void)
//{
// return HAL_GPIO_ReadPin(KEY5_GPIO_Port, KEY5_Pin);
//}
//uint8_t read_button6_GPIO(void)
//{
//  return HAL_GPIO_ReadPin(KEY6_GPIO_Port, KEY6_Pin);
//}
////uint8_t read_button_GPIO(uint8_t KEY_nun)
////{
////    switch (KEY_nun)
////    {
////    case 1:
////        return HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin);
////        break;

////    case 2:
////        return HAL_GPIO_ReadPin(KEY2_GPIO_Port, KEY2_Pin);
////        break;

////    case 3:
////        return HAL_GPIO_ReadPin(KEY3_GPIO_Port, KEY3_Pin);
////        break;

////    case 4:
////        return HAL_GPIO_ReadPin(KEY4_GPIO_Port, KEY4_Pin);
////        break;

////    case 5:
////        return HAL_GPIO_ReadPin(KEY5_GPIO_Port, KEY5_Pin);
////        break;

////    case 6:
////        return HAL_GPIO_ReadPin(KEY6_GPIO_Port, KEY6_Pin);
////        
////    }
////}

//uint8_t button1_callback()
//{

//    uint32_t button_event_val;

//    button_event_val = get_button_event((struct Button *)&KEY_1);
//	
//	printf("button_event_val=%d\r\n",button_event_val);

//    switch (button_event_val)
//    {
//    case SINGLE_CLICK:
//		
//       KEY_flag = SINGLE_CLICK; // 单击事件
//			printf("button_event_val=%d\r\n",button_event_val);
//					printf("0000\r\n");
//        break;
//    case DOUBLE_CLICK:
//		
//         KEY_flag = DOUBLE_CLICK;// 双击事件
//			printf("button_event_val=%d\r\n",button_event_val);
//					printf("1111\r\n");
//        break;
//		
//    case LONG_PRESS_START:
//		printf("KEY_flag=%d\r\n",KEY_flag);
//         KEY_flag = LONG_PRESS_START; // 长按事件
//					printf("2222\r\n");
//    return KEY_flag;
//			
//    }
//}

//uint8_t button2_callback()
//{

//    uint32_t button_event_val;

//    button_event_val = get_button_event((struct Button *)&KEY_2);

//    switch (button_event_val)
//    {

//    case SINGLE_CLICK:
//        return KEY2_SINGLE_CLICK; // 单击事件 // 单击事件
//       // break;
//    case DOUBLE_CLICK:
//        return KEY2_DOUBLE_CLICK; // 双击事件
//      //  break;
//    case LONG_PRESS_START:
//        return KEY2_LONG_PRESS; // 长按事件
//    default :
//			return 0;
//    }
//}

//uint8_t button3_callback()
//{

//    uint32_t button_event_val;

//    button_event_val = get_button_event((struct Button *)&KEY_3);

//    switch (button_event_val)
//    {

//    case SINGLE_CLICK:
//        return KEY3_SINGLE_CLICK; // 单击事件
//     //   break;
//    case DOUBLE_CLICK:
//        return KEY3_DOUBLE_CLICK; // 双击事件
//     //   break;
//    case LONG_PRESS_START:
//        return KEY3_LONG_PRESS; // 长按事件
//    default :
//			return 0;
//    }
//}

//uint8_t button4_callback()
//{

//    uint32_t button_event_val;

//    button_event_val = get_button_event((struct Button *)&KEY_4);

//    switch (button_event_val)
//    {

//    case SINGLE_CLICK:
//        return KEY4_SINGLE_CLICK; // 单击事件
//      //  break;
//    case DOUBLE_CLICK:
//        return KEY4_DOUBLE_CLICK; // 双击事件
//      //  break;
//    case LONG_PRESS_START:
//        return KEY4_LONG_PRESS; // 长按事件
//    default :
//			return 0;
//    }
//}

//uint8_t button5_callback()
//{

//    uint32_t button_event_val;

//    button_event_val = get_button_event((struct Button *)&KEY_5);

//    switch (button_event_val)
//    {

//    case SINGLE_CLICK:
//        return KEY5_SINGLE_CLICK; // 单击事件
//     //   break;
//    case DOUBLE_CLICK:
//        return KEY5_DOUBLE_CLICK; // 双击事件
//      //  break;
//    case LONG_PRESS_START:
//        return KEY5_LONG_PRESS; // 长按事件
//		default :
//			return 0;
//    }
//}

//uint8_t button6_callback()
//{

//    uint32_t button_event_val;

//    button_event_val = get_button_event((struct Button *)&KEY_6);

//    switch (button_event_val)
//    {

//    case SINGLE_CLICK:
//        return KEY6_SINGLE_CLICK; // 单击事件
//      //  break;
//    case DOUBLE_CLICK:
//        return KEY6_DOUBLE_CLICK; // 双击事件
//   //     break;
//    case LONG_PRESS_START:
//        return KEY6_LONG_PRESS; // 长按事件
//    default:
//			return 0;  
//    }
//}

//void KEY_Init(void)
//{
//    button_init(&KEY_1, read_button1_GPIO, 0);
//    button_init(&KEY_2, read_button2_GPIO, 0);
//    button_init(&KEY_3, read_button3_GPIO, 0);
//    button_init(&KEY_4, read_button4_GPIO, 0);
//    button_init(&KEY_5, read_button5_GPIO, 0);
//    button_init(&KEY_6, read_button6_GPIO, 0);

//    button_attach(&KEY_1,SINGLE_CLICK, button1_callback);
//		button_attach(&KEY_1, 	DOUBLE_CLICK,	 button1_callback);
//		button_attach(&KEY_1, LONG_PRESS_START, button1_callback);
//    button_start(&KEY_1);

////    button_attach(&KEY_2, PRESS_DOWN, button2_callback);
////    button_attach(&KEY_2, PRESS_UP, button2_callback);
////    button_attach(&KEY_2, LONG_PRESS_START, button2_callback);
////    button_start(&KEY_2);

////    button_attach(&KEY_3, PRESS_DOWN, button3_callback);
////    button_attach(&KEY_3, PRESS_UP, button3_callback);
////    button_attach(&KEY_3, LONG_PRESS_START, button3_callback);
////    button_start(&KEY_3);

////    button_attach(&KEY_4, PRESS_DOWN, button4_callback);
////    button_attach(&KEY_4, PRESS_UP, button4_callback);
////    button_attach(&KEY_4, LONG_PRESS_START, button4_callback);
////    button_start(&KEY_4);

////    button_attach(&KEY_5, PRESS_DOWN, button5_callback);
////    button_attach(&KEY_5, PRESS_UP, button5_callback);
////    button_attach(&KEY_5, LONG_PRESS_START, button5_callback);
////    button_start(&KEY_5);

////    button_attach(&KEY_6, PRESS_DOWN, button6_callback);
////    button_attach(&KEY_6, PRESS_UP, button6_callback);
////    button_attach(&KEY_6, LONG_PRESS_START, button6_callback);
////    button_start(&KEY_6);
//		
//}







