/*
 * LCD.h
 *
 *  Created on: Sep 12, 2019
 *      Author: csant
 */

#include <unistd.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

/* Driver Header files */
#include <ti/drivers/GPIO.h>

#ifndef lcd_h
#define lcd_h

/* Command Set for LCD Display HD4478, 16x2 */
#define DISPLAY_ON 0x0F // Turns on the display, cursor and blinking of cursor
#define DISPLAY_OFF 0x08 // Turns off the display, cursor and blinking of cursor
#define CLEAR_DISPLAY 0x01 // Clears everything from the Display
#define FUNCTION_SET 0x2C // Function Set: 8-bit mode, 2-lines, 5x8 Grid, 40us
#define FUNCTION_RESET 0x3 // Function Reset, default command for initializing 4-bit mode
#define EMS 0x06 // Turns on cursor increment, with no display shift
#define EMSS 0x07 // Turns on cursor increment, with display shift
#define LINE_TWO 0x40// Sends cursor to second line
#define LINE_ONE 0x00 // Sends cursor to first line
#define SET_CURSOR 0x80 //Sets cursor
#define HOME 0x02 // Sets cursor to the first position of the line it is currently in
#define LOW (0) // Logical-low
#define HIGH (1) // Logical-high

uint_least8_t D7;
uint_least8_t D6;
uint_least8_t D5;
uint_least8_t D4;
uint_least8_t Rs;
uint_least8_t E;


/* Initializes the LCD with the 4-bit configuration
 *
 * params: None
 * returns: None
 *
 * Note: Check if the GPIOs used in this method
 * are the same GPIOs as you are using.
 *
 *  */
void LCD_init(uint_least8_t  E, uint_least8_t  RS, uint_least8_t  D7, uint_least8_t  D6, uint_least8_t  D5, uint_least8_t  D4);
/* Sends the string to the write function */
void write_char(uint8_t *);
/* Sends the initialization commands to the LCD */
void send_command(uint8_t);
/* Writes the data/command to the LCD */
void write(uint8_t);
/* Toggles Enable in LCD */
void enable();
/* Clears everything from the display*/
void clear();





#endif /* LCD_H_ */


