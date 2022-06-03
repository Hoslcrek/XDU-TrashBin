#include "HX711.h"
#include "delay.h"

long HX711_Buffer = 0;

long TrashBin1_Weight = 0;
long TrashBin1_Weight_NoLoad = 0;

long TrashBin2_Weight = 0;
long TrashBin2_Weight_NoLoad = 0;

long TrashBin3_Weight = 0;
long TrashBin3_Weight_NoLoad = 0;

long TrashBin4_Weight = 0;
long TrashBin4_Weight_NoLoad = 0;

#define GapValue 430
#define HIGH 1
#define LOW  0

// 本来还是应该注意点不要写这么烂的代码的，算了，完成个任务罢了，反正内存也够，传感器也少

void HX711_All_Init()
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure1;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	//TrashBin1 GPIO
	
  // SCK
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	
  // DOUT
  GPIO_InitStructure1.GPIO_Pin = GPIO_Pin_8 ;
  GPIO_InitStructure1.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure1.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure1.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOC, &GPIO_InitStructure1);

	//TrashBin2 GPIO
	
  // SCK
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
  // DOUT
  GPIO_InitStructure1.GPIO_Pin = GPIO_Pin_6 ;
  GPIO_InitStructure1.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure1.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure1.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure1);

	//TrashBin3 GPIO
	
  // SCK
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
  // DOUT
  GPIO_InitStructure1.GPIO_Pin = GPIO_Pin_6 ;
  GPIO_InitStructure1.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure1.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure1.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOB, &GPIO_InitStructure1);

	//TrashBin4 GPIO
	
  // SCK
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
  // DOUT
  GPIO_InitStructure1.GPIO_Pin = GPIO_Pin_8 ;
  GPIO_InitStructure1.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure1.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure1.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOB, &GPIO_InitStructure1);

	delay_ms(10);
	
	TrashBin1_Weight_NoLoad = HX711_TrashBin1_Read();
	TrashBin2_Weight_NoLoad = HX711_TrashBin2_Read();
	TrashBin3_Weight_NoLoad = HX711_TrashBin3_Read();
	TrashBin4_Weight_NoLoad = HX711_TrashBin4_Read();
}



// 本来还是应该注意点不要写这么烂的代码的，算了，完成个任务罢了，反正内存也够，传感器也少


long HX711_TrashBin1_GetWeight()
{
	HX711_Buffer = HX711_TrashBin1_Read();
	TrashBin1_Weight = HX711_Buffer;		//获取实物的AD采样数值
	TrashBin1_Weight = (long)((float)(TrashBin1_Weight - TrashBin1_Weight_NoLoad)/GapValue); 	
	return TrashBin1_Weight;
}

long HX711_TrashBin2_GetWeight()
{
	HX711_Buffer = HX711_TrashBin2_Read();
	TrashBin2_Weight = HX711_Buffer;		//获取实物的AD采样数值
	TrashBin2_Weight = (long)((float)(TrashBin2_Weight - TrashBin2_Weight_NoLoad)/GapValue); 	
	return TrashBin2_Weight;
}
long HX711_TrashBin3_GetWeight()
{
	HX711_Buffer = HX711_TrashBin3_Read();
	TrashBin3_Weight = HX711_Buffer;		//获取实物的AD采样数值
	TrashBin3_Weight = (long)((float)(TrashBin3_Weight - TrashBin3_Weight_NoLoad)/GapValue); 	
	return TrashBin3_Weight;
}
long HX711_TrashBin4_GetWeight()
{
	HX711_Buffer = HX711_TrashBin4_Read();
	TrashBin4_Weight = HX711_Buffer;		//获取实物的AD采样数值
	TrashBin4_Weight = (long)((float)(TrashBin4_Weight - TrashBin4_Weight_NoLoad)/GapValue); 	
	return TrashBin4_Weight;
}


unsigned long HX711_TrashBin1_Read(void)	//增益128
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


unsigned long HX711_TrashBin2_Read(void)	//增益128
{
	unsigned long val = 0; 
	unsigned char i = 0; 

	while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6));

	delay_us(1);   
	//sck 拉低
	GPIO_ResetBits(GPIOA,GPIO_Pin_7);
	
	//判断是否准备好
	while(!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6));
  
	for(i=0;i<24;i++) 
	{ 
		delay_us(200);
		GPIO_SetBits(GPIOA,GPIO_Pin_7);	   
		val=val<<1; 
		delay_us(1);  
		GPIO_ResetBits(GPIOA,GPIO_Pin_7);	   
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6))  
			val++; 
		delay_us(1); 		

	} 
	GPIO_SetBits(GPIOA,GPIO_Pin_7); 
	val = val^0x800000; 
	delay_us(1); 
	GPIO_ResetBits(GPIOA,GPIO_Pin_7); 
	delay_us(1);  
	return val; 
}

unsigned long HX711_TrashBin3_Read(void)	//增益128
{
	unsigned long val = 0; 
	unsigned char i = 0; 

	while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6));

	delay_us(1);   
	//sck 拉低
	GPIO_ResetBits(GPIOB,GPIO_Pin_7);
	
	//判断是否准备好
	while(!GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6));
  
	for(i=0;i<24;i++) 
	{ 
		delay_us(200);
		GPIO_SetBits(GPIOB,GPIO_Pin_7);	   
		val=val<<1; 
		delay_us(1);  
		GPIO_ResetBits(GPIOB,GPIO_Pin_7);	   
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6))  
			val++; 
		delay_us(1); 		

	} 
	GPIO_SetBits(GPIOB,GPIO_Pin_7); 
	val = val^0x800000; 
	delay_us(1); 
	GPIO_ResetBits(GPIOB,GPIO_Pin_7); 
	delay_us(1);  
	return val; 
}

unsigned long HX711_TrashBin4_Read(void)	//增益128
{
	unsigned long val = 0; 
	unsigned char i = 0; 

	while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8));

	delay_us(1);   
	//sck 拉低
	GPIO_ResetBits(GPIOB,GPIO_Pin_9);
	
	//判断是否准备好
	while(!GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8));
  
	for(i=0;i<24;i++) 
	{ 
		delay_us(200);
		GPIO_SetBits(GPIOB,GPIO_Pin_9);	   
		val=val<<1; 
		delay_us(1);  
		GPIO_ResetBits(GPIOB,GPIO_Pin_9);	   
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8))  
			val++; 
		delay_us(1); 		

	} 
	GPIO_SetBits(GPIOB,GPIO_Pin_9); 
	val = val^0x800000; 
	delay_us(1); 
	GPIO_ResetBits(GPIOB,GPIO_Pin_9); 
	delay_us(1);  
	return val; 
}

