#include "TCS34725_iic.h"
#include "delay.h"

#define TCS_SDA_PIN_NUM       0
#define TCS_SDA_PIN           GPIO_Pin_0
#define TCS_SDA_GPIO          GPIOA
#define TCS_SCL_PIN           GPIO_Pin_5
#define TCS_SCL_GPIO          GPIOA

#define TCS_SDA_DIR_IN()       {TCS_SDA_GPIO->MODER&=~(3<<(TCS_SDA_PIN_NUM*2));TCS_SDA_GPIO->MODER|=0<<(TCS_SDA_PIN_NUM*2);}//����ģʽ
#define TCS_SDA_DIR_OUT()      {TCS_SDA_GPIO->MODER&=~(3<<(TCS_SDA_PIN_NUM*2));TCS_SDA_GPIO->MODER|=1<<(TCS_SDA_PIN_NUM*2);}//���ģʽ

//void TCS_SDA_DIR_OUT(void){
//		GPIO_InitTypeDef GPIO_InitStructure;
//		GPIO_InitStructure.GPIO_Pin = TCS_SDA_PIN;
//		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
//		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
//		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//50MHz
//		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
//		GPIO_Init(TCS_SDA_GPIO, &GPIO_InitStructure);
//}

//void TCS_SDA_DIR_IN(void){
//		GPIO_InitTypeDef GPIO_InitStructure;
//		GPIO_InitStructure.GPIO_Pin = TCS_SDA_PIN;
//		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ����ģʽ
//		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
//		GPIO_Init(TCS_SDA_GPIO, &GPIO_InitStructure);
//}



#define TCS_SDA_READ					GPIO_ReadInputDataBit(GPIOA, TCS_SDA_PIN)

#define TCS_SDA_SET						GPIO_SetBits(GPIOA,TCS_SDA_PIN)
#define TCS_SDA_RESET					GPIO_ResetBits(GPIOA,TCS_SDA_PIN)

#define TCS_SCL_SET           GPIO_SetBits(GPIOA,TCS_SCL_PIN)
#define TCS_SCL_RESET         GPIO_ResetBits(GPIOA,TCS_SCL_PIN)


//TCS34725 I2C��ʼ��



void TCS34725_I2C_Init(void)

{

    GPIO_InitTypeDef GPIO_InitStructure;
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //ʹ��GPIOAʱ��
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
		
//		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	
    GPIO_InitStructure.GPIO_Pin = TCS_SDA_PIN;
		GPIO_Init(GPIOA,&GPIO_InitStructure); 

    GPIO_InitStructure.GPIO_Pin = TCS_SCL_PIN;
    GPIO_Init(GPIOA,&GPIO_InitStructure); 
	
    TCS_SDA_SET;
    TCS_SCL_SET;

}



//����I2C��ʼ�ź�

void TCS34725_I2C_Start()

{

        TCS_SDA_DIR_OUT();//sda�����

        TCS_SDA_SET;

        TCS_SCL_SET;

        delay_us(4);

        TCS_SDA_RESET;//START:when CLK is high,DATA change form high to low 

        delay_us(4);

        TCS_SCL_RESET;//ǯסI2C���ߣ�׼�����ͻ�������� 

}



//����I2Cֹͣ�ź�

void TCS34725_I2C_Stop()

{

        TCS_SDA_DIR_OUT();//sda�����

        TCS_SCL_RESET;

        TCS_SDA_RESET;//STOP:when CLK is high DATA change form low to high

        delay_us(4);

        TCS_SCL_SET;

        TCS_SDA_SET;//����I2C���߽����ź�

        delay_us(4);                                                                   

}



//�ȴ�Ӧ���źŵ���

//����ֵ��1������Ӧ��ʧ��

//        0������Ӧ��ɹ�

uint8_t TCS34725_I2C_Wait_ACK()

