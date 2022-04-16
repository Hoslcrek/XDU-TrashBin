#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "tftlcd.h"
#include "spi.h"
#include "TCS34725_iic.h"
#include "VL6180X_iic.h"
#include "FPGA_signals.h"

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
	uint8_t range=0;
	uint16_t RGB565=0;
	uint8_t i=0;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init(100);
	uart_init(115200);
	LED_Init();
 	LCD_Init();
	TCS34725_Init();
	VL6180X_Init();
	FPGA_GPIO_Init();
	
	LCD_Clear(BLUE);
	Display_TongFei_LOGO(0, 0);


	delay_ms(100);
		 
	while(1)
	{
			range = VL6180X_Read_Range();
			TCS34725_GetRawData(&rgb);
			RGB565 = TCS34725_GetRGB565Data(&rgb); 
			
			if(i<5)
			{
				FPGA_Send_Rotate(ROTATE_0);
				delay_ms(2000);
				FPGA_Send_Rotate(CCW_ROTATE_90);
				delay_ms(2000);
				i++;
			}
		
			LCD_ShowNum(0,0,rgb.r,5,16);
			LCD_ShowNum(50,0,rgb.g,5,16);
			LCD_ShowNum(100,0,rgb.b,5,16);
			
			LCD_ShowString(0,50,120,16,16,"Distance: ");
			LCD_ShowNum(120,50,range,3,16);
			
			LCD_ShowString(0,120,120,16,16,"Current Color: ");
			LCD_Fill(120,120,150,150,RGB565);
			delay_ms(1000);
			
	}
		
}

