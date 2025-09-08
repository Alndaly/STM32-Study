#include "LED.h"
#include "OLED.h"
#include "Serial.h"
#include "misc.h"
#include <string.h>

int main() {
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  Serial_Init();
  OLED_Init();
  LED_Init();

  for (;;) {
    if (Serial_RxFlag == 1) {
      OLED_Clear();
      OLED_ShowString(1, 1, Serial_RxPacket);
      if (strcmp(Serial_RxPacket, "LED_ON") == 0) {
        LED1_ON();
      } else if (strcmp(Serial_RxPacket, "LED_OFF") == 0) {
        LED1_OFF();
      } else {
        OLED_ShowString(2, 1, "Wrong Command");
        Serial_SendString("Wrong Command");
      }
      Serial_RxFlag = 0;
    }
  }
}
