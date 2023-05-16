#include "stm32f10x.h"                  // Device header
#include "LED.h"

/**
  * @brief  LED引脚初始化配置
  * @param  无
  * @retval 无
  */
void LED_Init(void){
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
		GPIO_InitTypeDef GPIOLED_Init;
		GPIOLED_Init.GPIO_Mode=GPIO_Mode_Out_PP;
		GPIOLED_Init.GPIO_Pin=GPIO_Pin_0;
		GPIOLED_Init.GPIO_Speed=GPIO_Speed_50MHz;
		GPIO_Init(GPIOA,&GPIOLED_Init);
}

/**
  * @brief  LED状态控制
  * @param  i @LED的状态
  * @retval 无
  */
void LED_Data(int i){
	if(i==0){GPIO_WriteBit(GPIOA,GPIO_Pin_0,Bit_RESET);}
		else{GPIO_WriteBit(GPIOA,GPIO_Pin_0,Bit_SET);}
}

//void LED_DataBeep(int i){
//	if(i==0){GPIO_WriteBit(GPIOA,GPIO_Pin_1,Bit_RESET);}
//		else{GPIO_WriteBit(GPIOA,GPIO_Pin_1,Bit_SET);}
//}

