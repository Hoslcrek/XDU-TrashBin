#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "tftlcd.h"
#include "spi.h"
#include "TCS34725_iic.h"
#include "VL6180X_iic.h"

// WARNING: PA2,PA3,PA9,PA10最好别用其他用途

// TFTLCD接线：
// SDA->PB5
// SCK->PB3
// LCD_PWR->PB13
// LCD_RST->PB14
// LCD_WR->PB15
// LCD_CS->PB1

// TCS34725接线：
// SDA->PA0
// SCL->PA1

// VL6180接线：
// SDA->PC6
// SCL->PC8

COLOR_RGBC rgb;
COLOR_HSL hsl;


int main(void)
{ 
	int i=0,j=0;
	uint8_t range=0;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(100);  //初始化延时函数
	uart_init(115200);	//初始化串口波特率为115200
	LED_Init();					//初始化LED 
 	LCD_Init();					//LCD初始化  
 	
	VL6180X_Init();
	
	LCD_Clear(BLUE);
	Display_TongFei_LOGO(0, 0);

	i=TCS34725_Init();
	LCD_ShowNum(0,0,i,1,32);

	delay_ms(100);
	TCS34725_Setup();
		 
	while(1)
	{
			range=VL6180X_Read_Range();
			TCS34725_GetRawData(&rgb);
			LCD_ShowNum(0,50,rgb.r,5,32);
			LCD_ShowNum(0,100,rgb.g,5,32);
			LCD_ShowNum(0,150,rgb.b,5,32);
			
			LCD_ShowNum(0,200,range,5,32);
			
			delay_ms(1000);
		
			if(j==20){
				j=0;
			}
			
			LCD_ShowNum(0,230,j,2,32);
			
			j++;

	}
		
}

