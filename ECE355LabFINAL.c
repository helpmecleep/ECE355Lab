

// Worked on EXTI0,1 and PA0; October 20th
// Worked on EXTI0,1, GPIOC, and PA0; October 30th
// Worked on placing the oled template; October 30th
// Worked on EXTI1, PA1, and NE555 timer; November 7th
// Saleh:
// PA0 now works
//ADC and DAC work
//PA0 works
//Fixes to implement
//ADC DAC Connection
//PA0 Bounce fix
//Initialized GPIOB
//Initialized TIM3
//worked on Olde CMD and Data
//wokred on OLED config
// worked on OLED config
//
// This file is part of the GNU ARM Eclipse distribution.
// Copyright (c) 2014 Liviu Ionescu.
//

// ----------------------------------------------------------------------------
// School: University of Victoria, Canada.
// Course: ECE 355 "Microprocessor-Based Systems".
// This is template code for Part 2 of Introductory Lab.
//
// See "system/include/cmsis/stm32f051x8.h" for register/bit definitions.
// See "system/src/cmsis/vectors_stm32f051x8.c" for handler declarations.
// ----------------------------------------------------------------------------

#include <stdio.h>
#include "diag/Trace.h"
#include <string.h>
#include "cmsis/cmsis_device.h"

// ----------------------------------------------------------------------------
//
// STM32F0 empty sample (trace via $(trace)).
//
// Trace support is enabled by adding the TRACE macro definition.
// By default the trace messages are forwarded to the $(trace) output,
// but can be rerouted to any device or completely suppressed, by
// changing the definitions required in system/src/diag/trace_impl.c
// (currently OS_USE_TRACE_ITM, OS_USE_TRACE_SEMIHOSTING_DEBUG/_STDOUT).
//

// ----- main() ---------------------------------------------------------------

// Sample pragmas to cope with warnings. Please note the related line at
// the end of this function, used to pop the compiler diagnostics status.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"


/* Definitions of registers and their bits are
   given in system/include/cmsis/stm32f051x8.h */


/* Clock prescaler for TIM2 timer: no prescaling */
#define myTIM2_PRESCALER ((uint16_t)0x0000)
/* Maximum possible setting for overflow */
#define myTIM2_PERIOD ((uint32_t)0xFFFFFFFF)

/* Clock prescaler for TIM3 timer: no prescaling */
#define myTIM3_PRESCALER ((uint16_t)0x0000)
/* Maximum possible setting for overflow */
#define myTIM3_PERIOD ((uint32_t)0xFFFFFFFF)
/*ADC Scale*/
#define ADC_SCALE 4095

void myGPIO_Init(void);
void myTIM2_Init(void);
void myTIM3_Init(void);
void TIM3_Delay(uint32_t);
void myEXTI_Init(void);
void myADC_Init(void);
void myDAC_Init(void);

/*Relevant led inits*/
void oled_Write(unsigned char);
void oled_Write_Cmd(unsigned char);
void oled_Write_Data(unsigned char);
void oled_config(void);
void refresh_OLED(void);

// Declare/initialize your global variables here...
// NOTE: You'll need at least one global variable
// (say, timerTriggered = 0 or 1) to indicate
// whether TIM2 has started counting or not.
unsigned int count = 0;
float period = 0;
unsigned int frequency = 0;
unsigned int res = 0;
float adcVal = 0;
unsigned int inSig = 0; // Flags that tells user button is pressed

SPI_HandleTypeDef SPI_Handle;
//
// LED Display initialization commands
//
unsigned char oled_init_cmds[] =
{
    0xAE,
    0x20, 0x00,
    0x40,
    0xA0 | 0x01,
    0xA8, 0x40 - 1,
    0xC0 | 0x08,
    0xD3, 0x00,
    0xDA, 0x32,
    0xD5, 0x80,
    0xD9, 0x22,
    0xDB, 0x30,
    0x81, 0xFF,
    0xA4,
    0xA6,
    0xAD, 0x30,
    0x8D, 0x10,
    0xAE | 0x01,
    0xC0,
    0xA0
};

//
// Character specifications for LED Display (1 row = 8 bytes = 1 ASCII character)
// Example: to display '4', retrieve 8 data bytes stored in Characters[52][X] row
//          (where X = 0, 1, ..., 7) and send them one by one to LED Display.
// Row number = character ASCII code (e.g., ASCII code of '4' is 0x34 = 52)
//

