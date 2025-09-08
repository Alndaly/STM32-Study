#include <stdint.h>
#include "stm32f10x.h"

void delay_ms(uint32_t ms) {
  uint32_t ticks = SystemCoreClock / 1000; // 每 ms 的时钟数
  if (ticks == 0 || ticks > SysTick_LOAD_RELOAD_Msk)
    return; // 容错
  uint32_t ctrl_backup =
      SysTick->CTRL; // （可选）备份原有配置 —— 但并非所有平台都安全恢复
  SysTick->LOAD = ticks - 1;
  SysTick->VAL = 0;
  SysTick->CTRL = SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_CLKSOURCE_Msk; // 0x5
  for (uint32_t i = 0; i < ms; i++) {
    while (!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk))
      ;
  }
  SysTick->CTRL = ctrl_backup; // 尝试恢复（注意：恢复也可能导致不一致，需小心）
}
