/***************** Header *********************/
/**********************************************
* Univeristy of Southern Denmark
* Embedded Programming (EMP)
*
* MODULENAME: led_indicatpr.c
* PROJECT: EMP
* DESCRIPTION: control RGB LED
* Change log:
**********************************************
* Date of Change
* 16/02/2026: MODUKE CREATED
**********************************************


/***************** Include files **************/
#ifndef LED_INDICATOR_H
#define LED_INDICATOR_H
#include <stdint.h>
#include "tm4c123gh6pm.h"
/***************** Defines ********************/
#define LED_MASK  0b00001110

enum led_Color {
    BLACK = 0,
    RED = 0b00000010,
    BLUE = 0b00000100,
    PINK = 0b00000110,
    GREEN = 0b00001000,
    YELLOW = 0b00001010,
    CYAN = 0b00001100,
    WHITE = 0b00001110,
};



/***************** Functions ******************/
extern void set_Color(led_Color);
/**********************************************
* Input: color to set RGB LED
* Output: none
* Function: the color will change
***********************************************/

extern void init_RGB(void);
/**********************************************
* Input: none
* Output: none
* Function: initiliazes the proper GPIO pins
***********************************************/

/***************** End of module **************/
#endif