{

        uint8_t timeOut = 0;

        TCS_SDA_DIR_IN();//SDA����Ϊ����  

        TCS_SDA_SET; delay_us(1);

        TCS_SCL_SET; delay_us(1);

        while(TCS_SDA_READ)

        {

                timeOut++;

                if(timeOut > 250)

                {

                        TCS34725_I2C_Stop();

                        return 1;

                }

        }

        TCS_SCL_RESET;//ʱ�����0

                        

        return 0;        

} 



//����ACKӦ��

void TCS34725_I2C_ACK()

{

        TCS_SCL_RESET;

        TCS_SDA_DIR_OUT();//sda�����

        TCS_SDA_RESET;

        delay_us(2);

        TCS_SCL_SET;

        delay_us(2);

        TCS_SCL_RESET;

}



//������ACKӦ��                    

void TCS34725_I2C_NACK(){
        TCS_SCL_RESET;
        TCS_SDA_DIR_OUT();//sda�����
        TCS_SDA_SET;
        delay_us(2);
        TCS_SCL_SET;
        delay_us(2);
        TCS_SCL_RESET;
}        



//I2C����һ���ֽ�                  

void TCS34725_I2C_Send_Byte(uint8_t byte)

{

        uint8_t i = 0;

        TCS_SDA_DIR_OUT();//sda�����

        TCS_SCL_RESET;//����ʱ�ӿ�ʼ���ݴ���

        for(i = 0; i < 8; i++)

        {

                if(((byte & 0x80) >> 7) == 0x01)
									TCS_SDA_SET; 
								else
									TCS_SDA_RESET;

                byte <<= 1;

                delay_us(2);

                TCS_SCL_SET;

                delay_us(2);

                TCS_SCL_RESET;

                delay_us(2);

        } 

} 



//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   

uint8_t TCS34725_I2C_Read_Byte(uint8_t ack)

{

        uint8_t i,receive = 0;

        TCS_SDA_DIR_IN();

        for(i = 0; i < 8; i++)

        {

                TCS_SCL_RESET;

                delay_us(2);

                TCS_SCL_SET;

                receive <<= 1;

                if(TCS_SDA_READ) receive++;

                delay_us(1);

        }
        if (!ack) TCS34725_I2C_NACK();//����nACK
        else TCS34725_I2C_ACK(); //����ACK 
        return receive;

}

void TCS34725_I2C_Write(uint8_t slaveAddress,uint8_t* dataBuffer, uint8_t bytesNumber, uint8_t stopBit){

        unsigned char i = 0;

        TCS34725_I2C_Start();

        TCS34725_I2C_Send_Byte((slaveAddress << 1) | 0x00);           //���ʹӻ���ַд����

        TCS34725_I2C_Wait_ACK();

        for(i = 0; i < bytesNumber; i++)

        {

                TCS34725_I2C_Send_Byte(*(dataBuffer + i));

                TCS34725_I2C_Wait_ACK();

        }

        if(stopBit == 1) TCS34725_I2C_Stop();

}

void TCS34725_I2C_Read(uint8_t slaveAddress, uint8_t* dataBuffer, uint8_t bytesNumber, uint8_t stopBit){

        unsigned char i = 0;

        TCS34725_I2C_Start();

        TCS34725_I2C_Send_Byte((slaveAddress << 1) | 0x01);           //���ʹӻ���ַ������

        TCS34725_I2C_Wait_ACK();

        for(i = 0; i < bytesNumber; i++)

        {

                if(i == bytesNumber - 1)

                {

                        *(dataBuffer + i) = TCS34725_I2C_Read_Byte(0);//��ȡ�����һ���ֽڷ���NACK

                }

                else

                {

                        *(dataBuffer + i) = TCS34725_I2C_Read_Byte(1);

                }

        }

        if(stopBit == 1) TCS34725_I2C_Stop();

}

void TCS34725_Write(unsigned char subAddr, unsigned char* dataBuffer, unsigned char bytesNumber){
    unsigned char sendBuffer[10] = {0, };
    unsigned char byte = 0;
   
    sendBuffer[0] = subAddr | TCS34725_COMMAND_BIT;
    for(byte = 1; byte <= bytesNumber; byte++)
    {
        sendBuffer[byte] = dataBuffer[byte - 1];
    }
        TCS34725_I2C_Write(TCS34725_ADDRESS, sendBuffer, bytesNumber + 1, 1);
}

