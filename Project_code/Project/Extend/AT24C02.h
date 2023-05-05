#ifndef __AT24C02_H
#define __AT24C02_H


void AT24C02_Init(void);
void At24c02Write(unsigned char addr,unsigned char dat);
unsigned char At24c02Read(unsigned char addr);



#endif
