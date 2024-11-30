#ifndef __BSP_SPI_H
#define __BSP_SPI_H

#include "gpio.h"


typedef enum
{
	SPI_NULL_Mem = 0, //不操作寄存器
	SPI_Write_Mem ,   //写寄存器
	SPI_Read_Mem,     //读寄存器
	
}SPI_Access_Mode;

typedef enum
{
	SPI_NULL_MODE=0,  //无传输模式
	SPI_BLACK_MODE,   //阻塞模式
	SPI_IT_MODE,      //中断模式
	SPI_DMA_MODE,     //DMA传输

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
 void    (*Transmit)(SPIInstance* spi ,uint8_t *pData ,uint16_t size ,SPI_Work_Mode Work_Mode);//基于HAL库的SPI写字节函数
 void    (*Receive)(SPIInstance* spi ,uint8_t *pData ,uint16_t size, SPI_Work_Mode Work_Mode);//基于HAL库的SPI读字节函数
 uint8_t (*Transrecv)(SPIInstance* spi ,uint8_t *txData ,SPI_Work_Mode Work_Mode);//基于HAL库的SPI读写字节函数
} SPI_t;


typedef void    (*spitransmit)(SPIInstance* spi ,uint8_t *pData ,uint16_t size ,SPI_Work_Mode Work_Mode);//基于HAL库的SPI写字节函数
typedef void    (*spireceive)(SPIInstance* spi ,uint8_t *pData ,uint16_t size, SPI_Work_Mode Work_Mode);//基于HAL库的SPI读字节函数
typedef uint8_t (*spitransrecv)(SPIInstance* spi ,uint8_t *txData ,SPI_Work_Mode Work_Mode);//基于HAL库的SPI读写字节函数

/* 初始化 */
void SPI_init(SPI_t* spi ,
	            SPIInstance* spiinstance,
              spitransmit Transmit,
              spireceive  Receive,
              spitransrecv Transrecv );


//基于HAL库的SPI发送函数的封装
void SPITransmit(SPIInstance* spi ,uint8_t *pData ,uint16_t size ,SPI_Work_Mode Work_Mode);

//基于HAL库的SPI接收函数的封装
void SPIReceive(SPIInstance* spi ,uint8_t *pData ,uint16_t size, SPI_Work_Mode   Work_Mode);

//基于HAL库的SPI读写字节函数
uint8_t  SPITransRecv(SPIInstance* spi ,uint8_t *txData ,SPI_Work_Mode Work_Mode);
#endif

