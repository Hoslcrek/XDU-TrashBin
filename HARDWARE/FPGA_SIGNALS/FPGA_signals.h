#include "sys.h"

#define ROTATE_0   0
#define CCW_ROTATE_45   1
#define CCW_ROTATE_90   2
#define CCW_ROTATE_135   3
#define CCW_ROTATE_180   4
#define STOP 7

#define BIT0 PAout(13)
#define BIT1 PAout(14)
#define BIT2 PAout(15)

void FPGA_GPIO_Init(void);
void FPGA_Send_Rotate(uint8_t angle);
