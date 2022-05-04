#include "HX711.h"
#include "delay.h"

long HX711_Buffer = 0;
long Weight = 0;
long Weight_NoLoad = 0;

#define GapValue 430
#define HIGH 1
#define LOW  0
#define HX711_GROUP GPIOC

//****************************************************
//初始化HX711
//****************************************************
void HX711_Init()
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure1;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

  //SCK
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_Init(GPIOC, &GPIO_InitStructure);


  //DOUT
  GPIO_InitStructure1.GPIO_Pin = GPIO_Pin_8 ;
  GPIO_InitStructure1.GPIO_Mode = GPIO_Mode_IN;
  //GPIO_InitStructure1.GPIO_OType = GPIO_OType_OD; 
  GPIO_InitStructure1.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure1.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOC, &GPIO_InitStructure1);

	delay_ms(10);
	Weight_NoLoad = HX711_Read();
}

//****************************************************
//称重
//****************************************************
long HX711_GetWeight()
{
	HX711_Buffer = HX711_Read();
	Weight = HX711_Buffer;		//获取实物的AD采样数值
	Weight = (long)((float)(Weight - Weight_NoLoad)/GapValue); 	
	return Weight;
}

//****************************************************
//读取HX711
//****************************************************
unsigned long HX711_Read(void)	//增益128
{
	unsigned long val = 0; 
	unsigned char i = 0; 

	while(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_8));

	delay_us(1);   
	//sck 拉低
	GPIO_ResetBits(GPIOC,GPIO_Pin_9);
	
	//判断是否准备好
	while(!GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_8));
  
	for(i=0;i<24;i++) 
	{ 
		delay_us(200);
		GPIO_SetBits(GPIOC,GPIO_Pin_9);	   
		val=val<<1; 
		delay_us(1);  
		GPIO_ResetBits(GPIOC,GPIO_Pin_9);	   
		if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_8))  
			val++; 
		delay_us(1); 		

	} 
	GPIO_SetBits(GPIOC,GPIO_Pin_9); 
	val = val^0x800000; 
	delay_us(1); 
	GPIO_ResetBits(GPIOC,GPIO_Pin_9); 
	delay_us(1);  
	return val; 
}