unsigned char Characters[][8] = {
    {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,0b00000000, 0b00000000, 0b00000000},  // SPACE
    {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,0b00000000, 0b00000000, 0b00000000},  // SPACE
    {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,0b00000000, 0b00000000, 0b00000000},  // SPACE
    {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,0b00000000, 0b00000000, 0b00000000},  // SPACE
    {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,0b00000000, 0b00000000, 0b00000000},  // SPACE
    {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,0b00000000, 0b00000000, 0b00000000},  // SPACE
    {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,0b00000000, 0b00000000, 0b00000000},  // SPACE
    {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,0b00000000, 0b00000000, 0b00000000},  // SPACE
    {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,0b00000000, 0b00000000, 0b00000000},  // SPACE
    {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,0b00000000, 0b00000000, 0b00000000},  // SPACE
    {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,0b00000000, 0b00000000, 0b00000000},  // SPACE
    {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,0b00000000, 0b00000000, 0b00000000},  // SPACE
    {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,0b00000000, 0b00000000, 0b00000000},  // SPACE
    {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,0b00000000, 0b00000000, 0b00000000},  // SPACE
    {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,0b00000000, 0b00000000, 0b00000000},  // SPACE
    {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,0b00000000, 0b00000000, 0b00000000},  // SPACE
    {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,0b00000000, 0b00000000, 0b00000000},  // SPACE
    {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,0b00000000, 0b00000000, 0b00000000},  // SPACE
    {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,0b00000000, 0b00000000, 0b00000000},  // SPACE
    {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,0b00000000, 0b00000000, 0b00000000},  // SPACE
    {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,0b00000000, 0b00000000, 0b00000000},  // SPACE
    {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,0b00000000, 0b00000000, 0b00000000},  // SPACE
    {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,0b00000000, 0b00000000, 0b00000000},  // SPACE
    {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,0b00000000, 0b00000000, 0b00000000},  // SPACE
    {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,0b00000000, 0b00000000, 0b00000000},  // SPACE
    {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,0b00000000, 0b00000000, 0b00000000},  // SPACE
    {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,0b00000000, 0b00000000, 0b00000000},  // SPACE
    {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,0b00000000, 0b00000000, 0b00000000},  // SPACE
    {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,0b00000000, 0b00000000, 0b00000000},  // SPACE
    {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,0b00000000, 0b00000000, 0b00000000},  // SPACE
    {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,0b00000000, 0b00000000, 0b00000000},  // SPACE
    {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,0b00000000, 0b00000000, 0b00000000},  // SPACE
    {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,0b00000000, 0b00000000, 0b00000000},  // SPACE
    {0b00000000, 0b00000000, 0b01011111, 0b00000000, 0b00000000,0b00000000, 0b00000000, 0b00000000},  // !
    {0b00000000, 0b00000111, 0b00000000, 0b00000111, 0b00000000,0b00000000, 0b00000000, 0b00000000},  // "
    {0b00010100, 0b01111111, 0b00010100, 0b01111111, 0b00010100,0b00000000, 0b00000000, 0b00000000},  // #
    {0b00100100, 0b00101010, 0b01111111, 0b00101010, 0b00010010,0b00000000, 0b00000000, 0b00000000},  // $
    {0b00100011, 0b00010011, 0b00001000, 0b01100100, 0b01100010,0b00000000, 0b00000000, 0b00000000},  // %
    {0b00110110, 0b01001001, 0b01010101, 0b00100010, 0b01010000,0b00000000, 0b00000000, 0b00000000},  // &
    {0b00000000, 0b00000101, 0b00000011, 0b00000000, 0b00000000,0b00000000, 0b00000000, 0b00000000},  // '
    {0b00000000, 0b00011100, 0b00100010, 0b01000001, 0b00000000,0b00000000, 0b00000000, 0b00000000},  // (
    {0b00000000, 0b01000001, 0b00100010, 0b00011100, 0b00000000,0b00000000, 0b00000000, 0b00000000},  // )
    {0b00010100, 0b00001000, 0b00111110, 0b00001000, 0b00010100,0b00000000, 0b00000000, 0b00000000},  // *
    {0b00001000, 0b00001000, 0b00111110, 0b00001000, 0b00001000,0b00000000, 0b00000000, 0b00000000},  // +
    {0b00000000, 0b01010000, 0b00110000, 0b00000000, 0b00000000,0b00000000, 0b00000000, 0b00000000},  // ,
    {0b00001000, 0b00001000, 0b00001000, 0b00001000, 0b00001000,0b00000000, 0b00000000, 0b00000000},  // -
    {0b00000000, 0b01100000, 0b01100000, 0b00000000, 0b00000000,0b00000000, 0b00000000, 0b00000000},  // .
    {0b00100000, 0b00010000, 0b00001000, 0b00000100, 0b00000010,0b00000000, 0b00000000, 0b00000000},  // /
    {0b00111110, 0b01010001, 0b01001001, 0b01000101, 0b00111110,0b00000000, 0b00000000, 0b00000000},  // 0
    {0b00000000, 0b01000010, 0b01111111, 0b01000000, 0b00000000,0b00000000, 0b00000000, 0b00000000},  // 1
    {0b01000010, 0b01100001, 0b01010001, 0b01001001, 0b01000110,0b00000000, 0b00000000, 0b00000000},  // 2
    {0b00100001, 0b01000001, 0b01000101, 0b01001011, 0b00110001,0b00000000, 0b00000000, 0b00000000},  // 3
    {0b00011000, 0b00010100, 0b00010010, 0b01111111, 0b00010000,0b00000000, 0b00000000, 0b00000000},  // 4
    {0b00100111, 0b01000101, 0b01000101, 0b01000101, 0b00111001,0b00000000, 0b00000000, 0b00000000},  // 5
    {0b00111100, 0b01001010, 0b01001001, 0b01001001, 0b00110000,0b00000000, 0b00000000, 0b00000000},  // 6
    {0b00000011, 0b00000001, 0b01110001, 0b00001001, 0b00000111,0b00000000, 0b00000000, 0b00000000},  // 7
    {0b00110110, 0b01001001, 0b01001001, 0b01001001, 0b00110110,0b00000000, 0b00000000, 0b00000000},  // 8
    {0b00000110, 0b01001001, 0b01001001, 0b00101001, 0b00011110,0b00000000, 0b00000000, 0b00000000},  // 9
    {0b00000000, 0b00110110, 0b00110110, 0b00000000, 0b00000000,0b00000000, 0b00000000, 0b00000000},  // :
    {0b00000000, 0b01010110, 0b00110110, 0b00000000, 0b00000000,0b00000000, 0b00000000, 0b00000000},  // ;
    {0b00001000, 0b00010100, 0b00100010, 0b01000001, 0b00000000,0b00000000, 0b00000000, 0b00000000},  // <
    {0b00010100, 0b00010100, 0b00010100, 0b00010100, 0b00010100,0b00000000, 0b00000000, 0b00000000},  // =
    {0b00000000, 0b01000001, 0b00100010, 0b00010100, 0b00001000,0b00000000, 0b00000000, 0b00000000},  // >
    {0b00000010, 0b00000001, 0b01010001, 0b00001001, 0b00000110,0b00000000, 0b00000000, 0b00000000},  // ?
    {0b00110010, 0b01001001, 0b01111001, 0b01000001, 0b00111110,0b00000000, 0b00000000, 0b00000000},  // @
    {0b01111110, 0b00010001, 0b00010001, 0b00010001, 0b01111110,0b00000000, 0b00000000, 0b00000000},  // A
    {0b01111111, 0b01001001, 0b01001001, 0b01001001, 0b00110110,0b00000000, 0b00000000, 0b00000000},  // B
    {0b00111110, 0b01000001, 0b01000001, 0b01000001, 0b00100010,0b00000000, 0b00000000, 0b00000000},  // C
    {0b01111111, 0b01000001, 0b01000001, 0b00100010, 0b00011100,0b00000000, 0b00000000, 0b00000000},  // D
    {0b01111111, 0b01001001, 0b01001001, 0b01001001, 0b01000001,0b00000000, 0b00000000, 0b00000000},  // E
    {0b01111111, 0b00001001, 0b00001001, 0b00001001, 0b00000001,0b00000000, 0b00000000, 0b00000000},  // F
    {0b00111110, 0b01000001, 0b01001001, 0b01001001, 0b01111010,0b00000000, 0b00000000, 0b00000000},  // G
    {0b01111111, 0b00001000, 0b00001000, 0b00001000, 0b01111111,0b00000000, 0b00000000, 0b00000000},  // H
    {0b01000000, 0b01000001, 0b01111111, 0b01000001, 0b01000000,0b00000000, 0b00000000, 0b00000000},  // I
    {0b00100000, 0b01000000, 0b01000001, 0b00111111, 0b00000001,0b00000000, 0b00000000, 0b00000000},  // J
    {0b01111111, 0b00001000, 0b00010100, 0b00100010, 0b01000001,0b00000000, 0b00000000, 0b00000000},  // K
    {0b01111111, 0b01000000, 0b01000000, 0b01000000, 0b01000000,0b00000000, 0b00000000, 0b00000000},  // L
    {0b01111111, 0b00000010, 0b00001100, 0b00000010, 0b01111111,0b00000000, 0b00000000, 0b00000000},  // M
    {0b01111111, 0b00000100, 0b00001000, 0b00010000, 0b01111111,0b00000000, 0b00000000, 0b00000000},  // N
    {0b00111110, 0b01000001, 0b01000001, 0b01000001, 0b00111110,0b00000000, 0b00000000, 0b00000000},  // O
    {0b01111111, 0b00001001, 0b00001001, 0b00001001, 0b00000110,0b00000000, 0b00000000, 0b00000000},  // P
    {0b00111110, 0b01000001, 0b01010001, 0b00100001, 0b01011110,0b00000000, 0b00000000, 0b00000000},  // Q
    {0b01111111, 0b00001001, 0b00011001, 0b00101001, 0b01000110,0b00000000, 0b00000000, 0b00000000},  // R
    {0b01000110, 0b01001001, 0b01001001, 0b01001001, 0b00110001,0b00000000, 0b00000000, 0b00000000},  // S
    {0b00000001, 0b00000001, 0b01111111, 0b00000001, 0b00000001,0b00000000, 0b00000000, 0b00000000},  // T
    {0b00111111, 0b01000000, 0b01000000, 0b01000000, 0b00111111,0b00000000, 0b00000000, 0b00000000},  // U
    {0b00011111, 0b00100000, 0b01000000, 0b00100000, 0b00011111,0b00000000, 0b00000000, 0b00000000},  // V
    {0b00111111, 0b01000000, 0b00111000, 0b01000000, 0b00111111,0b00000000, 0b00000000, 0b00000000},  // W
    {0b01100011, 0b00010100, 0b00001000, 0b00010100, 0b01100011,0b00000000, 0b00000000, 0b00000000},  // X
    {0b00000111, 0b00001000, 0b01110000, 0b00001000, 0b00000111,0b00000000, 0b00000000, 0b00000000},  // Y
    {0b01100001, 0b01010001, 0b01001001, 0b01000101, 0b01000011,0b00000000, 0b00000000, 0b00000000},  // Z
    {0b01111111, 0b01000001, 0b00000000, 0b00000000, 0b00000000,0b00000000, 0b00000000, 0b00000000},  // [
    {0b00010101, 0b00010110, 0b01111100, 0b00010110, 0b00010101,0b00000000, 0b00000000, 0b00000000},  // back slash
    {0b00000000, 0b00000000, 0b00000000, 0b01000001, 0b01111111,0b00000000, 0b00000000, 0b00000000},  // ]
    {0b00000100, 0b00000010, 0b00000001, 0b00000010, 0b00000100,0b00000000, 0b00000000, 0b00000000},  // ^
    {0b01000000, 0b01000000, 0b01000000, 0b01000000, 0b01000000,0b00000000, 0b00000000, 0b00000000},  // _
    {0b00000000, 0b00000001, 0b00000010, 0b00000100, 0b00000000,0b00000000, 0b00000000, 0b00000000},  // `
    {0b00100000, 0b01010100, 0b01010100, 0b01010100, 0b01111000,0b00000000, 0b00000000, 0b00000000},  // a
    {0b01111111, 0b01001000, 0b01000100, 0b01000100, 0b00111000,0b00000000, 0b00000000, 0b00000000},  // b
    {0b00111000, 0b01000100, 0b01000100, 0b01000100, 0b00100000,0b00000000, 0b00000000, 0b00000000},  // c
    {0b00111000, 0b01000100, 0b01000100, 0b01001000, 0b01111111,0b00000000, 0b00000000, 0b00000000},  // d
    {0b00111000, 0b01010100, 0b01010100, 0b01010100, 0b00011000,0b00000000, 0b00000000, 0b00000000},  // e
    {0b00001000, 0b01111110, 0b00001001, 0b00000001, 0b00000010,0b00000000, 0b00000000, 0b00000000},  // f
    {0b00001100, 0b01010010, 0b01010010, 0b01010010, 0b00111110,0b00000000, 0b00000000, 0b00000000},  // g
    {0b01111111, 0b00001000, 0b00000100, 0b00000100, 0b01111000,0b00000000, 0b00000000, 0b00000000},  // h
    {0b00000000, 0b01000100, 0b01111101, 0b01000000, 0b00000000,0b00000000, 0b00000000, 0b00000000},  // i
    {0b00100000, 0b01000000, 0b01000100, 0b00111101, 0b00000000,0b00000000, 0b00000000, 0b00000000},  // j
    {0b01111111, 0b00010000, 0b00101000, 0b01000100, 0b00000000,0b00000000, 0b00000000, 0b00000000},  // k
    {0b00000000, 0b01000001, 0b01111111, 0b01000000, 0b00000000,0b00000000, 0b00000000, 0b00000000},  // l
    {0b01111100, 0b00000100, 0b00011000, 0b00000100, 0b01111000,0b00000000, 0b00000000, 0b00000000},  // m
    {0b01111100, 0b00001000, 0b00000100, 0b00000100, 0b01111000,0b00000000, 0b00000000, 0b00000000},  // n
    {0b00111000, 0b01000100, 0b01000100, 0b01000100, 0b00111000,0b00000000, 0b00000000, 0b00000000},  // o
    {0b01111100, 0b00010100, 0b00010100, 0b00010100, 0b00001000,0b00000000, 0b00000000, 0b00000000},  // p
    {0b00001000, 0b00010100, 0b00010100, 0b00011000, 0b01111100,0b00000000, 0b00000000, 0b00000000},  // q
    {0b01111100, 0b00001000, 0b00000100, 0b00000100, 0b00001000,0b00000000, 0b00000000, 0b00000000},  // r
    {0b01001000, 0b01010100, 0b01010100, 0b01010100, 0b00100000,0b00000000, 0b00000000, 0b00000000},  // s
    {0b00000100, 0b00111111, 0b01000100, 0b01000000, 0b00100000,0b00000000, 0b00000000, 0b00000000},  // t
    {0b00111100, 0b01000000, 0b01000000, 0b00100000, 0b01111100,0b00000000, 0b00000000, 0b00000000},  // u
    {0b00011100, 0b00100000, 0b01000000, 0b00100000, 0b00011100,0b00000000, 0b00000000, 0b00000000},  // v
    {0b00111100, 0b01000000, 0b00111000, 0b01000000, 0b00111100,0b00000000, 0b00000000, 0b00000000},  // w
    {0b01000100, 0b00101000, 0b00010000, 0b00101000, 0b01000100,0b00000000, 0b00000000, 0b00000000},  // x
    {0b00001100, 0b01010000, 0b01010000, 0b01010000, 0b00111100,0b00000000, 0b00000000, 0b00000000},  // y
    {0b01000100, 0b01100100, 0b01010100, 0b01001100, 0b01000100,0b00000000, 0b00000000, 0b00000000},  // z
    {0b00000000, 0b00001000, 0b00110110, 0b01000001, 0b00000000,0b00000000, 0b00000000, 0b00000000},  // {
    {0b00000000, 0b00000000, 0b01111111, 0b00000000, 0b00000000,0b00000000, 0b00000000, 0b00000000},  // |
    {0b00000000, 0b01000001, 0b00110110, 0b00001000, 0b00000000,0b00000000, 0b00000000, 0b00000000},  // }
    {0b00001000, 0b00001000, 0b00101010, 0b00011100, 0b00001000,0b00000000, 0b00000000, 0b00000000},  // ~
    {0b00001000, 0b00011100, 0b00101010, 0b00001000, 0b00001000,0b00000000, 0b00000000, 0b00000000}   // <-
};




