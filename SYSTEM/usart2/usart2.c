#include "sys.h"
#include "usart2.h"	
#include <string.h>


//���ʹ��ucos,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos ʹ��	  
#endif

 
 // PA2 TX
 // PA3 RX
 
 void USART2_SendData(u8 *buf, u16 len){
		u16 t;
		for(t=0;t<len;t++){
				while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
				USART_SendData(USART2,buf[t]);
		}
		while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
 }
 

 
#if EN_USART2_RX   //���ʹ���˽���
//����2�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
 
u8 USART2_RX_BUF[USART2_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
u8 CMD[USART2_REC_LEN];  
 
u16 USART2_RX_STA=0;       //����״̬���	

//��ʼ��IO ����1 
//bound:������
void uart2_init(u32 bound){
   //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //ʹ��GPIOAʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//ʹ��USART2ʱ��
 
	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2); //GPIOA2����ΪUSART2
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2); //GPIOA3����ΪUSART2
	
	//USART2�˿�����
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3; //GPIOA2��GPIOA3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOA,&GPIO_InitStructure); //��ʼ��PA2��PA3

  //USART2��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
  USART_Init(USART2, &USART_InitStructure); //��ʼ������2
	
  USART_Cmd(USART2, ENABLE);  //ʹ�ܴ���2 
	
	//USART_ClearFlag(USART1, USART_FLAG_TC);
	
#if EN_USART2_RX	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//��������ж�

	//Usart1 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;//����2�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��NVIC�Ĵ���

#endif
	
}


void USART2_IRQHandler(void)                	//����2�жϷ������
{
	u8 Res;
#if SYSTEM_SUPPORT_OS 		//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //�涨��'/'��β������\r\n
	{
		Res = USART_ReceiveData(USART2);//(USART2->DR);	//��ȡ���յ�������

		if(Res == '/'){
			strcpy((char*)CMD,(char*)USART2_RX_BUF);
			USART2_RX_STA = 0;
			memset(USART2_RX_BUF,0,sizeof(USART2_RX_BUF[0])*USART2_REC_LEN);
		}
		else{
			if(Res != 0x0a && Res != 0x0d){
				if(USART2_RX_STA<USART2_REC_LEN){
					USART2_RX_BUF[USART2_RX_STA]=Res;
					USART2_RX_STA++;
				}
				else{
					USART2_RX_STA=0;
				}
			}
		}	
	}

#if SYSTEM_SUPPORT_OS 	//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
	OSIntExit();  											 
#endif
} 
#endif	




