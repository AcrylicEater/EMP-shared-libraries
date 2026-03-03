#include "led_indicator.h"

void init_RGB(void){
    SYSCTL_RCGC2_R = SYSCTL_RCGC2_GPIOF; //initialize GPIOF
    uint32_t dummy = SYSCTL_RCGC2_R;
    GPIO_PORTF_DIR_R |= LED_MASK; // set proper PORTF bits to output
    GPIO_PORTF_DEN_R |= LED_MASK; // set digital function
}

void set_Color(led_Color){
    GPIO_PORTF_DATA_R = (GPIO_PORTF_DATA_R & ~(LED_MASK)) | led_Color; //set corresponding RGB bits on/off
}