/*** Call this function to boost the STM32F0xx clock to 48 MHz ***/

void SystemClock48MHz( void )
{
//
// Disable the PLL
//
    RCC->CR &= ~(RCC_CR_PLLON);
//
// Wait for the PLL to unlock
//
    while (( RCC->CR & RCC_CR_PLLRDY ) != 0 );
//
// Configure the PLL for 48-MHz system clock
//
    RCC->CFGR = 0x00280000;
//
// Enable the PLL
//
    RCC->CR |= RCC_CR_PLLON;
//
// Wait for the PLL to lock
//
    while (( RCC->CR & RCC_CR_PLLRDY ) != RCC_CR_PLLRDY );
//
// Switch the processor to the PLL clock source
//
    RCC->CFGR = ( RCC->CFGR & (~RCC_CFGR_SW_Msk)) | RCC_CFGR_SW_PLL;
//
// Update the system with the new clock frequency
//
    SystemCoreClockUpdate();

}

/*****************************************************************/


int main(int argc, char* argv[])
{

	SystemClock48MHz();

	trace_printf("System clock: %u Hz\n", SystemCoreClock);

	myGPIO_Init();		/* Initialize I/O port PA */
	myTIM2_Init();		/* Initialize timer TIM2 */
	myTIM3_Init();		/* Initialize timer TIM3 */
	myEXTI_Init();		/* Initialize EXTI */
	myADC_Init();       /* Initialize ADC */
    myDAC_Init();       /* Initialize DAC */
	oled_config();      /* Initialize OLED */


	while (1)
	{
        ADC1->CR |= ADC_CR_ADSTART; //start adc conversion
        if (ADC1->ISR & ADC_ISR_EOC) //check if conversion is complete
        {
            ADC1->ISR |= ADC_ISR_EOC; //clear EOC flag
            adcVal = ADC1->DR; //read adc value
            DAC->DHR12R1 = adcVal; //write adc value to DAC
            res = 5000*adcVal/ADC_SCALE; //convert adc value to resistance
        }
        refresh_OLED();
        //for ( long i = 0; i < 200000; i++ );
	}

	return 0;

}



