#include "romate_control.h"
#include "tim.h"
#include "NRF24L01.h"
#include "spi.h"


nrf24l01_t nrf24l01_1;
SPI_t spi_1;

void contrul_24G()
{
  HAL_TIM_Base_Start(&htim3);
	//adc_init(&dir_val,hadc1);
	SPIInstance spiinstance_1={
		       .CE_24L01_GPIO_Port  = NRF_CE_GPIO_Port,
					 .CE_24L01_Pin 		    =	NRF_CE_Pin,														              
					 .CSN_24L01_GPIO_Port	=	NRF_NCS_GPIO_Port,				
					 .CSN_24L01_Pin 			=	NRF_NCS_Pin,				   														                 
	         .IQR_24L01_GPIO_Port = NRF_IQR_GPIO_Port,
					 .IQR_24L01_Pin       = NRF_IQR_Pin,
	         .bsp_spi_Handle      = &hspi1};								 
  SPI_init(&spi_1 ,&spiinstance_1,SPITransmit, SPIReceive ,SPITransRecv);

					 
  Nrf24l01_init(&nrf24l01_1,&spi_1, NRF24L01GetIRQf, nRF24_SPI_Send_Byte, NRF24L01_Check, NRF_CE_Enable, NRF_CSN_Enable);						
	while(nrf24l01_1.nrf24l01_check(&nrf24l01_1))
			{
				HAL_Delay(100);
			}		
}







