#ifndef __KEY_H__
#define __KEY_H__

#include "gpio.h"


// �����¼�
typedef enum _KEY_EventList_TypeDef 
{
	KEY_Event_Null 		   = 0x00,  // ���¼�
	KEY_Event_SingleClick  = 0x01,  // ����
	KEY_Event_DoubleClick  = 0x02,  // ˫��
	KEY_Event_LongPress    = 0x04   // ����
}KEY_EventList_TypeDef;


// ����״̬
typedef enum _KEY_StatusList_TypeDef 
{
	KEY_Status_Idle = 0				, // ����
	KEY_Status_Debounce   		    , // ����
	KEY_Status_ConfirmPress		    , // ȷ�ϰ���	
	KEY_Status_ConfirmPressLong		, // ȷ�ϳ�����	
	KEY_Status_WaiteAgain		    , // �ȴ��ٴΰ���
	KEY_Status_SecondPress          , // �ڶ��ΰ���
}KEY_StatusList_TypeDef;


// �������������¡��ͷ�
typedef enum
{ 
		KEY_Action_Release = 0,
	KEY_Action_Press

}KEY_Action_TypeDef;

// �������ŵĵ�ƽ
typedef enum
{ 
	KKEY_PinLevel_Low = 0,
	KEY_PinLevel_High
}KEY_PinLevel_TypeDef;


// �������ýṹ��
typedef struct _KEY_Configure_TypeDef 
{
	uint16_t                        KEY_Count;        //������������
	KEY_Action_TypeDef             KEY_Action;        //�������������»�̧��
	KEY_StatusList_TypeDef         KEY_Status;        //����״̬
	KEY_EventList_TypeDef          KEY_Event;          //�����¼�
	KEY_PinLevel_TypeDef          (*KEY_ReadPin_Fcn)(void);    //��IO��ƽ����
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





