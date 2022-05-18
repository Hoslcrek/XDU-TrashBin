#include "sys.h"
#include "delay.h"
#include "usart1.h"
#include "usart2.h"
#include "led.h"
#include "tftlcd.h"
#include "spi.h"
#include "TCS34725_iic.h"
#include "VL6180X_iic.h"
#include "FPGA_signals.h"
#include "HC_SR04.h"
#include "HX711.h"


// WARNING: PA2,PA3,PA9,PA10����USART��ñ���������;
// ע�⣺���STM32F411RE��������PA2��PA3��û�к�MCU���ӵģ�/�ƶ�����������Ҫ��USART2��ʹ��ST-Link���ֵ�TX��RX�����https://shequ.stmicroelectronics.cn/thread-599987-1-1.html

// USART1���ڹ��ͨL610ͨ�ţ�ʹ��printf()
// USART2������λ��ͨ�ţ�ʹ��USART2_SendData(u8* buf, u16 len)


// TFTLCD���ߣ�
// SDA->PB5
// SCK->PB3
// LCD_PWR->PB13
// LCD_RST->PB14
// LCD_WR->PB15
// LCD_CS->PB1

// TCS34725���ߣ�
// SDA->PA0
// SCL->PA12

// VL6180���ߣ�
// SDA->PC6
// SCL->PC8

// HC-SR04���ߣ�
// PA1->Echo
// PA4->Trig

// HX711���ߣ�
// PC8->DT
// PC9->SCK

// FPGA���ֽ��ߣ�
// ���ǲ��֣�PB12->?
// ʶ�𲿷�: PA11->? , PA12->?
// ����ת�����֣�PB0->B2 , PC1->C1 , PC0->C3

COLOR_RGBC rgb;
COLOR_HSL hsl;


int main(void)
{ 
//	uint8_t range=0;
	uint16_t RGB565=0;
	uint16_t HC_SR04_distance=0;
	uint32_t weight=0;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init(168);
	
	uart1_init(115200);
	uart2_init(115200);
	
	TIM2_init(5000,419); 
	LED_Init();
 	LCD_Init();
	TCS34725_Init();
	VL6180X_Init();
	FPGA_GPIO_Init();
	HC_SR04_GPIO_Init();
//	HX711_Init();
	
	LCD_Clear(BLUE);
//	Display_TongFei_LOGO(0, 0);


	delay_ms(100);
		 
	while(1)
	{
//			range = VL6180X_Read_Range();
			TCS34725_GetRawData(&rgb);
			RGB565 = TCS34725_GetRGB565Data(&rgb); 
			HC_SR04_distance = HC_SR04_GetDiatance();
//			weight = HX711_GetWeight();
		
			printf("Start");
		
//			USART2_SendData("Start/",7);
		
			if(strstr((char*)CMD,"h") != 0){
				USART2_SendData("Start",6);

			}
			else if(strstr((char*)CMD,"i") != 0){
				printf("%d",weight);

			}
			else if(strstr((char*)CMD,"j") != 0){
				printf("135");

			}
			else if(strstr((char*)CMD,"k") != 0){
				printf("180");

			}
			
			
			LCD_ShowNum(0,0,rgb.r,5,16);
			LCD_ShowNum(50,0,rgb.g,5,16);
			LCD_ShowNum(100,0,rgb.b,5,16);
			
			LCD_ShowString(0,50,120,16,16,"Distance: ");
			LCD_ShowNum(120,50,HC_SR04_distance,5,16);
			
			LCD_ShowString(0,70,120,16,16,"Weight: ");
			LCD_ShowNum(120,70,weight,5,16);
			
			LCD_ShowString(0,120,120,16,16,"Current Color: ");
			LCD_Fill(120,120,150,150,RGB565);
			
			LCD_Fill(0,200,240,216,WHITE);
//			LCD_ShowString(0,200,200,16,16,(char*)CMD);
			LCD_ShowString(0,200,200,16,16,(char*)USART1_RX_BUF);
			delay_ms(1000);
			
	}
		
}

