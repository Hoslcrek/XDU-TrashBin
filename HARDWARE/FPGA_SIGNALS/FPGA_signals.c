#include "FPGA_signals.h"


void FPGA_GPIO_Init(void)
{
	  GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE); //使能GPIOC时钟
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE); //使能GPIOB时钟
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //使能GPIOA时钟
	
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; //下拉
	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
    GPIO_Init(GPIOC,&GPIO_InitStructure); 
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_12;
		GPIO_Init(GPIOB,&GPIO_InitStructure); 
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_12;
		GPIO_Init(GPIOA,&GPIO_InitStructure); 
		
		TOPBIT=0;
	
		RECONGBIT0=0;
		RECONGBIT1=0;
		
		BOTTOMBIT0=0;
		BOTTOMBIT1=0;
		BOTTOMBIT2=0;
}

void FPGA_Cover_Open()
{
		TOPBIT=1;
}

void FPGA_Cover_Close()
{
		TOPBIT=0;
}

void FPGA_Recongize_Wait() // 初始状态
{
		RECONGBIT0=0;
		RECONGBIT1=0;
}

void FPGA_Recongize_Watch() // 识别状态
{
		RECONGBIT0=1;
		RECONGBIT1=0;
}

void FPGA_Recongize_Throw() // 扔状态
{
		RECONGBIT0=0;
		RECONGBIT1=1;
}

void FPGA_Bottom_TrashBin1() // 0度
{
		BOTTOMBIT0=0;
		BOTTOMBIT1=0;
		BOTTOMBIT2=0;
}

void FPGA_Bottom_TrashBin2() // 90度
{
		BOTTOMBIT0=1;
		BOTTOMBIT1=0;
		BOTTOMBIT2=0;
}
void FPGA_Bottom_TrashBin3() // 180度
{
		BOTTOMBIT0=0;
		BOTTOMBIT1=1;
		BOTTOMBIT2=0;
}
void FPGA_Bottom_TrashBin4() // 270度
{
		BOTTOMBIT0=0;
		BOTTOMBIT1=0;
		BOTTOMBIT2=1;
}




