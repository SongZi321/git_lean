#ifndef __ROCKER_H
#define __ROCKER_H

#include"gpio.h"


/***********************************
Òý½ÅÅäÖÃ
A0->x
A1->y
B0->sw

************************************/
#define Filter_width  10






typedef enum {
X_VAL=0,
Y_VAL,
Z_VAL
}direction_t;

typedef enum {
  Z_UP=0,
	Z_DOWN
}Z_State;


typedef struct{
ADC_HandleTypeDef* hadc;
uint16_t	adc_dma_storage[2*Filter_width];
	
uint16_t X_Val;
uint16_t Y_Val;
uint16_t Z_Val;

uint16_t Last_X_Val;
uint16_t Last_Y_Val;

}Dir_Value;


void prf_val(Dir_Value* dir);
void adc_init(Dir_Value* dir,ADC_HandleTypeDef adc);
extern Dir_Value dir_val;
#endif




