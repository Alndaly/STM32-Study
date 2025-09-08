#include "OLED.h"
#include "misc.h"
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_usart.h"
#include <stdint.h>

char Serial_RxPacket[100];
uint8_t Serial_RxFlag;

void Serial_Init(void) {
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  USART_InitTypeDef USART_InitStructure;
  USART_InitStructure.USART_BaudRate = 9600;
  USART_InitStructure.USART_HardwareFlowControl =
      USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_Init(USART1, &USART_InitStructure);

  USART_Cmd(USART1, ENABLE);

  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

  NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

void Serial_SendByte(uint8_t Byte) {
  USART_SendData(USART1, Byte);
  while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
    ;
}

void Serial_SendString(char *String) {
  while (*String != '\0') {
    Serial_SendByte(*String);
    String++;
  }
}

void USART1_IRQHandler(void) {

  static uint8_t RxState = 0;
  static uint8_t pRxPackage = 0;

  if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET) {

    uint8_t RxData = USART_ReceiveData(USART1);

    if (RxState == 0) {

      if (RxData == '@' && Serial_RxFlag == 0) {
        RxState = 1;
        pRxPackage = 0;
      }

    } else if (RxState == 1) {

      if (RxData != '@' && RxData != '#') {
        Serial_RxPacket[pRxPackage] = RxData;
        pRxPackage++;
      } else {
        RxState = 0;
        Serial_RxPacket[pRxPackage] = '\0';
        Serial_RxFlag = 1;
      }
    }

    USART_ClearITPendingBit(USART1, USART_IT_RXNE);
  }
}