void TCS34725_Read(unsigned char subAddr, unsigned char* dataBuffer, unsigned char bytesNumber){
        subAddr |= TCS34725_COMMAND_BIT;
       
        TCS34725_I2C_Write(TCS34725_ADDRESS, (unsigned char*)&subAddr, 1, 0);
        TCS34725_I2C_Read(TCS34725_ADDRESS, dataBuffer, bytesNumber, 1);
	
}

unsigned char TCS34725_Init(void){
        uint8_t id=0;
				TCS34725_I2C_Init();
        TCS34725_Read(TCS34725_ID,&id,1);
        if(id==0x44){
                TCS34725_SetIntegrationTime(TCS34725_INTEGRATIONTIME_240MS);
                TCS34725_SetGain(TCS34725_GAIN_60X);
                TCS34725_Enable();
                return 1;

        }
        return 0;
}

void TCS34725_SetIntegrationTime(uint8_t time){
        unsigned char cmd = time;
        TCS34725_Write(TCS34725_ATIME, &cmd, 1);
}


void TCS34725_SetGain(uint8_t gain){
        unsigned char cmd = gain;
        TCS34725_Write(TCS34725_CONTROL, &cmd, 1);
}


void TCS34725_Setup(void){
        TCS34725_SetIntegrationTime(TCS34725_INTEGRATIONTIME_240MS);
        TCS34725_SetGain(TCS34725_GAIN_16X);
}


void TCS34725_Enable(void){
	uint8_t cmd = TCS34725_ENABLE_PON;

        TCS34725_Write(TCS34725_ENABLE,&cmd,1);
        cmd = TCS34725_ENABLE_PON | TCS34725_ENABLE_AEN;
        TCS34725_Write(TCS34725_ENABLE,&cmd,1);
        delay_ms(3);
}

void TCS34725_Disable(void){
        uint8_t cmd = 0;

        TCS34725_Read(TCS34725_ENABLE,&cmd,1);
        cmd = cmd& ~(TCS34725_ENABLE_PON | TCS34725_ENABLE_AEN);
        TCS34725_Write(TCS34725_ENABLE,&cmd,1);

}

uint16_t TCS34725_GetChannelData(uint8_t reg){
        uint8_t tmp[2]={0,0};
        uint16_t data;

        TCS34725_Read(reg,tmp,2);
        data=(tmp[1]<<8|tmp[0]);

        return data;
}

uint8_t TCS34725_GetRawData(COLOR_RGBC *rgbc){
        u8 status = TCS34725_STATUS_AVALID;
        TCS34725_Read(TCS34725_STATUS,&status,1);
        if(status & TCS34725_STATUS_AVALID){
                rgbc->c = TCS34725_GetChannelData(TCS34725_CDATAL);
                rgbc->r = TCS34725_GetChannelData(TCS34725_RDATAL);
                rgbc->g = TCS34725_GetChannelData(TCS34725_GDATAL);
                rgbc->b = TCS34725_GetChannelData(TCS34725_BDATAL);
        }
				return 0;
}

uint16_t TCS34725_GetRGB565Data(COLOR_RGBC *rgbc){
				u16 ret=0;
        u8 status = TCS34725_STATUS_AVALID;
        TCS34725_Read(TCS34725_STATUS,&status,1);
        if(status & TCS34725_STATUS_AVALID){
                rgbc->c = TCS34725_GetChannelData(TCS34725_CDATAL);
                rgbc->r = TCS34725_GetChannelData(TCS34725_RDATAL);
                rgbc->g = TCS34725_GetChannelData(TCS34725_GDATAL);
                rgbc->b = TCS34725_GetChannelData(TCS34725_BDATAL);
								ret = ((rgbc->r)>>11)<<11 | ((rgbc->g)>>10)<<5 | ((rgbc->b)>>11)<<0 ;
					return ret;
        }
				return 0;
}


