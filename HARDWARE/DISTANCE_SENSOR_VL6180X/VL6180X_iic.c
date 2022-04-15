#include "VL6180X_iic.h"
#include "delay.h"

// 注意：VL6180X读写是16位的



void VL6180X_IIC_Init(void)
{			
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);


	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	IIC_SCL=1;
	IIC_SDA=1;
}

void VL6180X_IIC_Start(void)
{
	SDA_OUT();    
	IIC_SDA=1;	  	  
	IIC_SCL=1;
	delay_us(4);
 	IIC_SDA=0;
	delay_us(4);
	IIC_SCL=0;
}	  

void VL6180X_IIC_Stop(void)
{
	SDA_OUT();
	IIC_SCL=0;
	IIC_SDA=0;
 	delay_us(4);
	IIC_SCL=1; 
	IIC_SDA=1;
	delay_us(4);							   	
}


u8 VL6180X_IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();      
	IIC_SDA=1;delay_us(1);	   
	IIC_SCL=1;delay_us(1);	 
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			VL6180X_IIC_Stop();
			return 1;
		}
	}
	IIC_SCL=0; 
	return 0;  
} 

void VL6180X_IIC_Ack(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=0;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
}
    
void VL6180X_IIC_NAck(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=1;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
}					 				     
	  

void VL6180X_IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	SDA_OUT(); 	    
    IIC_SCL=0;
    for(t=0;t<8;t++)
    {              
        IIC_SDA=(txd&0x80)>>7;
        txd<<=1; 	  
		delay_us(2); 
		IIC_SCL=1;
		delay_us(2); 
		IIC_SCL=0;	
		delay_us(2);
    }	 
} 	    

u8 VL6180X_IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();
    for(i=0;i<8;i++ )
	{
        IIC_SCL=0; 
        delay_us(2);
				IIC_SCL=1;
        receive<<=1;
        if(READ_SDA)receive++;   
		delay_us(1); 
    }					 
    if (!ack)
        VL6180X_IIC_NAck();
    else
        VL6180X_IIC_Ack();
    return receive;
}

// 下面是VL6180用的



//写	reg寄存器 data数据
u8 VL6180X_WriteByte(u16 reg,u8 data)
{
	uint8_t Index_H = (uint8_t)(reg >> 8);
	uint8_t Index_L = (uint8_t)(reg & 0xFF);
	
	VL6180X_IIC_Start();
	VL6180X_IIC_Send_Byte((VL6180X_DEFAULT_I2C_ADDR<<1)|0);
	if(VL6180X_IIC_Wait_Ack())	//等待应答
	{
		VL6180X_IIC_Stop();	
		return 1;		
	}
	VL6180X_IIC_Send_Byte(Index_H);
	VL6180X_IIC_Wait_Ack();	//等待ACK
	VL6180X_IIC_Send_Byte(Index_L);
	VL6180X_IIC_Wait_Ack();	//等待ACK
	VL6180X_IIC_Send_Byte(data);
	if(VL6180X_IIC_Wait_Ack())	//等待ACK
	{
		VL6180X_IIC_Stop();	 
		return 1;		 
	}
	VL6180X_IIC_Stop();
	return 0;	
}

//VL6180X读取8位数据
u8 VL6180X_ReadByte(u16 reg)
{
	u8 res;
	uint8_t Index_H = (uint8_t)(reg >> 8);
	uint8_t Index_L = (uint8_t)(reg & 0xff);
    VL6180X_IIC_Start(); 
	VL6180X_IIC_Send_Byte((VL6180X_DEFAULT_I2C_ADDR<<1)|0);//发送器件地址+写命令	
	VL6180X_IIC_Wait_Ack();		//等待应答 
    VL6180X_IIC_Send_Byte(Index_H);	//写寄存器地址
    VL6180X_IIC_Wait_Ack();		//等待应答
	VL6180X_IIC_Send_Byte(Index_L);	//写寄存器地址
	VL6180X_IIC_Wait_Ack();	
	
    VL6180X_IIC_Start();
	VL6180X_IIC_Send_Byte((VL6180X_DEFAULT_I2C_ADDR<<1)|1);//发送器件地址+读命令	
    VL6180X_IIC_Wait_Ack();		//等待应答 
	res=VL6180X_IIC_Read_Byte(0);//读取数据,发送nACK 
    VL6180X_IIC_Stop();			//产生一个停止条件 
	return res;
}