void myGPIO_Init()
{
    /*===========================GPIOA Inits=============================*/
	/* Enable clock for GPIOA peripheral */
	// Relevant register: RCC->AHBENR
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN; //enable gpioa clock

	/*Configure PA0, PA1, and PA2 as input*/
	/* Configure PA5 and PA4 as analog*/
	// Relevant register: GPIOA->MODER
    // Clear the MODER bits for PA0, PA1, PA2, PA4, and PA5
     GPIOA->MODER &= ~(GPIO_MODER_MODER0 | GPIO_MODER_MODER1 | GPIO_MODER_MODER2 | GPIO_MODER_MODER4 | GPIO_MODER_MODER5);
     GPIOA->MODER &= ~(GPIO_MODER_MODER0);
     GPIOA->MODER &= ~(GPIO_MODER_MODER1);
     GPIOA->MODER &= ~(GPIO_MODER_MODER2);
     GPIOA->MODER |= (GPIO_MODER_MODER4);
     GPIOA->MODER |= (GPIO_MODER_MODER5);
	/*Ensure no pull-up/pull-down for PA0-5*/
	// Relevant register: GPIOA->PUPDR
     GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPDR0);
     GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPDR1);
	 GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPDR2);
     GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPDR4);
     GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPDR5);

    /*===========================GPIOB Inits=============================*/
    /* Enable clock for GPIOB peripheral */
	// Relevant register: RCC->AHBENR
	RCC->AHBENR |= RCC_AHBENR_GPIOBEN;

	/*Configure PB4-7 as output, PB3 and 5 as AF0*/
	// Relevant register: GPIOB->MODER
    // Clear the MODER bits for PB3-7
     GPIOB->MODER &= ~(GPIO_MODER_MODER3 | GPIO_MODER_MODER4 | GPIO_MODER_MODER5 | GPIO_MODER_MODER6 | GPIO_MODER_MODER7);

     GPIOB->MODER |= (GPIO_MODER_MODER3_1);
     GPIOB->MODER |= (GPIO_MODER_MODER4_0);
     GPIOB->MODER |= (GPIO_MODER_MODER5_1);
     GPIOB->MODER |= (GPIO_MODER_MODER6_0);
     GPIOB->MODER |= (GPIO_MODER_MODER7_0);
     GPIOB->AFR[0] &= ~(GPIO_AFRL_AFRL3);
     GPIOB->AFR[0] &= ~(GPIO_AFRL_AFRL5);

	/*Ensure no pull-up/pull-down for PB3-7*/
    GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPDR3);
    GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPDR4);
	GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPDR5);
    GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPDR6);
    GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPDR7);

    /*===========================SPI1 Inits=============================*/
    /*Enable clock for SPI1*/
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
}


