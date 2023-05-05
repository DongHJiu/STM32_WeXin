#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "KEY_JZ.h"
#include "AT24C02.h"
#include "OLED.h"
#include "usart.h"

uint16_t keynum;
uint16_t num = 0;

uint16_t key_password[5] = {0,0,0,0,0};
uint16_t key_passwords[5] = {0,0,0,0,0};	

	
void OLED_main(void)
{
//	OLED_Clear();
//	OLED_ShowString(1,1,"1:Input PassWord");
	UsartPrintf(USART1,"1:Input PassWord\n");
//	OLED_ShowString(2,1,"2:Amend PassWord");
	UsartPrintf(USART1,"2:Amend PassWord\n");
}

int Key_Input(void)
{
	OLED_Clear();
	OLED_ShowString(1,1,"----");
	UsartPrintf(USART1,"Input PassWord\n");
	Delay_ms(10);
			
	u8 KD = 0;
	while(Key_return() != 16 && KD != 4)
	{
		keynum = Key_return();
		while(keynum)
		{
			if(keynum == 10){key_password[0] = 0;}
			else{key_password[0] = keynum;}
			At24c02Write(1,key_password[0]);
			OLED_ShowNum(2,1,1,key_password[0]);
			UsartPrintf(USART1,"%d\n",key_password[0]);
			KD = 1;
			keynum = 0;
		}
		while(KD == 1)
		{
			while(keynum == 0){keynum = Key_return();}		
			if(keynum == 10){key_password[1] = 0;}
			else{key_password[1] = keynum;}
			At24c02Write(2,key_password[1]);
			OLED_ShowNum(2,2,1,key_password[1]);
			UsartPrintf(USART1,"%d\n",key_password[1]);
			KD = 2;
			keynum = 0;
		}	
		while(KD == 2)
		{
			while(keynum == 0){keynum = Key_return();}	
			if(keynum == 10){key_password[2] = 0;}
			else{key_password[2] = keynum;}
			At24c02Write(3,key_password[2]);
			OLED_ShowNum(2,3,1,key_password[2]);
			UsartPrintf(USART1,"%d\n",key_password[2]);
			KD = 3;
			keynum = 0;
		}			
		while(KD == 3)
		{
			while(keynum == 0){keynum = Key_return();}	
			if(keynum == 10){key_password[3] = 0;}
			else{key_password[3] = keynum;}
			At24c02Write(4,key_password[3]);
			OLED_ShowNum(2,4,1,key_password[3]);
			UsartPrintf(USART1,"%d\n",key_password[3]);
			KD = 4;
			keynum = 0;
		}
	}
		if(At24c02Read(1) != At24c02Read(5))
		{
			return 1;
		}
		else if(At24c02Read(2) != At24c02Read(6))
		{
			return 1;
		}
		else if(At24c02Read(3) != At24c02Read(7))
		{
			return 1;
		}
		else if(At24c02Read(4) != At24c02Read(8))
		{
			return 1;
		}			
		else{return 0;}
}

int Key_Amend(void)
{
	OLED_Clear();
//	OLED_ShowString(1,1,"----");
//	UsartPrintf(USART1,"----\n");
	if(Key_Input())
	{
		OLED_ShowString(3,1,"!!!!");
		UsartPrintf(USART1,"flash\n");
		return 1;
	}
	else{OLED_ShowString(3,1,"----");UsartPrintf(USART1,"Input new PassWord\n");}
	
	
	u8 KD = 0;
	while(Key_return() != 16 && KD != 4)
	{
		keynum = Key_return();
		while(keynum)
		{
			if(keynum == 10){key_passwords[0] = 0;}
			else{key_passwords[0] = keynum;}
			At24c02Write(5,key_passwords[0]);
			OLED_ShowNum(2,1,1,key_passwords[0]);
			UsartPrintf(USART1,"%d\n",key_passwords[0]);
			KD = 1;
			keynum = 0;
		}
		while(KD == 1)
		{
			while(keynum == 0){keynum = Key_return();}		
			if(keynum == 10){key_passwords[1] = 0;}
			else{key_passwords[1] = keynum;}
			At24c02Write(6,key_passwords[1]);
			OLED_ShowNum(2,2,1,key_passwords[1]);
			UsartPrintf(USART1,"%d\n",key_passwords[1]);
			KD = 2;
			keynum = 0;
		}		
		while(KD == 2)
		{
			while(keynum == 0){keynum = Key_return();}		
			if(keynum == 10){key_passwords[2] = 0;}
			else{key_passwords[2] = keynum;}
			At24c02Write(7,key_passwords[2]);
			OLED_ShowNum(2,2,1,key_passwords[2]);
			UsartPrintf(USART1,"%d\n",key_passwords[2]);
			KD = 3;
			keynum = 0;
		}
		while(KD == 3)
		{
			while(keynum == 0){keynum = Key_return();}	
			if(keynum == 10){key_passwords[3] = 0;}
			else{key_passwords[3] = keynum;}
			At24c02Write(8,key_passwords[3]);
			OLED_ShowNum(2,4,1,key_passwords[3]);
			UsartPrintf(USART1,"%d\n",key_passwords[3]);
			KD = 4;
			keynum = 0;
		}		
	}		
	return 0;
}

void PassWord_Init(void)
{	

//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);	
	
	KEY_GPIOInit();
	AT24C02_Init();
	//uint16_t flg = 0;

	for(int i = 0;i > 3;i++)
	{
		key_password[i] = At24c02Read(i+1);
	}
	for(int i = 0;i > 3;i++)
	{
		key_passwords[i] = At24c02Read(i+5);
	}		

	OLED_main();
}

void Rest_PassWord(void)
{
	uint16_t keynum = Key_return();
	if(keynum == 1)
	{
		if(Key_Input())
		{
			OLED_ShowString(3,1,"!!!!");
			UsartPrintf(USART1,"flash\n");
		}
		else
		{
			OLED_ShowString(3,1,"----");
			UsartPrintf(USART1,"ture\n");
		}
	}
	if(keynum == 2)
	{
		if(Key_Amend() == 1)
		{
			OLED_ShowString(3,1,"!!!!");
			UsartPrintf(USART1,"flash\n");				
		}
		else
		{
			OLED_ShowString(3,1,"----");
			UsartPrintf(USART1,"ture\n");
		}				
	}	
}

