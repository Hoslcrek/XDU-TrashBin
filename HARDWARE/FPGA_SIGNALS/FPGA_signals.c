#include "FPGA_signals.h"

void FPGA_GPIO_Init(void)
{
	  GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //使能GPIOA时钟
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; //上拉
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
    GPIO_Init(GPIOA,&GPIO_InitStructure); 
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