void myTIM2_Init()
{
	/* Enable clock for TIM2 peripheral */
	// Relevant register: RCC->APB1ENR
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

	/* Configure TIM2: buffer auto-reload, count up, stop on overflow,
	 * enable update events, interrupt on overflow only */
	// Relevant register: TIM2->CR1
	TIM2->CR1 = ((uint16_t)0x008C);

	/* Set clock pre-scaler value */
	TIM2->PSC = myTIM2_PRESCALER;

	/* Set auto-reloaded delay */
	TIM2->ARR = myTIM2_PERIOD;

	/* Update timer registers */
	// Relevant register: TIM2->EGR
	TIM2->EGR = ((uint16_t)0x0001);

	/* Assign TIM2 interrupt priority = 0 in NVIC */
	// Relevant register: NVIC->IP[3], or use NVIC_SetPriority
	NVIC_SetPriority(TIM2_IRQn, 0);

	/* Enable TIM2 interrupts in NVIC */
	// Relevant register: NVIC->ISER[0], or use NVIC_EnableIRQ
	NVIC_EnableIRQ(TIM2_IRQn);

	/* Enable update interrupt generation */
	// Relevant register: TIM2->DIER
	TIM2->DIER |= TIM_DIER_UIE;
}


void myTIM3_Init(){
	/* Enable clock for TIM2 peripheral */
	// Relevant register: RCC->APB1ENR
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

	/* Configure TIM3: buffer auto-reload, count up, stop on overflow,
	 * enable update events, interrupt on overflow only */
	// Relevant register: TIM3->CR1
	TIM3->CR1 = ((uint16_t)0x008C);

	/* Set clock prescaler value */
	TIM3->PSC = myTIM3_PRESCALER;

	/* Set auto-reloaded delay */
	TIM3->ARR = myTIM3_PERIOD;

	/* Update timer registers */
	// Relevant register: TIM2->EGR
	TIM3->EGR = ((uint16_t)0x0001);

	/* Enable update interrupt generation */
	// Relevant register: TIM3->DIER
	TIM3->DIER |= TIM_DIER_UIE;
}


