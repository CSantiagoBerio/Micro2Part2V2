/*
 * LCD.c
 *
 *  Created on: Sep 12, 2019
 *      Author: csant
 */
/* Icnludes for default C methods */
#include <unistd.h>
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

#include "lcd.h"

/* TI SimpleLink CC3220S SDK Header files */
#include <ti/drivers/GPIO.h>
//#include <ti\devices\cc32xx\driverlib\gpio.h>
//#include <ti\devices\cc32xx\inc\hw_memmap.h>
/* Driver configuration */
#include "ti_drivers_config.h"

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

///****************************** LCD Library *****************************************/

/* Initializes LCD */
void LCD_init(uint_least8_t  E, uint_least8_t  RS, uint_least8_t  D7, uint_least8_t  D6, uint_least8_t  D5, uint_least8_t  D4);
/* Sends the string to the write function */
void write_char(uint8_t chars[]);
/* Sends the initialization commands to the LCD */
void send_command(uint8_t hex);
/* Writes the data/command to the LCD */
void write(uint8_t byte);
/* Toggles Enable in LCD */
void enable();
/* Clears everything from the display*/
void clear();

uint_least8_t  E, RS, D7, D6, D5, D4;

//
///***********************************************************************************/

void LCD_init(uint_least8_t  e, uint_least8_t  rs, uint_least8_t  d7, uint_least8_t  d6, uint_least8_t  d5, uint_least8_t  d4)
{

    D7 = d7;
    D6 = d6;
    D5 = d5;
    D4 = d4;
    RS = rs;
    E = e;

    /*///////////////////////////////////////////// Configures the pins as output //////////////////////////////////////////////////////// */

    /* Data Pins */
    GPIO_setConfig(D4, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_HIGH);
    GPIO_setConfig(D5, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_HIGH);
    GPIO_setConfig(D6, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_HIGH);
    GPIO_setConfig(D7, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_HIGH);

    GPIO_write(D4, LOW);
    GPIO_write(D5, LOW);
    GPIO_write(D6, LOW);
    GPIO_write(D7, LOW);


    /* Control Pins */
    GPIO_setConfig(E, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_HIGH);
    GPIO_setConfig(RS, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_HIGH);


    usleep(40000);

    GPIO_write(RS, LOW); // RS = 0;

    write(FUNCTION_RESET); // Resets LCD from 8-bit to 4-bit mode
    usleep(40); // delay at least 40 us

    send_command(FUNCTION_SET); // Sets LCD configuration to 4-bit mode, 2-lines, 5x8 matrix
    usleep(40); // delay at least 40 us

    send_command(FUNCTION_SET); // Sets LCD configuration to 4-bit mode, 2-lines, 5x8 matrix
    usleep(40); // delay at least 40 us

    send_command(DISPLAY_OFF); // Display OFF, Cursor OFF, Blink OFF
    usleep(40); // wait 40 us

    send_command(CLEAR_DISPLAY); // Clears Screen
    usleep(40); // wait 40 us

    send_command(EMS); // Entry Mode Set
    usleep(40); // wait 1.53 ms

    send_command(DISPLAY_ON); // Display ON, Cursor ON, Blink ON
    usleep(40); // wait at least 40 us
}

void enable()
{
    usleep(40); // wait at least 40 us
    GPIO_write(E, HIGH); // E = 1
    usleep(40); // wait at least 40 us
    GPIO_write(E, LOW); // E = 0
    usleep(40);  // wait at least 40 us
}


void write_char(uint8_t chars[])
{
    volatile int i = 0;
    GPIO_write(RS, HIGH); // RS = 1
    usleep(50);

    while(!(chars[i] == NULL)) // while byte[i] != \0
    {
        write(chars[i]); // Send data to pins
        write(chars[i] << 4);
        i++; // Move to next element in char array
        sleep(0.0001); // wait for at least 0.1 ms
//        enable();
    }
}


void send_command(uint8_t hex)
{
    GPIO_write(RS, LOW); // RS = 0

    GPIO_write(D7, (1 & (hex >> 7)));
    GPIO_write(D6, (1 & (hex >> 6)));
    GPIO_write(D5, (1 & (hex >> 5)));
    GPIO_write(D4, (1 & (hex >> 4)));


    enable();


    uint8_t shifted = hex << 4;

    GPIO_write(D7, (1 & (shifted >> 7)));
    GPIO_write(D6, (1 & (shifted >> 6)));
    GPIO_write(D5, (1 & (shifted >> 5)));
    GPIO_write(D4, (1 & (shifted >> 4)));

    enable();

}

void write(uint8_t byte)
{

    GPIO_write(D7, (1 & (byte >> 7)));
    GPIO_write(D6, (1 & (byte >> 6)));
    GPIO_write(D5, (1 & (byte >> 5)));
    GPIO_write(D4, (1 & (byte >> 4)));

    enable();

//    uint8_t shifted = byte << 4;
//
//    GPIO_write(D7, (1 & (shifted >> 7)));
//    GPIO_write(D6, (1 & (shifted >> 6)));
//    GPIO_write(D5, (1 & (shifted >> 5)));
//    GPIO_write(D4, (1 & (shifted >> 4)));
//
//    enable();

}

void clear(){
    //    send_command(DISPLAY_OFF); // Display OFF, Cursor OFF, Blink OFF
    //    usleep(40); // wait 40 us
    send_command(CLEAR_DISPLAY); // Clears Screen
    //    usleep(40); // wait 40 us
    //    send_command(DISPLAY_ON); // DIsplay ON, Cursor ON, Blink ON
    usleep(40); // wait at least 40 us
    send_command(HOME);
}