uint8_t VL6180X_Read_ID(void)
{
	return VL6180X_ReadByte(VL6180X_REG_IDENTIFICATION_MODEL_ID);
}

uint8_t VL6180X_Init(void)
{
	VL6180X_IIC_Init();
	
	delay_us(1);
	if(VL6180X_Read_ID() == VL6180X_DEFAULT_ID)
	{	
		VL6180X_WriteByte(0x0207, 0x01);
		VL6180X_WriteByte(0x0208, 0x01);
		VL6180X_WriteByte(0x0096, 0x00);
		VL6180X_WriteByte(0x0097, 0xfd);
		VL6180X_WriteByte(0x00e3, 0x00);
		VL6180X_WriteByte(0x00e4, 0x04);
		VL6180X_WriteByte(0x00e5, 0x02);
		VL6180X_WriteByte(0x00e6, 0x01);
		VL6180X_WriteByte(0x00e7, 0x03);
		VL6180X_WriteByte(0x00f5, 0x02);
		VL6180X_WriteByte(0x00d9, 0x05);
		VL6180X_WriteByte(0x00db, 0xce);
		VL6180X_WriteByte(0x00dc, 0x03);
		VL6180X_WriteByte(0x00dd, 0xf8);
		VL6180X_WriteByte(0x009f, 0x00);
		VL6180X_WriteByte(0x00a3, 0x3c);
		VL6180X_WriteByte(0x00b7, 0x00);
		VL6180X_WriteByte(0x00bb, 0x3c);
		VL6180X_WriteByte(0x00b2, 0x09);
		VL6180X_WriteByte(0x00ca, 0x09);
		VL6180X_WriteByte(0x0198, 0x01);
		VL6180X_WriteByte(0x01b0, 0x17);
		VL6180X_WriteByte(0x01ad, 0x00);
		VL6180X_WriteByte(0x00ff, 0x05);
		VL6180X_WriteByte(0x0100, 0x05);
		VL6180X_WriteByte(0x0199, 0x05);
		VL6180X_WriteByte(0x01a6, 0x1b);
		VL6180X_WriteByte(0x01ac, 0x3e);
		VL6180X_WriteByte(0x01a7, 0x1f);
		VL6180X_WriteByte(0x0030, 0x00);
		
		// Recommended : Public registers - See data sheet for more detail
		VL6180X_WriteByte(0x0011, 0x10);       // Enables polling for 'New Sample ready'
									// when measurement completes
		VL6180X_WriteByte(0x010a, 0x30);       // Set the averaging sample period
									// (compromise between lower noise and increased execution time)
		VL6180X_WriteByte(0x003f, 0x46);       // Sets the light and dark gain (upper nibble). Dark gain should not be changed !上半字节要写入0x4	默认增益是1.0
		VL6180X_WriteByte(0x0031, 0xFF);       // sets the # of range measurements after
									// which auto calibration of system is performed
		VL6180X_WriteByte(0x0040, 0x63);       // Set ALS integration time to 100ms
		VL6180X_WriteByte(0x002e, 0x01);       // perform a single temperature calibration of the ranging sensor

		// Optional: Public registers - See data sheet for more detail
		VL6180X_WriteByte(0x001b, 0x09);    //测量间隔	轮询模式
									// period to 100ms	每步10ms->0-10ms
		VL6180X_WriteByte(0x003e, 0x31);      //测量周期	ALS模式
									// to 500ms		
		VL6180X_WriteByte(0x0014, 0x24);       // Configures interrupt on 'New Sample
									// Ready threshold event'
		return 0;
	}
	else return 1;
}

