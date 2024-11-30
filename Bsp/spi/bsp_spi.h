#ifndef __BSP_SPI_H
#define __BSP_SPI_H

#include "gpio.h"


typedef enum
{
	SPI_NULL_Mem = 0, //�������Ĵ���
	SPI_Write_Mem ,   //д�Ĵ���
	SPI_Read_Mem,     //���Ĵ���
	
}SPI_Access_Mode;

typedef enum
{
	SPI_NULL_MODE=0,  //�޴���ģʽ
	SPI_BLACK_MODE,   //����ģʽ
	SPI_IT_MODE,      //�ж�ģʽ
	SPI_DMA_MODE,     //DMA����

}SPI_Work_Mode;

typedef struct spi_temp_s
{
GPIO_TypeDef*  CE_24L01_GPIO_Port;
uint16_t       CE_24L01_Pin;
	
GPIO_TypeDef*  CSN_24L01_GPIO_Port;
 uint16_t      CSN_24L01_Pin;
	 
GPIO_TypeDef*  IQR_24L01_GPIO_Port;  
uint16_t       IQR_24L01_Pin; 
	
SPI_HandleTypeDef *bsp_spi_Handle;
} SPIInstance;

typedef struct 
{
 SPIInstance* spiinstance;
 void    (*Transmit)(SPIInstance* spi ,uint8_t *pData ,uint16_t size ,SPI_Work_Mode Work_Mode);//����HAL���SPIд�ֽں���
 void    (*Receive)(SPIInstance* spi ,uint8_t *pData ,uint16_t size, SPI_Work_Mode Work_Mode);//����HAL���SPI���ֽں���
 uint8_t (*Transrecv)(SPIInstance* spi ,uint8_t *txData ,SPI_Work_Mode Work_Mode);//����HAL���SPI��д�ֽں���
} SPI_t;


typedef void    (*spitransmit)(SPIInstance* spi ,uint8_t *pData ,uint16_t size ,SPI_Work_Mode Work_Mode);//����HAL���SPIд�ֽں���
typedef void    (*spireceive)(SPIInstance* spi ,uint8_t *pData ,uint16_t size, SPI_Work_Mode Work_Mode);//����HAL���SPI���ֽں���
typedef uint8_t (*spitransrecv)(SPIInstance* spi ,uint8_t *txData ,SPI_Work_Mode Work_Mode);//����HAL���SPI��д�ֽں���

/* ��ʼ�� */
void SPI_init(SPI_t* spi ,
	            SPIInstance* spiinstance,
              spitransmit Transmit,
              spireceive  Receive,
              spitransrecv Transrecv );


//����HAL���SPI���ͺ����ķ�װ
void SPITransmit(SPIInstance* spi ,uint8_t *pData ,uint16_t size ,SPI_Work_Mode Work_Mode);

//����HAL���SPI���պ����ķ�װ
void SPIReceive(SPIInstance* spi ,uint8_t *pData ,uint16_t size, SPI_Work_Mode   Work_Mode);

//����HAL���SPI��д�ֽں���
uint8_t  SPITransRecv(SPIInstance* spi ,uint8_t *txData ,SPI_Work_Mode Work_Mode);
#endif

