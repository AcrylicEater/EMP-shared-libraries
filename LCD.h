/***************** Header *********************/
/**********************************************
* Univeristy of Southern Denmark
* Embedded Programming (EMP)
*
* MODULENAME:  LCD.h
* PROJECT: EMP
* DESCRIPTION: control emp board lcd
* Change log:
**********************************************
* Date of Change
* 25/02/2025
**********************************************/
/***************** Include files **************/
#ifndef LCD_H_
#define LCD_H_
#include <tm4c123gh6pm.h>
#include <systick.h>
#include <stdint.h>
/***************** Defines ********************/
#define LCD_DATA 0b11110000
#define LCD_RS   0b00000100
#define LCD_EN   0b00001000
/***************** Functions ******************/
void lcd_command(uint8_t cmd);
/**********************************************
* Input: 8bit command to send to lcd
* Output: none
* Function: sends command to lcd
***********************************************/
void lcd_init();
/**********************************************
* Input: none
* Output: none
* Function: initializes LCD functionality
***********************************************/
void lcd_char(uint8_t character);
/**********************************************
* Input: character to write
* Output: none
* Function: writes a single character at lcd cursor position
***********************************************/
void lcd_string(uint8_t *str);
/**********************************************
* Input: string to write
* Output: none
* Function: writes a string at cursor position
***********************************************/
void lcd_clear();
/**********************************************
* Input: none
* Output: none
* Function: clears display and resets cursor position
***********************************************/
void lcd_setcursor(uint8_t pos);
/**********************************************
* Input: cursor position
* Output: none
* Function: sets cursor position
***********************************************/
uint8_t lcd_getcursor();
/**********************************************
* Input: none
* Output: current cursor position
* Function: returns cursor position
***********************************************/


#endif /* LCD_H_ */