void TIM3_Delay(uint32_t delay){
    TIM3->CNT = delay; //set delay
    TIM3->CR1 |= TIM_CR1_CEN; //start timer
    while((TIM3->SR & TIM_SR_UIF) == 0);  //wait for delay
    TIM3->CR1 &= ~TIM_CR1_CEN; //stop timer
    TIM3->SR &= ~TIM_SR_UIF; //clear interrupt flag

}

void myADC_Init(){
    // Enable the ADC clock
    RCC->APB2ENR |= RCC_APB2ENR_ADCEN;
    ADC1->CFGR1 &= ~ADC_CFGR1_RES;
    ADC1->CFGR1 &= ~ADC_CFGR1_ALIGN;

    // Set sample time
    ADC1->SMPR |= ADC_SMPR_SMP;

    // Select ADC channel 5
    ADC1->CHSELR |= ADC_CHSELR_CHSEL5;

    // Enable ADC by setting ADEN in the CR register
    ADC1->CR |= ADC_CR_ADEN;
}

void myDAC_Init(){
    // Enable clock for DAC peripheral
    RCC->APB1ENR |= RCC_APB1ENR_DACEN;

    // Enable DAC channel 1 by setting EN1 in the DAC control register (DAC->CR)
    DAC->CR |= DAC_CR_EN1;
}

void myEXTI_Init()
{
	/* Map EXTI2 and EXTI0 line to PA2 and PA0 respectively */
	// Relevant register: SYSCFG -> EXTICR[0]
	//SYSCFG->EXTICR[0] &= 0xF0F0;
    // Map EXTI2, EXTI0 and EXTI 1 to PA2, PA0 and PA1 respectively
    SYSCFG->EXTICR[0] &= 0xF000;

	/* EXTI2, EXTI1 and EXTI0 line interrupts: set rising-edge trigger */
	// Relevant register: EXTI->RTSR
	EXTI->RTSR |= 0x07;

	/* Unmask interrupts from EXTI2, EXTI1, and EXTI0 line */
	// Relevant register: EXTI->IMR
	EXTI-> IMR &= ~(EXTI_IMR_MR0 | EXTI_IMR_MR1 | EXTI_IMR_MR2);
	EXTI-> IMR |= (EXTI_IMR_MR0 | EXTI_IMR_MR2);

	/* Assign EXTI2 interrupt priority = 0 in NVIC */
	// Relevant register: NVIC->IP[2], or use NVIC_SetPriority
	NVIC_SetPriority(EXTI2_3_IRQn, 1);
	/* Enable EXTI2 interrupts in NVIC */
	// Relevant register: NVIC->ISER[0], or use NVIC_EnableIRQ
    NVIC_EnableIRQ(EXTI2_3_IRQn);

	/* Assign EXTI0 interrupt priority = 0 in NVIC */
	// Relevant register: NVIC->IP[2], or use NVIC_SetPriority
	NVIC_SetPriority(EXTI0_1_IRQn, 0);
	/* Enable EXTI0 interrupts in NVIC */
	// Relevant register: NVIC->ISER[0], or use NVIC_EnableIRQ
    NVIC_EnableIRQ(EXTI0_1_IRQn);
}


