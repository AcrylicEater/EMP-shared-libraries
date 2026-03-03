/***************** Header *********************
* Univeristy of Southern Denmark
* Embedded Programming (EMP)
*
* MODULENAME: ButtonBro
* PROJECT: Standard library
* DESCRIPTION: easy configure of buttons
* Change log:
**********************************************
* Date of Change
* 23/02/2026: MODULE CREATED
**********************************************/

/***************** Include files **************/
#ifndef BUTTONBRO_H
#define BUTTONBRO_H
#include <stdint.h>
#include <stddef.h>
#include <tm4c123gh6pm.h>
#include <systick.h>
#include <led_indicator.h>

/***************** Defines ********************/
//#define SW1           0x10
#define SYSTICK_1MS   15999

#define DEFAULT_DEBOUNCE_TIME     30
#define DEFAULT_DOUBLE_PRESS_TIME 400
#define DEFAULT_LONG_PRESS_TIME   1000

typedef void (*ButtonCallback)(void);

typedef struct
{
    // Timing parameters in ms
    uint32_t debounceTime;
    uint32_t doublePressTime;
    uint32_t longPressTime;

    // Optional callbacks
    ButtonCallback singlePress;
    ButtonCallback doublePress;
    ButtonCallback longPress;
    ButtonCallback releasePress;

} ButtonConfig;

typedef struct //Når man virkelig savner C++
{
    ButtonConfig config;
    uint8_t pinMask;

    // runtime variables
    uint64_t lastSWevent;     //timestamp for last time switch changed, used for debounce
    uint64_t lastReleaseEvent;//timestamp for last time switch was let go, used for double press
    uint8_t retrigger;        //flag to avoid retrigger on hold
    uint8_t LP_retrigger;     //flag to avoid retrigger on hold for long press
} Button;


/***************** Functions*******/
void Button_init(Button *btn, uint8_t pinMask, ButtonConfig *user_config);
/**********************************************
* Input: Pointer to button object, pin mask of button bit and pointer to user config
* Output: none
* Function: initialize button object and associated hardware. undefined configs will go to default
* Example use: initialize SW1 with functionality for release, single press and long press, with custom long press time

void write_SP(void){
    lcd_clear();
    lcd_string("Single press SW1!");
}

void write_LP(void){
    lcd_clear();
    lcd_string("Long press SW1!");
}

Button sw1;

void init(void){
 ButtonConfig cfg = {0}; //initialize config with default values
 cfg.longPressTime = 1000; //only write those you want to change
 cfg.longPress = write_LP;
 cfg.singlePress = write_SP;
 Button_init(&sw1, 0x10, &cfg); //call init function with pointer to sw object, pin mask and config
}


***********************************************/
void Button_update(Button *btn);
/**********************************************
* Input: button object to update
* Output: none (function calls to button handlers)
* Function: checks button state and calls callbacks if necessary
***********************************************/
#endif
