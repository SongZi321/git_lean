#include "bsp_spi.h"
#include "stdio.h"
#include "usart.h"


void SPI_init(SPI_t* spi ,
	            SPIInstance* spiinstance,
              spitransmit Transmit,
              spireceive  Receive,
              spitransrecv Transrecv )
{
spi->spiinstance = spiinstance;
spi->Receive     = Receive;
spi->Transmit    = Transmit;
spi->Transrecv   = Transrecv;
}

 


//����HAL���SPI���ͺ����ķ�װ
void SPITransmit(SPIInstance* spi ,uint8_t *pData ,uint16_t size ,SPI_Work_Mode Work_Mode)
{
		switch(Work_Mode)
		{
      case SPI_IT_MODE:
		 	 HAL_SPI_Transmit_IT(spi->bsp_spi_Handle , pData, size);
			break;
			
			 case SPI_DMA_MODE:
			  HAL_SPI_Transmit_DMA(spi->bsp_spi_Handle , pData, size);
			break;
			
			case SPI_BLACK_MODE:
		  	HAL_SPI_Transmit(spi->bsp_spi_Handle , pData, size,100);
        HAL_GPIO_WritePin(spi->CSN_24L01_GPIO_Port,spi->CSN_24L01_Pin ,GPIO_PIN_SET );
			break;
			
			case SPI_NULL_MODE:
			 printf("SPITransmit:error\r\n");
			break;
			default:
	    while(1); //״̬��������ģʽ��ֹͣ���䣻
			
		}
}
//����HAL���SPI���պ����ķ�װ
void SPIReceive(SPIInstance* spi ,uint8_t *pData ,uint16_t size, SPI_Work_Mode   Work_Mode)
{
		switch(Work_Mode)
		{
      case SPI_IT_MODE:
			 HAL_SPI_Receive_IT(spi->bsp_spi_Handle , pData, size);
		  	break;
			
			case SPI_DMA_MODE:
			 HAL_SPI_Receive_DMA(spi->bsp_spi_Handle , pData, size);
			 break;
			
			case SPI_BLACK_MODE:
		   HAL_SPI_Receive(spi->bsp_spi_Handle , pData, size,100);
			 // ����ģʽ������ûص�����,������ɺ�ֱ������Ƭѡ����
		 	 HAL_GPIO_WritePin(spi->CSN_24L01_GPIO_Port,spi->CSN_24L01_Pin ,GPIO_PIN_SET );
		   break;
			
			case SPI_NULL_MODE:
			 printf("SPIReceive:error\r\n");
			break;
		  default:
	     while(1); //״̬��������ģʽ��ֹͣ���䣻
			 
		}
}

//����HAL���SPI��д�ֽں���
uint8_t  SPITransRecv(SPIInstance* spi ,uint8_t *txData ,SPI_Work_Mode Work_Mode)
{
	uint8_t rxData=0;
		switch(Work_Mode )
		{
      case SPI_IT_MODE:
			 HAL_SPI_TransmitReceive_IT(spi->bsp_spi_Handle , txData, &rxData, 1);
			return rxData;
		  	
			
			case SPI_DMA_MODE:
			 HAL_SPI_TransmitReceive_DMA(spi->bsp_spi_Handle , txData, &rxData, 1);
			 return rxData;
			
			case SPI_BLACK_MODE:
		   HAL_SPI_TransmitReceive(spi->bsp_spi_Handle, txData, &rxData, 1,1000);
			 // ����ģʽ������ûص�����,������ɺ�ֱ������Ƭѡ����
          HAL_GPIO_WritePin(spi->CSN_24L01_GPIO_Port,spi->CSN_24L01_Pin ,GPIO_PIN_SET ); 
		  return rxData;
			
			case SPI_NULL_MODE:
			 printf("SPITransRecv:error\r\n");
			return 0; 
			
		  default:
	     while(1)
				return 0; 
			 ; //״̬��������ģʽ��ֹͣ���䣻
		
		}
	}


/* spi������ʼ�� */
