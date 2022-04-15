#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "tftlcd.h"
#include "spi.h"
#include "TCS34725_iic.h"
#include "VL6180X_iic.h"

// WARNING: PA2,PA3,PA9,PA10��ñ���������;

// TFTLCD���ߣ�
// SDA->PB5
// SCK->PB3
// LCD_PWR->PB13
// LCD_RST->PB14
// LCD_WR->PB15
// LCD_CS->PB1

// TCS34725���ߣ�
// SDA->PA0
// SCL->PA1

// VL6180���ߣ�
// SDA->PC6
// SCL->PC8

COLOR_RGBC rgb;
COLOR_HSL hsl;


int main(void)
{ 
	int i=0;
	uint8_t range=0;
	uint16_t RGB565=0;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(100);  //��ʼ����ʱ����
	uart_init(115200);	//��ʼ�����ڲ�����Ϊ115200
	LED_Init();					//��ʼ��LED 
 	LCD_Init();					//LCD��ʼ��  
 	
	VL6180X_Init();
	
	LCD_Clear(BLUE);
	Display_TongFei_LOGO(0, 0);

	i=TCS34725_Init();
	LCD_ShowNum(0,0,i,1,32);

	delay_ms(100);
	TCS34725_Setup();
		 
	while(1)
	{
			range = VL6180X_Read_Range();
			TCS34725_GetRawData(&rgb);
			RGB565 = TCS34725_GetRGB565Data(&rgb); 
			
			LCD_ShowNum(0,0,rgb.r,5,32);
			LCD_ShowNum(50,0,rgb.g,5,32);
			LCD_ShowNum(100,0,rgb.b,5,32);
			
			LCD_ShowString(0,50,120,16,16,"Distance: ");
			LCD_ShowNum(120,120,range,3,16);
			
			LCD_ShowString(0,120,120,16,16,"Current Color: ");
			LCD_Fill(120,120,150,150,RGB565);
			
			delay_ms(1000);
	}
		
}

