#ifndef __DHT_H
#define __DHT_H

extern u8 p[5];
extern u8 ret;

void DHT11_Init(void);
u8 DHT11_ReadData(void);

#endif
