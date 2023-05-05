#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "AT24C02.h"


//基于I2C通讯的芯片要去看数据手册
/*引脚定义(基于STM32)*/
#define I2C_W_SCL(x)		GPIO_WriteBit(GPIOC, GPIO_Pin_13, (BitAction)(x))
#define I2C_W_SDA(x)		GPIO_WriteBit(GPIOC, GPIO_Pin_14, (BitAction)(x))

/*引脚初始化*/
void AT24C02_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructurert;
 	GPIO_InitStructurert.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructurert.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructurert.GPIO_Pin = GPIO_Pin_13;
 	GPIO_Init(GPIOC, &GPIO_InitStructurert);
	GPIO_InitStructurert.GPIO_Pin = GPIO_Pin_14;
 	GPIO_Init(GPIOC, &GPIO_InitStructurert);
	
	
	I2C_W_SCL(1);
	I2C_W_SDA(1);
}

// 起始信号：在I2C_W_SCL时钟信号在高电平期间I2C_W_SDA信号产生一个下降沿
void First_I2C_Start()
{
	I2C_W_SDA(1);
	Delay_us(10);
	I2C_W_SCL(1);
	Delay_us(10);    //建立时间是I2C_W_SDA保持时间>4.7us
	I2C_W_SDA(0);
	Delay_us(10);    //保持时间是>4us
	I2C_W_SCL(0);			
	Delay_us(10);		
}

//终止信号：在I2C_W_SCL时钟信号高电平期间I2C_W_SDA信号产生一个上升沿
void First_I2C_Stop()
{
	I2C_W_SDA(0);
	Delay_us(10);
	I2C_W_SCL(1);
	Delay_us(10);    //建立时间大于4.7us
	I2C_W_SDA(1);
	Delay_ms(10);		
}

//通过I2C发送一个字节。在I2C_W_SCL时钟信号高电平期间， 保持发送信号I2C_W_SDA保持稳定
unsigned char First_I2C_SendByte(unsigned char dat, unsigned char ack)
{
	unsigned char a = 0,b = 0;//最大255，一个机器周期为1us，最大延时255us。	
	int idata;	
	for(a=0; a<8; a++)//要发送8位，从最高位开始
	{
		idata=dat >> 7;	 //起始信号之后I2C_W_SCL=0，所以可以直接改变I2C_W_SDA信号
		I2C_W_SDA(idata);
		dat = dat << 1;
		Delay_us(10);
		I2C_W_SCL(1);
		Delay_us(10);    //建立时间>4.7us
		I2C_W_SCL(0);
		Delay_us(10);//时间大于4us		
	}
	I2C_W_SDA(1);
	Delay_us(10);
	I2C_W_SCL(1);
	while(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_14)==1 && (ack == 1))//等待应答，也就是等待从设备把I2C_W_SDA拉低
	{
		b++;
		if(b > 200)	 //如果超过200us没有应答发送失败，或者为非应答，表示接收结束
		{
			I2C_W_SCL(0);
			Delay_us(10);
			return 0;
		}
	}
	I2C_W_SCL(0);
	Delay_us(10);
 	return 1;		
}

// 使用I2c读取一个字节
unsigned char First_I2C_ReadByte()
{
	unsigned char a = 0,dat = 0;
	I2C_W_SDA(1);			//起始和发送一个字节之后I2C_W_SCL都是0
	Delay_us(10);
	for(a=0; a<8; a++)//接收8个字节
	{
		I2C_W_SCL(1);
		Delay_us(10);
		dat <<= 1;
		dat |= GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_14);
		Delay_us(10);
		I2C_W_SCL(0);
		Delay_us(10);
	}
	return dat;		
}


//函数功能		   : 往24c02的一个地址写入一个数据
void At24c02Write(unsigned char addr,unsigned char dat)
{
	First_I2C_Start();
	First_I2C_SendByte(0xa0, 1);//发送写器件地址
	First_I2C_SendByte(addr, 1);//发送要写入内存地址
	First_I2C_SendByte(dat, 0);	//发送数据
	First_I2C_Stop();
}


// 读取24c02的一个地址的一个数据
unsigned char At24c02Read(unsigned char addr)
{
	unsigned char num;
	First_I2C_Start();
	First_I2C_SendByte(0xa0, 1); //发送写器件地址
	First_I2C_SendByte(addr, 1); //发送要读取的地址
	First_I2C_Start();
	First_I2C_SendByte(0xa1, 1); //发送读器件地址
	num=First_I2C_ReadByte(); //读取数据
	First_I2C_Stop();
	return num;	
}

