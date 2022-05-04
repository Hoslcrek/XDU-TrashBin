#ifndef _HX711_H_
#define _HX711_H_
#include "sys.h"

void HX711_Init(void);
unsigned long HX711_Read(void);
long HX711_GetWeight(void);

#endif
