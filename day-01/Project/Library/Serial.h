#ifndef __SERIAL_H
#define __SERIAL_H

#include <stdint.h>

extern uint8_t Serial_RxFlag;
extern char Serial_RxPacket[];

void Serial_Init(void);
void Serial_SendString(char *str);
void Serial_SendByte(uint8_t data);

#endif