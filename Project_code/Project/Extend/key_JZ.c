#include "key_jz.h"

#define k5_Read() GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7)
#define k6_Read() GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8)
#define k7_Read() GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9)
#define k8_Read() GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_10)



void KEY_GPIOInit(void)
{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
		GPIO_InitTypeDef GPIOKEY_Init;
		GPIOKEY_Init.GPIO_Mode=GPIO_Mode_Out_PP;
		GPIOKEY_Init.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_5|GPIO_Pin_6;
		GPIOKEY_Init.GPIO_Speed=GPIO_Speed_50MHz;
		GPIO_Init(GPIOB,&GPIOKEY_Init);	
	
		GPIOKEY_Init.GPIO_Mode=GPIO_Mode_IPU;
		GPIOKEY_Init.GPIO_Pin=GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10;
		GPIOKEY_Init.GPIO_Speed=GPIO_Speed_50MHz;
		GPIO_Init(GPIOB,&GPIOKEY_Init);	 
}


/*行配置为输出，列配置为输入*/
uint8_t Key_return(void)
{
	
	uint8_t keynum = 0;
	//将行K1拉低，K2,K3,K4拉高 读取列的值
	GPIO_WriteBit(GPIOB, GPIO_Pin_0, Bit_RESET);
	GPIO_WriteBit(GPIOB, GPIO_Pin_1, Bit_SET);
	GPIO_WriteBit(GPIOB, GPIO_Pin_5, Bit_SET);
	GPIO_WriteBit(GPIOB, GPIO_Pin_6, Bit_SET);

	if(k5_Read() == 0){//4.1
		Delay_ms(10);//延时消抖
		while(k5_Read() == 0);
		keynum = 1;
		return keynum;
	}
	if(k6_Read() == 0){//4.2
		Delay_ms(10);//延时消抖
		while(k6_Read() == 0);
		keynum = 2;
		return keynum;
	}
	if(k7_Read() == 0){//4.3
		Delay_ms(10);//延时消抖
		while(k7_Read() == 0);
		keynum = 3;
		return keynum;
	}
	if(k8_Read() == 0){//4.4
		Delay_ms(10);//延时消抖
		while(k8_Read() == 0);
		keynum = 4;
		return keynum;
	}
	//将行K2拉低，K1,K3,K4拉高 读取列的值
	GPIO_WriteBit(GPIOB, GPIO_Pin_0, Bit_SET);
	GPIO_WriteBit(GPIOB, GPIO_Pin_1, Bit_RESET);
	GPIO_WriteBit(GPIOB, GPIO_Pin_5, Bit_SET);
	GPIO_WriteBit(GPIOB, GPIO_Pin_6, Bit_SET);

	if(k5_Read() == 0){//4.1
		Delay_ms(10);//延时消抖
		while(k5_Read() == 0);
		keynum = 5;
		return keynum;
	}
	if(k6_Read() == 0){//4.2
		Delay_ms(10);//延时消抖
		while(k6_Read() == 0);
		keynum = 6;
		return keynum;
	}
	if(k7_Read() == 0){//4.3
		Delay_ms(10);//延时消抖
		while(k7_Read() == 0);
		keynum = 7;
		return keynum;
	}
	if(k8_Read() == 0){//4.4
		Delay_ms(10);//延时消抖
		while(k8_Read() == 0);
		keynum = 8;
		return keynum;
	}

	//将行K3拉低，K2,K1,K4拉高 读取列的值
	GPIO_WriteBit(GPIOB, GPIO_Pin_0, Bit_SET);
	GPIO_WriteBit(GPIOB, GPIO_Pin_1, Bit_SET);
	GPIO_WriteBit(GPIOB, GPIO_Pin_5, Bit_RESET);
	GPIO_WriteBit(GPIOB, GPIO_Pin_6, Bit_SET);

	if(k5_Read() == 0){//3.1
		Delay_ms(10);//延时消抖
		while(k5_Read() == 0);
		keynum = 9;
		return keynum;
	}
	if(k6_Read() == 0){//3.2
		Delay_ms(10);//延时消抖
		while(k6_Read() == 0);
		keynum = 10;
		return keynum;
	}
	if(k7_Read() == 0){//3.3
		Delay_ms(10);//延时消抖
		while(k7_Read() == 0);
		keynum = 11;
		return keynum;
	}
	if(k8_Read() == 0){//3.4
		Delay_ms(10);//延时消抖
		while(k8_Read() == 0);
		keynum = 12;
		return keynum;
	}
	//将行K4拉低，K2,K3,K1拉高 读取列的值
	GPIO_WriteBit(GPIOB, GPIO_Pin_0, Bit_SET);
	GPIO_WriteBit(GPIOB, GPIO_Pin_1, Bit_SET);
	GPIO_WriteBit(GPIOB, GPIO_Pin_5, Bit_SET);
	GPIO_WriteBit(GPIOB, GPIO_Pin_6, Bit_RESET);

	if(k5_Read() == 0){//4.1
		Delay_ms(10);//延时消抖
		while(k5_Read() == 0);
		keynum = 13;
		return keynum;
	}
	if(k6_Read() == 0){//4.2
		Delay_ms(10);//延时消抖
		while(k6_Read() == 0);
		keynum = 14;
		return keynum;
	}
	if(k7_Read() == 0){//4.3
		Delay_ms(10);//延时消抖
		while(k7_Read() == 0);
		keynum = 15;
		return keynum;
	}
	if(k8_Read() == 0){//4.4
		Delay_ms(10);//延时消抖
		while(k8_Read() == 0);
		keynum = 16;
		return keynum;
	}

return 0;
}