/* This handler is declared in system/src/cmsis/vectors_stm32f051x8.c */
void TIM2_IRQHandler()
{
	/* Check if update interrupt flag is indeed set */
	if ((TIM2->SR & TIM_SR_UIF) != 0)
	{
		trace_printf("\n*** Overflow! ***\n");

		/* Clear update interrupt flag */
		// Relevant register: TIM2->SR
		TIM2->SR &= ~TIM_SR_UIF;


		/* Restart stopped timer */
		// Relevant register: TIM2->CR1
		TIM2->CR1 |= TIM_CR1_CEN;
	}
}


void oled_config( void )
{

    // Don't forget to enable GPIOB clock in RCC
    // Don't forget to configure PB3/PB5 as AF0
    // Don't forget to enable SPI1 clock in RCC

    SPI_Handle.Instance = SPI1;

    SPI_Handle.Init.Direction = SPI_DIRECTION_1LINE;
    SPI_Handle.Init.Mode = SPI_MODE_MASTER;
    SPI_Handle.Init.DataSize = SPI_DATASIZE_8BIT;
    SPI_Handle.Init.CLKPolarity = SPI_POLARITY_LOW;
    SPI_Handle.Init.CLKPhase = SPI_PHASE_1EDGE;
    SPI_Handle.Init.NSS = SPI_NSS_SOFT;
    SPI_Handle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
    SPI_Handle.Init.FirstBit = SPI_FIRSTBIT_MSB;
    SPI_Handle.Init.CRCPolynomial = 7;

    //
    // Initialize the SPI interface
    //
    HAL_SPI_Init( &SPI_Handle );

    //
    // Enable the SPI
    //
    __HAL_SPI_ENABLE( &SPI_Handle );


    /* Reset LED Display (RES# = PB4):
    - make pin PB4 = 0, wait for a few ms
    - make pin PB4 = 1, wait for a few ms
    */
    /*Saleh's Solution*/
    GPIOB->ODR &= ~GPIO_ODR_4;
    TIM3_Delay(5);

    GPIOB->ODR |= GPIO_ODR_4;
    TIM3_Delay(5);

    //
    // Send initialization commands to LED Display
    //
    for ( unsigned int i = 0; i < sizeof( oled_init_cmds ); i++ )
    {
    oled_Write_Cmd( oled_init_cmds[i] );
    }


    /* Fill LED Display data memory (GDDRAM) with zeros:
    - for each PAGE = 0, 1, ..., 7
    set starting SEG = 0
    call oled_Write_Data( 0x00 ) 128 times
    */

    for (unsigned int page = 0; page < 8; page++){
        oled_Write_Cmd(0xB0 + page);
        oled_Write_Cmd(0x02);
        oled_Write_Cmd(0x10);
        for (int seg = 0; seg < 128; seg++){
            oled_Write_Data(0x00);
        }
    }
}

void oled_Write_Cmd( unsigned char cmd )
{
     // make PB6 = CS# = 1
    GPIOB->ODR |= GPIO_ODR_6;
     // make PB7 = D/C# = 0
    GPIOB->ODR &= ~GPIO_ODR_7;
     // make PB6 = CS# = 0
    GPIOB->ODR &= ~GPIO_ODR_6;
    oled_Write( cmd );
     // make PB6 = CS# = 1
    GPIOB->ODR |= GPIO_ODR_6;
}

void oled_Write_Data( unsigned char data )
{
    // make PB6 = CS# = 1
    GPIOB->ODR |= GPIO_ODR_6;
    // make PB7 = D/C# = 1
    GPIOB->ODR |= GPIO_ODR_7;
    // make PB6 = CS# = 0
    GPIOB->ODR &= ~GPIO_ODR_6;
    oled_Write( data );
    // make PB6 = CS# = 1
    GPIOB->ODR |= GPIO_ODR_6;
}

void oled_Write( unsigned char Value )
{

    /* Wait until SPI1 is ready for writing (TXE = 1 in SPI1_SR) */

    while( ( SPI1->SR & SPI_SR_TXE ) == 0 );


    /* Send one 8-bit character:
       - This function also sets BIDIOE = 1 in SPI1_CR1
    */
    HAL_SPI_Transmit( &SPI_Handle, &Value, 1, HAL_MAX_DELAY );


    /* Wait until transmission is complete (TXE = 1 in SPI1_SR) */
    while( ( SPI1->SR & SPI_SR_TXE ) == 0 );

}


