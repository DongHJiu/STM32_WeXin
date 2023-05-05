/*
	主控芯片:STM32F103C6T6
	程序功能:采集GY-302光照模块数据在OLED屏上显示
*/
/*
GY-302的ADDR位接低电平
	PA4  = GY-302模块的SCL引脚
	PA5  = GY-302模块的SDA引脚
	PA6  = DHT-11模块的DATA引脚
	PA7  = OLED屏模块的SCL引脚
	PA8  = OLED屏模块的SDA引脚
	PA0  = LED0引脚
USART2:		PA2  = ESP-01S模块的RXT引脚
					PA3  = ESP-01S模块的TXD引脚
USART1: 	PA9  = 串口RXT
					PA10 = 串口TXD
*/


#include "main.h"

 
const char *topics[] = {"/mysmarthome/subs"};
const char topics_1[] = {"/mysmarthome/pubs"};

	char PUB_BUF[256];   //上传数据的buf

u8 temperature =0;  	    
u8 humidity = 0; 
float Light = 0; //光照度
	
int main(void)
{
	unsigned short timeCount = 0; //发送间隔变量
	unsigned char *dataPtr = NULL;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	
	
	Usart1_Init(9600);   //Debug串口通讯
	Usart2_Init(115200); //STM32-ESP8266通讯
	
	LED_Init();
	OLED_Init();         
	DHT11_Init();		
	BH1750_Init();       
	OLED_Clear();
		 
	UsartPrintf(USART_DEBUG,"Hardware init OK\r\n");
	ESP8266_Init();    //初始化ESP8266
	while(OneNet_DevLink())  //接入OneNET
		Delay_ms(500);
	
	LED_Data(1);       //LED闪烁提示接入成功
	Delay_ms(250);
	LED_Data(0);
	
	OneNet_Subscribe(topics,1);  //topics是消息的传输地址
	
	PassWord_Init();
	
	while(1)
	{
		Rest_PassWord();		
		if(timeCount % 40 == 0)   //1000ms / 25 = 40; 1S执行一次
		{
			if (!i2c_CheckDevice(BH1750_Addr))
			{
				Light = LIght_Intensity();              //读取光照强度
			}				
			OLED_ShowNum(1,1,Light,6);//OLED显示光照强度值
			OLED_ShowString(1,7,"/Lx");//显示光照强度单位
			
			UsartPrintf(USART_DEBUG,"gd = %.2f\n",Light);
			
			if(ret==0)
			{	
				DHT11_ReadData();
				OLED_ShowNum(2,1,p[0],3);
				OLED_ShowString(2,4,"%rh");
				
				UsartPrintf(USART_DEBUG,"sd = %d \n",p[0]);
				
				OLED_ShowNum(3,1,p[2],3);
				OLED_ShowString(3,4,"%C");
				
				UsartPrintf(USART_DEBUG,"wd = %d\n",p[2]);
			}				
		}
		
		if(++timeCount >= 200)									//5000ms / 25 = 200; 5s上传一次数据
		{
			UsartPrintf(USART_DEBUG, "OneNet_Publish\r\n");			
			//OneNet_Publish("pcTopic", "MQTT Publish Test");   //向消息队列发送信息
			
			
				sprintf(PUB_BUF,"{\"Hum\":%d,\"Temp\":%d,\"Light\":%.1f}",p[0],p[2],Light);
				OneNet_Publish(topics_1,PUB_BUF);
			
			
			timeCount = 0;
			ESP8266_Clear();
		}
		
		dataPtr = ESP8266_GetIPD(3);  //检测是否有下发指令_程序有变动_函数有变动
		if(dataPtr != NULL)
			OneNet_RevPro(dataPtr);		
		Delay_ms(10);		
	}
	
}

