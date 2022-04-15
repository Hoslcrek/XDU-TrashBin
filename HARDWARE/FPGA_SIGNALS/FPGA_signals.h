#include "sys.h"

#define CCW_ROTATE_90   0
#define CCW_ROTATE_45   2
#define CW_ROTATE_0   3
#define CW_ROTATE_45   4
#define CW_ROTATE_90   5

#define BIT0 PCout(13)
#define BIT1 PCout(14)
#define BIT2 PCout(15)

void FPGA_GPIO_Init(void);
void FPGA_Send_Rotate(uint8_t angle);