//单位毫米
uint8_t VL6180X_Read_Range(void)
{
	uint8_t range = 0;
	//开启传输
	while(!(VL6180X_ReadByte(VL6180X_REG_RESULT_RANGE_STATUS) & 0x01));
	VL6180X_WriteByte(VL6180X_REG_SYSRANGE_START,0x01);	//单次触发模式
	//等待新样本就绪阈值事件(New Sample Ready threshold event)
	while(!(VL6180X_ReadByte(VL6180X_REG_RESULT_INTERRUPT_STATUS_GPIO) & 0x04));
	range = VL6180X_ReadByte(VL6180X_REG_RESULT_RANGE_VAL);
	//获取完数据，清楚中断位
	VL6180X_WriteByte(VL6180X_REG_SYSTEM_INTERRUPT_CLEAR,0x07);	//0111b 清除了三种中断标志
	return range;
}

float VL6180X_Read_Lux(uint8_t Gain)
{
	float lux;
	uint8_t reg;
	reg = VL6180X_ReadByte(VL6180X_REG_SYSTEM_INTERRUPT_CONFIG);
	reg &= ~0x38;		//[5:3]清0
	reg |= (0x4<<3);	//开启转换New sample ready	开启转换
	
	VL6180X_WriteByte(VL6180X_REG_SYSALS_INTEGRATION_PERIOD_HI,0);
	VL6180X_WriteByte(VL6180X_REG_SYSALS_INTEGRATION_PERIOD_LO,100);	//101ms
	if (Gain > VL6180X_ALS_GAIN_40)
	{
		Gain = VL6180X_ALS_GAIN_40;
	}
	VL6180X_WriteByte(VL6180X_REG_SYSALS_ANALOGUE_GAIN, 0x40 | Gain);
	VL6180X_WriteByte(VL6180X_REG_SYSALS_START, 0x1);	//连续模式
	// New Sample Ready threshold event 新样本就绪
	while (4 != ((VL6180X_ReadByte(VL6180X_REG_RESULT_INTERRUPT_STATUS_GPIO) >> 3) & 0x7));
	
	lux = VL6180X_Read_HalfWold(VL6180X_REG_RESULT_ALS_VAL);
	VL6180X_WriteByte(VL6180X_REG_SYSTEM_INTERRUPT_CLEAR,0x07);	//0111b 清除了三种中断标志
	//矫正增益算法
	lux *= 0.32f; // calibrated count/lux
	switch(Gain) { 
	case VL6180X_ALS_GAIN_1: 
		break;
	case VL6180X_ALS_GAIN_1_25: 
		lux /= 1.25f;
		break;
	case VL6180X_ALS_GAIN_1_67: 
		lux /= 1.76f;
		break;
	case VL6180X_ALS_GAIN_2_5: 
		lux /= 2.5f;
		break;
	case VL6180X_ALS_GAIN_5: 
		lux /= 5;
		break;
	case VL6180X_ALS_GAIN_10: 
		lux /= 10;
		break;
	case VL6180X_ALS_GAIN_20: 
		lux /= 20;
		break;
	case VL6180X_ALS_GAIN_40: 
		lux /= 20;
		break;
	}
	lux *= 100;
	lux /= 100; // integration time in ms
	return lux;
}

//VL6180X读取16位数据
u16 VL6180X_Read_HalfWold(u16 reg)
{
	u16 res;
	uint8_t Index_H = (uint8_t)(reg >> 8);
	uint8_t Index_L = (uint8_t)(reg & 0xff);
  VL6180X_IIC_Start(); 
	VL6180X_IIC_Send_Byte((VL6180X_DEFAULT_I2C_ADDR<<1)|0);//发送器件地址+写命令	
	VL6180X_IIC_Wait_Ack();		//等待应答 
  VL6180X_IIC_Send_Byte(Index_H);	//写寄存器地址
  VL6180X_IIC_Wait_Ack();		//等待应答
	VL6180X_IIC_Send_Byte(Index_L);	//写寄存器地址
	VL6180X_IIC_Wait_Ack();	
	
  VL6180X_IIC_Start();
	VL6180X_IIC_Send_Byte((VL6180X_DEFAULT_I2C_ADDR<<1)|1);//发送器件地址+读命令	
  VL6180X_IIC_Wait_Ack();		//等待应答 
	res = VL6180X_IIC_Read_Byte(1);//读取数据,发送ACK 
	res <<= 8;
	res |= VL6180X_IIC_Read_Byte(0);//读取数据,发送nACK 
  VL6180X_IIC_Stop();			//产生一个停止条件 
	return res;
}