void refresh_OLED( void )
{
    // Buffer size = at most 16 characters per PAGE + terminating '\0'
    unsigned char Buffer[17];

    snprintf( Buffer, sizeof( Buffer ), "R: %5u Ohms", res );
    /* Buffer now contains your character ASCII codes for LED Display
       - select PAGE (LED Display line) and set starting SEG (column)
       - for each c = ASCII code = Buffer[0], Buffer[1], ...,
           send 8 bytes in Characters[c][0-7] to LED Display
    */
    oled_Write_Cmd(0xB2);
    oled_Write_Cmd(0x05);
    oled_Write_Cmd(0x10);

    for (int i = 0; i < 16; i++){
        if(Buffer[i] == 0){
            break;
        }
        for (int j = 0; j < 8; j++){
            oled_Write_Data(Characters[Buffer[i]][j]);
        }
    }

    snprintf( Buffer, sizeof( Buffer ), "F: %5u Hz", frequency );
    /* Buffer now contains your character ASCII codes for LED Display
       - select PAGE (LED Display line) and set starting SEG (column)
       - for each c = ASCII code = Buffer[0], Buffer[1], ...,
           send 8 bytes in Characters[c][0-7] to LED Display
    */

    oled_Write_Cmd(0xB4);
    oled_Write_Cmd(0x05);
    oled_Write_Cmd(0x10);
    for (int i = 0; i < 16; i++){
        if(Buffer[i] == 0){
            break;
        }
        for (int j = 0; j < 8; j++){
            oled_Write_Data(Characters[Buffer[i]][j]);
        }
    }

	/* Wait for ~100 ms (for example) to get ~10 frames/sec refresh rate
       - You should use TIM3 to implement this delay (e.g., via polling)
    */
    TIM3_Delay(100);
}

void EXTI0_1_IRQHandler()
{
	/* Check if EXTI0 interrupt pending flag is indeed set */
	if((EXTI->PR & EXTI_PR_PR1) != 0){
		if((TIM2->CR1 & TIM_CR1_CEN) == 0){
			TIM2->CNT = 0;
			TIM2->CR1 |= TIM_CR1_CEN;
		}
		else{
			TIM2->CR1 &= ~TIM_CR1_CEN;
			count = TIM2->CNT;
			period = (float)count/(float)SystemCoreClock;
            frequency = 1/period;
		}
		EXTI->PR |= EXTI_PR_PR1;
	}

	/* Check if EXTI0 interrupt pending flag is indeed set */
	if ((EXTI->PR & EXTI_PR_PR0) != 0){
		/* IDR = Input Data Register - Check if button is pressed (PA0 = 1) */
		if((GPIOA->IDR & GPIO_IDR_0) != 0){
			if (inSig == 0){
				inSig = 1;
				EXTI-> IMR &= ~(EXTI_IMR_MR1); /*Disable and EXTI1*/
				EXTI-> IMR |= EXTI_IMR_MR2; /*Enable EXTI2*/
				trace_printf("Function generator enabled\n");
			}
			else{
				inSig = 0;
				EXTI-> IMR &= ~(EXTI_IMR_MR2); /*Disable EXTI2*/
				EXTI-> IMR |= EXTI_IMR_MR1; /*Enable EXTI1*/
				trace_printf("NE555 Enabled\n");
			}
			/* Clear Pending Register for User Button by setting it to 1 */
			EXTI->PR |= EXTI_PR_PR0;
		}

	}
}


/* This handler is declared in system/src/cmsis/vectors_stm32f051x8.c */
void EXTI2_3_IRQHandler()
{

	/* Check if EXTI2 interrupt pending flag is indeed set */
	if ((EXTI->PR & EXTI_PR_PR2) != 0)
	{
		//
		// 1. If this is the first edge:
		//	- Clear count register (TIM2->CNT).
		//	- Start timer (TIM2->CR1).
		//    Else (this is the second edge):
		//	- Stop timer (TIM2->CR1).
		//	- Read out count register (TIM2->CNT).
		//	- Calculate signal period and frequency.
		//	- Print calculated values to the console.
		//	  NOTE: Function trace_printf does not work
		//	  with floating-point numbers: you must use
		//	  "unsigned int" type to print your signal
		//	  period and frequency.
		//
		if((TIM2->CR1 & TIM_CR1_CEN) == 0){
			TIM2->CNT = 0;
			TIM2->CR1 |= TIM_CR1_CEN;
		}
		else{
            TIM2->CR1 &= ~TIM_CR1_CEN;
            count = TIM2->CNT;
            period = (float)count/(float)SystemCoreClock;
            frequency = 1/period;

		}
		// 2. Clear EXTI2 interrupt pending flag (EXTI->PR).
		// NOTE: A pending register (PR) bit is cleared
		// by writing 1 to it.
		//
		EXTI->PR |= EXTI_PR_PR2;
	}
}




#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
