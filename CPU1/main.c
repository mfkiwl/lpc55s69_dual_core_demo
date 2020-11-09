#include "LPC55S69_cm33_core0.h"

#define LED_GPIO_BLOCK 1
#define LED_GPIO_PIN 4

static inline void Delay(void)
{
  for(int i = 0; i < 8000000; i++);
}

int main()
{  
  /**** LED setup ****/
  // Enable clocking for GPIO1 (located on AHB bus)
  SYSCON->AHBCLKCTRLX[0] |= SYSCON_AHBCLKCTRL0_GPIO1_MASK;

  // Set direction for GPIO1 7 as output
  GPIO->DIR[LED_GPIO_BLOCK] |= GPIO_DIR_DIRP(1 << LED_GPIO_PIN);
  
  while(1)
  {
    /**** LED blinking ****/
    GPIO->B[LED_GPIO_BLOCK][LED_GPIO_PIN] = 0;  // Turn led On
    Delay();
    GPIO->B[LED_GPIO_BLOCK][LED_GPIO_PIN] = 1;  // Turn led off
    Delay();
  }
}