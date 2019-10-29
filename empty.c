/*
 * Copyright (c) 2015-2019, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  ======== empty.c ========
 */

/* For usleep() */
#include <unistd.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <ctype.h>

/* LCD Header */
#include "lcd.h"

/* Driver Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/I2C.h>
// #include <ti/drivers/SPI.h>
#include <ti/drivers/UART.h>
// #include <ti/drivers/Watchdog.h>
#include <ti/devices/cc32xx/driverlib/uart.h>
#include <ti/drivers/uart/UARTCC32XX.h>

/* Driver configuration */
#include "ti_drivers_config.h"

/* Global Variables */
UART_Handle uart;
UART_Params params;
UART_Handle local;
UART_Params locparams;

I2C_Handle i2c;
I2C_Params i2cparams;

int counter = 0;
uint8_t memBuf[16];
int Done = 0;

void sendToLCD(UART_Handle handle, void *buf, size_t count)
{
    Done = 1;
    const char *c = buf;
    if(strcmp(*c, 0x08) == 0){
        Done = 2;
    }
}

void initialize_UART(){

    UART_Params_init(&params);
    params.baudRate = 9600;
    params.dataLength = UART_LEN_8;
    params.stopBits = UART_STOP_ONE;
    params.readEcho = UART_ECHO_OFF;

    params.readDataMode = UART_DATA_TEXT;
    params.readMode = UART_MODE_CALLBACK;
    params.readReturnMode = UART_RETURN_NEWLINE;
    params.readCallback = sendToLCD;

    params.writeDataMode = UART_DATA_TEXT;
    params.writeMode = UART_MODE_BLOCKING;

    uart = UART_open(UART, &params);
    if(uart){
        const char mess[40] = "Module Uart initialized Correctly\r\n";
        UART_write(uart, &mess, sizeof(mess));
    }
}

void uartTask(){
    initialize_UART();

    uint8_t input[1];

    write_char("Rock n' Roll");
    //    UART_read(uart, &input, 1);

    while(1){
        UART_read(uart, &input, 1);
        if(Done == 1){
            write_char(input);
            UART_write(uart, &input, 1);
            Done = 0;
            counter++;

        }else if(Done == 2){
            send_command(CLEAR_DISPLAY);
            Done = 0;
            counter = 0;
        }
        if(counter == 15){
            send_command(LINE_TWO);
            counter = 0;
            Done = 0;
        }
    }
}

I2C_Transaction i2cTransaction = {0};
uint8_t setTime[10] = {0x00, 0x00, 0x00, 0x40, 0x15, 0x02, 0x28, 0x10, 0x19, 0x00};

I2C_Transaction i2cRead = {0};
uint8_t getTime[8];
uint8_t readAddress[2] = {0x00, 0x00};
uint8_t readFrom[1] = {0x01};



void initialize_I2C(){
    bool status;
    I2C_Params_init(&i2cparams);
    i2cparams.bitRate = I2C_100kHz;

    i2c = I2C_open(I2C, &i2cparams);
    if(i2c == NULL){
        while(1);
    }

    i2cTransaction.slaveAddress = 0x68;
    i2cTransaction.writeBuf = setTime;
    i2cTransaction.writeCount = 0;
    i2cTransaction.readCount = 0;
    i2cTransaction.readBuf = NULL;

//    status = I2C_transfer(i2c, &i2cTransaction);

    if(!status){
        while(1);
    }

}

void read(){
    bool status1 = false;
    bool status0;
    //Read from DS307
    i2cRead.slaveAddress = 0x68;
    i2cRead.writeBuf = readAddress;
    i2cRead.writeCount = 2;
    i2cRead.readBuf = getTime;
    i2cRead.readCount = 0;

    if(I2C_transfer(i2c, &i2cRead)){
        printf("Slave Address Correct\n");
        usleep(20000);
    }else{
        printf("Address Incorrect\n");
    }

    i2cRead.writeBuf = readAddress;
    i2cRead.writeCount = 0;
    i2cRead.readBuf = getTime;
    i2cRead.readCount = 8;
    status1 = I2C_transfer(i2c, &i2cRead);

}

void i2cTask(){
    bool status1 = false;
    bool status0;
    initialize_I2C();

    read();

    while(1){
        sleep(1);
        read();


    }

}

/*
 *  ======== mainThread ========
 */
void *mainThread(void *arg0)
{
    GPIO_init();
    UART_init();
    I2C_init();

    LCD_init(GPIO_E, GPIO_RS, GPIO_D7, GPIO_D6, GPIO_D5, GPIO_D4);
    //    LCD_init(GPIO_E, GPIO_RS, GPIO_D7, GPIO_D6, GPIO_D5, GPIO_D4);

//    uartTask();
        i2cTask();






    return (NULL);
}
