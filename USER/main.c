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


// WARNING: PA2,PA3,PA9,PA10除了USART最好别用其他用途
// 注意：这个STM32F411RE开发板上PA2和PA3是没有和MCU连接的（/黄豆人流汗），要用USART2请使用ST-Link部分的TX和RX，详见https://shequ.stmicroelectronics.cn/thread-599987-1-1.html

// USART1用于广和通L610通信，使用printf()
// USART2用于上位机通信，使用USART2_SendData(u8* buf, u16 len)


// TFTLCD接线：
// SDA->PB5
// SCK->PB3
// LCD_PWR->PB13
// LCD_RST->PB14
// LCD_WR->PB15
// LCD_CS->PB1

// TCS34725接线：
// SDA->PA0
// SCL->PA5

// VL6180接线：
// SDA->PC6
// SCL->PC5

// HC-SR04接线：
// Echo->PA1
// Trig->PA4

// 四个HX711接线：
// DT->PC8 SCK->PC9
// DT->PA6 SCK->PA7
// DT->PB6 SCK->PB7
// DT->PB8 SCK->PB9

// FPGA部分接线：
// 顶盖部分：PB12->?
// 识别部分: PA11->? , PA12->?
// 底座转动部分：PB0->B2 , PC1->C1 , PC0->C3


// 黑白灰橙 -> VCC SCK DT GND
// 黄绿蓝紫 -> VCC SCK DT GND
// 黑白灰紫 -> VCC SCK DT GND
// 绿红黄黄   -> VCC SCK DT GND

char ch='\"';
char ch1='\\';
char ay='\"';
char fg='\\';
	
void L610_Init(void){
	printf("AT+MIPCALL=0\r\n");
	delay_ms(2000);
	printf("AT+MIPCALL=1\r\n");
	delay_ms(2000);
	printf("AT+MIPCALL=1\r\n");
	delay_ms(1000);
	printf("AT+TCDEVINFOSET=1,%cCBHYTW6QGV%c,%c114514%c,%crt+opUPrqvO4XHby7ANeDg==%c\r\n",ch,ch,ch,ch,ch,ch);
	delay_ms(1000);
	printf("AT+TCMQTTCONN=1,20000,240,1,1\r\n");
	delay_ms(1000);
	printf("AT+TCMQTTSUB=%c$thing/down/property/CBHYTW6QGV/114514%c,1\r\n",ch,ch);
	delay_ms(1000);
	// printf("AT+TCMQTTPUB=%c$thing/up/property/CBHYTW6QGV/114514%c,1,%c{%c%cmethod%c%c:%c%creport%c%c,%c%cclientToken%c%c:%c%c123%c%c,%c%cparams%c%c:{%c%cpower_switch%c%c:0,%c%cCO2%c%c:0,%c%cTVOC%c%c:0}}%c\r\n",ay,ay,ay,fg,ay,fg,ay,fg,ay,fg,ay,fg,ay,fg,ay,fg,ay,fg,ay,fg,ay,fg,ay,fg,ay,fg,ay,fg,ay,fg,ay,fg,ay,fg,ay,ay);
}

COLOR_RGBC rgb;
COLOR_HSL hsl;


