#include "LPC55S69_cm33_core0.h"

#define LED_GPIO_BLOCK 1
#define LED_GPIO_PIN 7

// Sequence required to be written
// to SYSCON->CPUCTRL register ORed with other bits
// to applay settings
// See UM11126 4.5.70 for more details
#define SYSCON_CPUCTRL_ENABLING_SEQ 0xC0C40000

// Warning!!! IAR specific code. 
// core1_image_start is defined in 
// LPC55S69_cm33_core0_flash.icf linker config file
extern unsigned char core1_image_start[];
#define CORE1_IMAGE_START core1_image_start

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
  
  
  /**** CPU1 (slave) setup ****/
  // Enable CPU1
  SYSCON->CPUCFG |= SYSCON_CPUCFG_CPU1ENABLE_MASK;

  // Setup firmware boot address
  SYSCON->CPBOOT = SYSCON_CPBOOT_CPBOOT(CORE1_IMAGE_START);

  // Enable clock and reset CPU1
  SYSCON->CPUCTRL = SYSCON->CPUCTRL | SYSCON_CPUCTRL_ENABLING_SEQ | 
                      SYSCON_CPUCTRL_CPU1RSTEN_MASK | SYSCON_CPUCTRL_CPU1CLKEN_MASK;
    
  // Release reset for CPU1
  SYSCON->CPUCTRL = (SYSCON->CPUCTRL | SYSCON_CPUCTRL_ENABLING_SEQ) & 
                    (~SYSCON_CPUCTRL_CPU1RSTEN_MASK);


  while(1)
  {
    /**** LED blinking ****/
    GPIO->B[LED_GPIO_BLOCK][LED_GPIO_PIN] = 0;  // Turn led On
    Delay();
    GPIO->B[LED_GPIO_BLOCK][LED_GPIO_PIN] = 1;  // Turn led off
    Delay();
  }
}