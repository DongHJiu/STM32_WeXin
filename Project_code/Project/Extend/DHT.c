#include "stm32f10x.h"                  // Device header
#include "Delay.h"

u8 p[5];
u8 ret;

void GPIO_DTH11(void){
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_OD; 
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_6;  
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);  
	GPIO_WriteBit(GPIOA,  GPIO_Pin_6, Bit_SET);

}

//主机发送开始信号
void Mcu_start(void)
{
  GPIO_WriteBit(GPIOA, GPIO_Pin_6, Bit_RESET);
  Delay_ms(18);                                
  GPIO_WriteBit(GPIOA, GPIO_Pin_6, Bit_SET);    
 
}

//等待DHT11响应
//1为失败，0为成功
u8 dht11_response(void)
{
	u8 i =0;
	while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6)==1)
	{
	i++;
		Delay_us(1);
		if(i>50)
		{
		return 1;
		}
	}
	
    while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6)==0);    
	while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6)==1);      
	return 0;          
  
}

//读八位测到的数据
u8 DH11_ReadByte(void)
{
	u8 i,data=0;
	for(i=0; i<8; i++)
	{
		while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6)==0);    
		Delay_us(50);
		data =data<<1;
		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6)==1)   
		{
		//data |=1;
			Delay_us(15);
			if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6)==1){data |=0x01;}
			else{data &=0xfe;};		
		}
		while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6)==1);  
	}
	return data;
}


//读40位数据
u8 DHT11_ReadData(void)
{
	Mcu_start();   
	ret = dht11_response();
	if (ret)
	{
	return 1;
	}
	  p[0]=DH11_ReadByte();  
		p[1]=DH11_ReadByte();  
		p[2]=DH11_ReadByte();     
		p[3]=DH11_ReadByte();     
		p[4]=DH11_ReadByte();   
	if(p[0]+p[1]+p[2]+p[3]!=p[4])
	{
	return 1; 
	}
	return 0;
}

void DHT11_Init(void){
	GPIO_DTH11();
	//DHT11_ReadData();
}

