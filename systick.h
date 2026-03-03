/*****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* MODULENAME.: systick.h
*
* PROJECT....: EMP
*
* DESCRIPTION: Initialize the systick interrupt.
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 150215  MoH    Module created.
*
*****************************************************************************/


/***************************** Include files *******************************/
#ifndef SYSTICK_H
#define SYSTICK_H
#include <stdint.h>
#include "tm4c123gh6pm.h"
/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Functions   *******************************/
void enable_global_int();
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : Enable global interrupt.
******************************************************************************/


void disable_global_int();
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : Disable global interrupt.
******************************************************************************/


void systick_init(uint32_t reload);
/*****************************************************************************
*   Input    : - interval for increasing ticks
*   Output   : - 
*   Function : Initialize the systick interrupt, will return the reload value. Safeguarded against multiple calls
******************************************************************************/

void systick_handler();

uint64_t get_ticks();
/*****************************************************************************
*   Input    : -
*   Output   : - systick
*   Function : get the amount of ticks passed
******************************************************************************/

uint64_t get_ms();
/*****************************************************************************
*   Input    : -
*   Output   : - ms
*   Function : get the amount of ms passed, regardless of tick speed setting
******************************************************************************/

void timeout_ticks(uint64_t ticks);
/*****************************************************************************
*   Input    : - amount of ticks to wait
*   Output   : -
*   Function : wait for amount of ticks
******************************************************************************/

void timeout_ms(uint64_t ms);
/*****************************************************************************
*   Input    : - amount of ms to wait
*   Output   : -
*   Function : wait for amount of ms
******************************************************************************/

#endif




