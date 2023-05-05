/*
	����оƬ:STM32F103C6T6
	������:�ɼ�GY-302����ģ��������OLED������ʾ
*/
/*
GY-302��ADDRλ�ӵ͵�ƽ
	PA4  = GY-302ģ���SCL����
	PA5  = GY-302ģ���SDA����
	PA6  = DHT-11ģ���DATA����
	PA7  = OLED��ģ���SCL����
	PA8  = OLED��ģ���SDA����
	PA0  = LED0����
USART2:		PA2  = ESP-01Sģ���RXT����
					PA3  = ESP-01Sģ���TXD����
USART1: 	PA9  = ����RXT
					PA10 = ����TXD
*/


#include "main.h"

 
const char *topics[] = {"/mysmarthome/subs"};
const char topics_1[] = {"/mysmarthome/pubs"};

	char PUB_BUF[256];   //�ϴ����ݵ�buf

u8 temperature =0;  	    
u8 humidity = 0; 
float Light = 0; //���ն�
	
int main(void)
{
	unsigned short timeCount = 0; //���ͼ������
	unsigned char *dataPtr = NULL;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	
	
	Usart1_Init(9600);   //Debug����ͨѶ
	Usart2_Init(115200); //STM32-ESP8266ͨѶ
	
	LED_Init();
	OLED_Init();         
	DHT11_Init();		
	BH1750_Init();       
	OLED_Clear();
		 
	UsartPrintf(USART_DEBUG,"Hardware init OK\r\n");
	ESP8266_Init();    //��ʼ��ESP8266
	while(OneNet_DevLink())  //����OneNET
		Delay_ms(500);
	
	LED_Data(1);       //LED��˸��ʾ����ɹ�
	Delay_ms(250);
	LED_Data(0);
	
	OneNet_Subscribe(topics,1);  //topics����Ϣ�Ĵ����ַ
	
	PassWord_Init();
	
	while(1)
	{
		Rest_PassWord();		
		if(timeCount % 40 == 0)   //1000ms / 25 = 40; 1Sִ��һ��
		{
			if (!i2c_CheckDevice(BH1750_Addr))
			{
				Light = LIght_Intensity();              //��ȡ����ǿ��
			}				
			OLED_ShowNum(1,1,Light,6);//OLED��ʾ����ǿ��ֵ
			OLED_ShowString(1,7,"/Lx");//��ʾ����ǿ�ȵ�λ
			
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
		
		if(++timeCount >= 200)									//5000ms / 25 = 200; 5s�ϴ�һ������
		{
			UsartPrintf(USART_DEBUG, "OneNet_Publish\r\n");			
			//OneNet_Publish("pcTopic", "MQTT Publish Test");   //����Ϣ���з�����Ϣ
			
			
				sprintf(PUB_BUF,"{\"Hum\":%d,\"Temp\":%d,\"Light\":%.1f}",p[0],p[2],Light);
				OneNet_Publish(topics_1,PUB_BUF);
			
			
			timeCount = 0;
			ESP8266_Clear();
		}
		
		dataPtr = ESP8266_GetIPD(3);  //����Ƿ����·�ָ��_�����б䶯_�����б䶯
		if(dataPtr != NULL)
			OneNet_RevPro(dataPtr);		
		Delay_ms(10);		
	}
	
}

