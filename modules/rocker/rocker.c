#include "rocker.h"
#include "adc.h"
#include "stdio.h"

/***********    自定义结构体   **********/
Dir_Value dir_val;


/************声明函数********************/
uint16_t firstOrderFilter(uint16_t  newValue, uint16_t oldValue, float a);
void prf_val(Dir_Value* dir);

void adc_init(Dir_Value* dir,ADC_HandleTypeDef adc)
{
	dir->hadc = &adc;
	dir->X_Val=2010;
  dir->Y_Val=2010;
  dir->Last_X_Val=2010;
  dir->Last_Y_Val=2010;	
  dir->Z_Val=1;
	
 for(int i=0;i<=2*Filter_width;i++)
	{
  dir->adc_dma_storage[i]=0;
	}
	
HAL_ADC_Start_DMA(dir->hadc,(uint32_t*)dir->adc_dma_storage,2);

}


void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{	
   dir_val.X_Val=dir_val.adc_dma_storage[0];
   dir_val.Y_Val=dir_val.adc_dma_storage[1];
	
	dir_val.X_Val=firstOrderFilter(dir_val.X_Val, dir_val.Last_X_Val, 0.3);
	dir_val.Y_Val=firstOrderFilter(dir_val.Y_Val, dir_val.Last_Y_Val, 0.3);
	dir_val.Last_X_Val=dir_val.X_Val;
	dir_val.Last_Y_Val=dir_val.Y_Val;
	
	
	prf_val(&dir_val);
	if(HAL_GPIO_ReadPin(Rocker_Z_GPIO_Port, Rocker_Z_Pin) == GPIO_PIN_RESET)
    dir_val.Z_Val = 0;    
  else if(HAL_GPIO_ReadPin(Rocker_Z_GPIO_Port, Rocker_Z_Pin) == GPIO_PIN_SET)
	{ 
	  dir_val.Z_Val = 1;
	}
}



void prf_val(Dir_Value* dir)
{

//printf("X_Val=%u\r\n",dir_val.X_Val/100);
//printf("Y_Val=%u\r\n",dir_val.Y_Val);
//printf("Z_Val=%u\r\n",dir_val.Z_Val);	
}

uint16_t firstOrderFilter(uint16_t  newValue, uint16_t oldValue, float a)
{
        return a * newValue + (1-a) * oldValue;
}

