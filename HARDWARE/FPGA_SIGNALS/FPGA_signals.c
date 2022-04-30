#include "FPGA_signals.h"

void FPGA_GPIO_Init(void)
{
	  GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE); //使能GPIOC时钟
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE); //使能GPIOB时钟
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; //下拉
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
    GPIO_Init(GPIOC,&GPIO_InitStructure); 
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
		GPIO_Init(GPIOB,&GPIO_InitStructure); 
		BIT0=0;
		BIT1=0;
		BIT2=0;
}

void FPGA_Send_Rotate(uint8_t angle)
{
    if(angle == ROTATE_0) 
    {
        BIT0 = 0;
        BIT1 = 0;
        BIT2 = 0;
    }
    else if(angle == CCW_ROTATE_45) 
    {
        BIT0 = 1;
        BIT1 = 0;
        BIT2 = 0;
    } 
    else if(angle == CCW_ROTATE_90) 
    {
        BIT0 = 0;
        BIT1 = 1;
        BIT2 = 0;
    }
    else if(angle == CCW_ROTATE_135) 
    {
        BIT0 = 1;
        BIT1 = 1;
        BIT2 = 0;
    }
    else if(angle == CCW_ROTATE_180) 
    {
        BIT0 = 0;
        BIT1 = 0;
        BIT2 = 1;
    }
    else 
    {
        BIT0 = 1;
        BIT1 = 1;
        BIT2 = 1;
    }
}