int main(void){
		// 变量声明
		uint8_t  state=0;
		uint8_t  distance_flag=0;
		uint8_t  VL6180X_distance=0;
		uint16_t RGB565=0;
		uint8_t  count=0;
//	uint16_t HC_SR04_distance=0;
		uint32_t TrashBin1_weight=0;
		uint32_t TrashBin2_weight=0;
		uint32_t TrashBin3_weight=0;
		uint32_t TrashBin4_weight=0;
		char updata[100];
		char L610_ReceivedData[USART1_REC_LEN];
		char* Data_position;
	
		// 各类初始化
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
		delay_init(168);
		uart1_init(115200);
		uart2_init(115200);
		TIM2_init(5000,419); 
//	LED_Init();
		LCD_Init();
		TCS34725_Init();
		VL6180X_Init();
		FPGA_GPIO_Init();
//	HC_SR04_GPIO_Init();
		HX711_All_Init();
		LCD_Clear(BLUE);
		Display_TongFei_LOGO(0, 0);
		L610_Init();
			 
		while(1)
		{
				//获取数据
				VL6180X_distance = VL6180X_Read_Range();
//			HC_SR04_distance = HC_SR04_GetDiatance();
				TrashBin1_weight = HX711_TrashBin1_GetWeight();
				TrashBin2_weight = HX711_TrashBin2_GetWeight();
				TrashBin3_weight = HX711_TrashBin3_GetWeight();
				TrashBin4_weight = HX711_TrashBin4_GetWeight();				
	
				// state = 0; 静止状态
				// state = 1; 默认工作状态
				// state = 2; 测试状态
			
			  // 上位机发送数据使设备进入工作状态，距离小于20cm，桶盖自动打开，识别部分开始循环工作，识别到后仍入对应垃圾，检测压力并返回数据
			  // 上位机发送数据使设备进入测试状态，所有部分不工作，根据上位机指令作出反应
			
//				USART2_SendD0eata("Start/",7);
				
				if(CMD[0]=='m'){
						state = 1;
				}
				else if(CMD[0]=='n'){
						state = 2;
				}
				
				if(state == 1){ 
					
						printf("AT+TCMQTTPUB=%c$thing/up/property/CBHYTW6QGV/114514%c,1,%c{%c%cmethod%c%c:%c%creport%c%c,%c%cclientToken%c%c:%c%c123%c%c,%c%cparams%c%c:{%c%cpower_switch%c%c:%d,%c%cTrashBin1%c%c:%u,%c%cTrashBin2%c%c:%u,%c%cTrashBin3%c%c:%u,%c%cTrashBin4%c%c:%u}}%c\r\n",ay,ay,ay,fg,ay,fg,ay,fg,ay,fg,ay,fg,ay,fg,ay,fg,ay,fg,ay,fg,ay,fg,ay,fg,ay,fg,ay,distance_flag,fg,ay,fg,ay,TrashBin1_weight,fg,ay,fg,ay,TrashBin2_weight,fg,ay,fg,ay,TrashBin3_weight,fg,ay,fg,ay,TrashBin4_weight,ay);
					
						if(strstr("h",(char*)CMD) != 0){
								sprintf(updata,"%dg",TrashBin1_weight);
								USART2_SendData((u8*)updata,strlen(updata));
								USART2_SendData((u8*)updata,strlen(updata));

						}
						else if(strstr("i",(char*)CMD) != 0){
								sprintf(updata,"%dg",TrashBin2_weight);
								USART2_SendData((u8*)updata,strlen(updata));		
								USART2_SendData((u8*)updata,strlen(updata));		
						
						}
						else if(strstr("j",(char*)CMD) != 0){
								sprintf(updata,"%dg",TrashBin3_weight);
								USART2_SendData((u8*)updata,strlen(updata));		
								USART2_SendData((u8*)updata,strlen(updata));		

						}
						else if(strstr("k",(char*)CMD) != 0){
								sprintf(updata,"%dg",TrashBin4_weight);
								USART2_SendData((u8*)updata,strlen(updata));			
								USART2_SendData((u8*)updata,strlen(updata));										

						}
						
						if(VL6180X_distance <= 100){
								FPGA_Cover_Open();
								distance_flag = 1;
								
								FPGA_Recongize_Wait();
								delay_ms(1000);
								FPGA_Recongize_Watch();
								delay_ms(800);
								RGB565 = TCS34725_GetRGB565Data(&rgb);
								delay_ms(500);
								
								if(rgb.r-rgb.b>7000 && rgb.g-rgb.b>7000){ // yellow
										FPGA_Bottom_TrashBin4();
										delay_ms(1500);
										FPGA_Recongize_Throw();
										delay_ms(1000);
								}
								else if(rgb.g-rgb.r>7000 && rgb.g-rgb.b>7000){ //green
										FPGA_Bottom_TrashBin2();
										delay_ms(1500);
										FPGA_Recongize_Throw();
										delay_ms(1000);
								}
								else if(rgb.b-rgb.r>7000 && rgb.b-rgb.g>7000){ //blue
										FPGA_Bottom_TrashBin3();
										delay_ms(1500);
										FPGA_Recongize_Throw();
										delay_ms(1000);
								}
								else if(rgb.r-rgb.g>7000 && rgb.r-rgb.b>7000){ //red
										FPGA_Bottom_TrashBin1();
										delay_ms(1500);
										FPGA_Recongize_Throw();
										delay_ms(1000);
								}
						}
						else {
								distance_flag = 0;
								FPGA_Cover_Close();
								RGB565 = TCS34725_GetRGB565Data(&rgb);
								delay_ms(1000);
						}
								
				}
				else if(state == 2){
						RGB565 = TCS34725_GetRGB565Data(&rgb);
						if(strstr("a",(char*)CMD) != 0){
								sprintf(updata,"%dg",TrashBin1_weight);
								USART2_SendData((u8*)updata,strlen(updata));
						}
						else if(strstr("b",(char*)CMD) != 0){
								sprintf(updata,"%dmm",VL6180X_distance);
								USART2_SendData((u8*)updata,strlen(updata));								
						}
						else if(strstr("c",(char*)CMD) != 0){
								sprintf(updata,"r:%d g:%d b:%d",rgb.r,rgb.g,rgb.b);
								USART2_SendData((u8*)updata,strlen(updata));								
						}
						else if(strstr("e",(char*)CMD) != 0){
								FPGA_Cover_Open();
								delay_ms(800);
								FPGA_Cover_Close();
								strcpy((char*)CMD,"n");
						}
						else if(strstr("f",(char*)CMD) != 0){
								FPGA_Recongize_Wait();
								delay_ms(800);
								FPGA_Recongize_Watch();
								delay_ms(800);
								FPGA_Recongize_Throw();
								strcpy((char*)CMD,"n");
						}
						else if(strstr("g",(char*)CMD) != 0){
								FPGA_Bottom_TrashBin1();
								delay_ms(1000);
								FPGA_Bottom_TrashBin2();
								delay_ms(1000);
								FPGA_Bottom_TrashBin3();
								delay_ms(1000);
								FPGA_Bottom_TrashBin4();
								strcpy((char*)CMD,"n");
						}
						delay_ms(700);
				}
				else{
						if(count < 10){
								count ++;
						}
						else{
								count = 0;
//								printf("AT+TCMQTTPUB=%c$thing/up/property/CBHYTW6QGV/114514%c,1,%c{%c%cmethod%c%c:%c%creport%c%c,%c%cclientToken%c%c:%c%c123%c%c,%c%cparams%c%c:{%c%cpower_switch%c%c:%d,%c%cTrashBin1%c%c:%u,%c%cTrashBin2%c%c:%u}}%c\r\n",ay,ay,ay,fg,ay,fg,ay,fg,ay,fg,ay,fg,ay,fg,ay,fg,ay,fg,ay,fg,ay,fg,ay,fg,ay,fg,ay,distance_flag,fg,ay,fg,ay,TrashBin1_weight,fg,ay,fg,ay,TrashBin2_weight,ay);
								printf("AT+TCMQTTPUB=%c$thing/up/property/CBHYTW6QGV/114514%c,1,%c{%c%cmethod%c%c:%c%creport%c%c,%c%cclientToken%c%c:%c%c123%c%c,%c%cparams%c%c:{%c%cpower_switch%c%c:%d,%c%cTrashBin1%c%c:%u,%c%cTrashBin2%c%c:%u,%c%cTrashBin3%c%c:%u,%c%cTrashBin4%c%c:%u}}%c\r\n",ay,ay,ay,fg,ay,fg,ay,fg,ay,fg,ay,fg,ay,fg,ay,fg,ay,fg,ay,fg,ay,fg,ay,fg,ay,fg,ay,distance_flag,fg,ay,fg,ay,TrashBin1_weight,fg,ay,fg,ay,TrashBin2_weight,fg,ay,fg,ay,TrashBin3_weight,fg,ay,fg,ay,TrashBin4_weight,ay);

						}
						RGB565 = TCS34725_GetRGB565Data(&rgb);
						delay_ms(700);
				}
				
				
				strncpy(L610_ReceivedData,(char*)USART1_RX_BUF,USART1_REC_LEN);
				Data_position = strstr(L610_ReceivedData,"power_switch");
				if(Data_position == NULL){
						memset(L610_ReceivedData,0,sizeof(L610_ReceivedData[0])*USART1_REC_LEN);
				}
				else if(*(Data_position+14) == '1'){
						FPGA_Cover_Open();
				}
				else if(*(Data_position+14) == '0'){
						FPGA_Cover_Close();
				}
								
				//显示部分
				LCD_ShowString(0,0,90,16,16,"ColorData:");
				LCD_ShowNum(100,0,rgb.r,5,16);
				LCD_ShowNum(150,0,rgb.g,5,16);
				LCD_ShowNum(200,0,rgb.b,5,16);
				
				LCD_ShowString(0,20,110,16,16,"Current Color: ");
				LCD_Fill(120,20,140,40,RGB565);
				
				
				LCD_ShowString(0,50,120,16,16,"Distance: ");
				LCD_ShowNum(90,50,VL6180X_distance,5,16);
				
				LCD_ShowString(0,70,120,16,16,"Weight: ");
				LCD_ShowNum(70,70,TrashBin1_weight,4,16);
				LCD_ShowNum(110,70,TrashBin2_weight,4,16);
				LCD_ShowNum(70,90,TrashBin3_weight,4,16);
				LCD_ShowNum(110,90,TrashBin4_weight,4,16);
				
				LCD_ShowString(0,160,200,16,16,Data_position);
				LCD_ShowString(0,200,200,16,16,(char*)CMD);
				
				
				
//				FPGA_Recongize_Wait();
//				FPGA_Bottom_TrashBin1();	
//				FPGA_Cover_Open();
//				delay_ms(3000);
//				
//				FPGA_Recongize_Watch();
//				FPGA_Bottom_TrashBin2();
//				FPGA_Cover_Close();
//				delay_ms(3000);
//				
//				FPGA_Bottom_TrashBin3();
//				FPGA_Recongize_Throw();
//				delay_ms(3000);
//				
//				FPGA_Bottom_TrashBin4();

		}
}

