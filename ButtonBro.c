#include "ButtonBro.h"


const ButtonConfig Default_config = {
    .debounceTime     = DEFAULT_DEBOUNCE_TIME,
    .doublePressTime  = DEFAULT_DOUBLE_PRESS_TIME,
    .longPressTime    = DEFAULT_LONG_PRESS_TIME,
    .singlePress      = NULL,
    .doublePress      = NULL,
    .longPress        = NULL,
    .releasePress     = NULL
};


void Button_init(Button *btn, uint8_t pinMask, ButtonConfig *user_config){
    //Setup config of button object
    btn->config = Default_config;
    if(user_config != NULL)
    {
        if(user_config->debounceTime) {btn->config.debounceTime = user_config->debounceTime;}
        if(user_config->doublePressTime) {btn->config.doublePressTime = user_config->doublePressTime;}
        if(user_config->longPressTime) {btn->config.longPressTime = user_config->longPressTime;}
        btn->config.singlePress = user_config->singlePress;
        btn->config.doublePress = user_config->doublePress;
        btn->config.longPress   = user_config->longPress;
        btn->config.releasePress = user_config->releasePress;
    }

    //Setup pin mask
    btn->pinMask = pinMask;

    //initialize runtime variables of button object
    btn->lastSWevent = 0;
    btn->lastReleaseEvent = 0;
    btn->retrigger = 0;
    btn->LP_retrigger = 0;
    
    //Hardware setup
    systick_init(SYSTICK_1MS);

    //SW setup
    if(pinMask == 0x01){ //special considerations for locked GPIOF0
        GPIO_PORTF_LOCK_R = 0x4C4F434B;   // unlock GPIO Port F
        GPIO_PORTF_CR_R = 0x1F;           // allow changes
    }

    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R5;
    while((SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_R5) == 0);
    GPIO_PORTF_DIR_R &= ~(pinMask); //SW bit as input
    GPIO_PORTF_DEN_R |= pinMask; // enable digital function
    GPIO_PORTF_PUR_R |= pinMask; // enable pull-ups on SW pin
}

//update button object btn
void Button_update(Button *btn){ 
    //get information on current state
    uint8_t pressed = !(GPIO_PORTF_DATA_R & btn->pinMask);
    uint64_t current_timestamp = get_ms();

    //Detect if SW has been let go
    if(!pressed && btn->retrigger && ((current_timestamp - btn->lastSWevent) > btn->config.debounceTime)){
        //set debounce time and reset retrigger
        btn->lastSWevent = current_timestamp;
        btn->retrigger = 0;
        btn->LP_retrigger = 0;
        btn->lastReleaseEvent = current_timestamp; //set timestamp for when switch was let go
        if(btn->config.releasePress != NULL){btn->config.releasePress();}
    }

    //Detect single press of SW1, by checking portf, retrigger flag and
    //avoiding bouncing by checking the timestamp of the previous switch event
    if(pressed && !btn->retrigger && ((current_timestamp - btn->lastSWevent) > btn->config.debounceTime)){
        //set debounce time and retrigger
        btn->lastSWevent = current_timestamp;
        btn->retrigger = 1;
        if(btn->config.singlePress != NULL){btn->config.singlePress();}

        //detect double press
        if((btn->lastSWevent - btn->lastReleaseEvent) <= btn->config.doublePressTime){
            if(btn->config.doublePress != NULL){btn->config.doublePress();}
        }

    }

    //Detect long press of SW1, by checking if SW1 in portF is still down and it being
    //more than longPressTime since the last switch event
    if(pressed && ((current_timestamp - btn->lastSWevent) >= btn->config.longPressTime)){
        if(btn->LP_retrigger == 0){
            btn->LP_retrigger = 1;
            if(btn->config.longPress != NULL){btn->config.longPress();}
        }
    }

}
