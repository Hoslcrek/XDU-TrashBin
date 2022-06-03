#ifndef _HX711_H_
#define _HX711_H_
#include "sys.h"

void HX711_All_Init(void);

unsigned long HX711_TrashBin1_Read(void);
unsigned long HX711_TrashBin2_Read(void);
unsigned long HX711_TrashBin3_Read(void);
unsigned long HX711_TrashBin4_Read(void);

long HX711_TrashBin1_GetWeight(void);
long HX711_TrashBin2_GetWeight(void);
long HX711_TrashBin3_GetWeight(void);
long HX711_TrashBin4_GetWeight(void);

#endif
