#ifndef __HC_SR04

#define __HC_SR04

#include "sys.h"

typedef uint16_t u16;

void HC_SR04_GPIO_Init(void);
void TIM2_init(u16 arr,u16 psc);
u16 HC_SR04_GetDiatance(void);
void TIM2_IRQHandler(void);

#endif
