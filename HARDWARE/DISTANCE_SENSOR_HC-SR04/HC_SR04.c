#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "HC_SR04.h"

void HC_SR04_GPIO_Init()
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  /*echo*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  /*trig*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
 
}
 
void TIM2_init(u16 arr, u16 psc)
{
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructrue;
  NVIC_InitTypeDef NVIC_InitStructure;
  
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
  TIM_DeInit(TIM2);
  
  TIM_TimeBaseStructrue.TIM_Period = arr;                   
  TIM_TimeBaseStructrue.TIM_Prescaler = psc;                
  TIM_TimeBaseStructrue.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructrue.TIM_CounterMode = TIM_CounterMode_Up;
  
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructrue);
  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
  
  NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn;             
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x00; 
  NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x01;       
  NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
  
  NVIC_Init(&NVIC_InitStructure);                           
  
  TIM_Cmd(TIM2, DISABLE); 
}

u16 TIM2_Flag;

u16 HC_SR04_GetDiatance()
{
  int distance = 0;
  u16 TIM = 0;
  TIM_Cmd(TIM2, ENABLE);           
  
  GPIO_SetBits(GPIOA, GPIO_Pin_4);  
  delay_us(30);
  GPIO_ResetBits(GPIOA, GPIO_Pin_4);
 
  while((!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1))&&TIM2_Flag==0);  
  TIM2->CNT = 0;              
  while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1)&&TIM2_Flag==0);    
  TIM_Cmd(TIM2, DISABLE);    
  
  if(TIM2_Flag==1)
    TIM2_Flag = 0;
  
  TIM = TIM_GetCounter(TIM2);
  distance = TIM*0.85;
  return distance;
}

void TIM2_IRQHandler(void)
{
  if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
  {
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    TIM2_Flag=1;
  }
}
 
