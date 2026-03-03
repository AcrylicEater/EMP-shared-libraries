#include "systick.h"


// Missing definitions in tm4c123gh6pm.h file
#define NVIC_INT_CTRL_PEND_SYST   0x04000000    // Pend a systick int
#define NVIC_INT_CTRL_UNPEND_SYST 0x02000000    // Unpend a systick int

#define SYSTICK_PRIORITY    0x7E

volatile uint64_t ticks = 0;
volatile float timescale_ms = 1.0;

void systick_handler(void)
/*****************************************************************************
*   Function : See module specification (.h-file).
*****************************************************************************/
{
  // Hardware clears systick int reguest
  ticks++;
}


void systick_init(uint32_t reload)
{
  static uint8_t is_init = 0;
  if(is_init){return;}
  // Disable systick timer
  NVIC_ST_CTRL_R &= ~(NVIC_ST_CTRL_ENABLE);

  // Set current systick counter to reload value
  NVIC_ST_CURRENT_R = reload;
  // Set Reload value, Systick reload register
  NVIC_ST_RELOAD_R = reload;

  timescale_ms = (15999.0+1) / (float)(reload+1);

  // NVIC systick setup, vector number 15
  // Clear pending systick interrupt request
  NVIC_INT_CTRL_R |= NVIC_INT_CTRL_UNPEND_SYST;

  // Set systick priority to 0x10, first clear then set.
  NVIC_SYS_PRI3_R &= ~(NVIC_SYS_PRI3_TICK_M);
  NVIC_SYS_PRI3_R |= (NVIC_SYS_PRI3_TICK_M & (SYSTICK_PRIORITY<<NVIC_SYS_PRI3_TICK_S));

  // Select systick clock source, Use core clock
  NVIC_ST_CTRL_R |= NVIC_ST_CTRL_CLK_SRC;

  // Enable systick interrupt
  NVIC_ST_CTRL_R |= NVIC_ST_CTRL_INTEN;

  // Enable and start timer
  NVIC_ST_CTRL_R |= NVIC_ST_CTRL_ENABLE;

  is_init = 1;
}

uint64_t get_ticks(){
    return ticks;
}

void timeout_ticks(uint64_t ticks){
   uint64_t start_timestamp = get_ticks();
   while(get_ticks() - start_timestamp < ticks);
   return;  
}

void timeout_ms(uint64_t ms){
   uint64_t start_timestamp = get_ticks();
   uint64_t ticks = (uint64_t)((float)ms * timescale_ms);
   while(get_ticks() - start_timestamp < ticks);
   return;  
}

uint64_t get_ms(){
    return (uint64_t)((float)ticks / timescale_ms);
}