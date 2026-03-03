#include "LCD.h"

static uint8_t cursor_pos = 0;

void lcd_command(uint8_t cmd){
    GPIO_PORTC_DATA_R = (GPIO_PORTC_DATA_R & ~(LCD_DATA)) | (cmd & LCD_DATA); //Put the four highest bits of CMD on portC
    GPIO_PORTD_DATA_R &= ~(LCD_RS); //Select command register
    GPIO_PORTD_DATA_R |= LCD_EN; //Pull enable pin high to latch data
    timeout_ms(1);
    GPIO_PORTD_DATA_R &= ~(LCD_EN); //Pull enable pin low
    timeout_ms(1); // give it time to write to the register

    GPIO_PORTC_DATA_R = (GPIO_PORTC_DATA_R & ~(LCD_DATA)) | (cmd<<4); //write the lowest four bits of CMD on portD
    GPIO_PORTD_DATA_R |= LCD_EN; //Pull enable pin high to latch data
    timeout_ms(1);
    GPIO_PORTD_DATA_R &= ~(LCD_EN); //Pull enable pin low
    timeout_ms(1); // give it time to write to the register
}



void lcd_init(){
    //TIVA HARDWARE SETUP
    systick_init(15999);

    SYSCTL_RCGC2_R = (SYSCTL_RCGC2_GPIOC | SYSCTL_RCGC2_GPIOD); //initialize GPIOC & GPIOD
    uint32_t dummy = SYSCTL_RCGC2_R;

    timeout_ms(15);
    //Set data direction
    GPIO_PORTC_DIR_R |= LCD_DATA; //PC7-PC4 as output
    GPIO_PORTD_DIR_R |= (LCD_RS | LCD_EN); //PD2 and PD3 as output

    //Set digital function
    GPIO_PORTC_DEN_R |= LCD_DATA;
    GPIO_PORTD_DEN_R |= (LCD_RS | LCD_EN);

    //LCD SETUP
    GPIO_PORTD_DATA_R &= ~(LCD_RS | LCD_EN); //pull control lines low for minimization of funny business

    lcd_command(0x02); //return to start of dram
    timeout_ms(5); // wait for execution time of
    lcd_command(0x28); // 4-bit mode, 2 lines, 8 dot per symbol
    timeout_ms(1);
    lcd_command(0x01); // clear display
    lcd_command(0x0C); //Turn display on, turn off cursor
    timeout_ms(1);
    //lcd_command(0x06);
}

void lcd_setcursor(uint8_t pos){
    if(pos>=32){return;}
    uint8_t shift_cmd;
    int8_t dir;
    if(pos>cursor_pos){
        dir = 1;
        shift_cmd = 0b00010100;
    } else if (pos<cursor_pos){
        dir = -1;
        shift_cmd = 0b00010000;
    } else{
        return;
    }
    while(cursor_pos!=pos){
        if(cursor_pos==16){
            uint8_t i;
            for(i=0; i<24; i++ ){
                lcd_command(shift_cmd);
            }
        }
        lcd_command(shift_cmd);
        cursor_pos += dir;
    }
    return;
}

void lcd_char(uint8_t character){
    switch(cursor_pos){
        case 16:
        {
            uint8_t i;
            for(i=0; i<24; i++ ){
                lcd_command(0b00010100);
            }
        }
            break;
        case 32:
        {
            lcd_setcursor(0);
        }
            break;
    }
    GPIO_PORTC_DATA_R = (GPIO_PORTC_DATA_R & ~(LCD_DATA)) | (character & LCD_DATA); //Put the four highest bits of CMD on portC
    GPIO_PORTD_DATA_R |= LCD_RS; //Select data register
    GPIO_PORTD_DATA_R |= LCD_EN; //Pull enable pin high to latch data
    timeout_ms(1);
    GPIO_PORTD_DATA_R &= ~(LCD_EN); //Pull enable pin low

    GPIO_PORTC_DATA_R = (GPIO_PORTC_DATA_R & ~(LCD_DATA)) | (character<<4); //write the lowest four bits of CMD on portD
    GPIO_PORTD_DATA_R |= LCD_EN; //Pull enable pin high to latch data
    timeout_ms(1);
    GPIO_PORTD_DATA_R &= ~(LCD_EN); //Pull enable pin low
    timeout_ms(1); // give it time to write to the register
    cursor_pos++;
}

void lcd_string(uint8_t *str){
    while(*str){
        lcd_char(*str);
        str++;
    }
}

void lcd_clear(){
    lcd_command(0x01);
    cursor_pos = 0;
}



uint8_t lcd_getcursor(){
    return cursor_pos;
}


