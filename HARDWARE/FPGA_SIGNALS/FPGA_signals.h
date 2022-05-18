#include "sys.h"

#define TOPBIT PBout(12) //顶盖舵机

#define RECONGBIT0 PAout(11) //识别部分舵机
#define RECONGBIT1 PAout(12)

#define BOTTOMBIT0 PCout(0) //底部舵机
#define BOTTOMBIT1 PCout(1)
#define BOTTOMBIT2 PBout(0)

void FPGA_GPIO_Init(void);

void FPGA_Cover_Open(void);
void FPGA_Cover_Close(void);

void FPGA_Recongize_Wait(void);
void FPGA_Recongize_Watch(void);
void FPGA_Recongize_Throw(void);

void FPGA_Bottom_TrashBin1(void);
void FPGA_Bottom_TrashBin2(void);
void FPGA_Bottom_TrashBin3(void);
void FPGA_Bottom_TrashBin4(void);
