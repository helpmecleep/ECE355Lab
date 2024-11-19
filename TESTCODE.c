// the final final final projecttttt for realllll
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

#define myTIM3_PRESCALER ((uint16_t)0x0000)
#define myTIM3_PERIOD ((uint32_t)0xFFFFFFFF)

void myGPIO_Init(void);
void myTIM2_Init(void);
void myTIM3_Init(void);
void myEXTI_Init(void);
void myADC_Init(void);
void myDAC_Init(void);

void oled_Write(unsigned char);
void oled_Write_Cmd(unsigned char);
void oled_Write_Data(unsigned char);
void oled_config(void);
void refresh_OLED(void);

int inSig = 1;
int firstedge = 1;
float count;
float period;
unsigned int frequency = 0;  // Example: measured frequency value (global variable)
unsigned int Res = 0;   // Example: measured resistance value (global variable)

SPI_HandleTypeDef SPI_Handle;


// LED Display initialization commands

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



// Character specifications for LED Display (1 row = 8 bytes = 1 ASCII character)
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

void SystemClock48MHz( void ){

// Disable the PLL
    RCC->CR &= ~(RCC_CR_PLLON);

// Wait for the PLL to unlock
    while (( RCC->CR & RCC_CR_PLLRDY ) != 0 );

// Configure the PLL for 48-MHz system clock
    RCC->CFGR = 0x00280000;

// Enable the PLL
    RCC->CR |= RCC_CR_PLLON;

// Wait for the PLL to lock
    while (( RCC->CR & RCC_CR_PLLRDY ) != RCC_CR_PLLRDY );

// Switch the processor to the PLL clock source
    RCC->CFGR = ( RCC->CFGR & (~RCC_CFGR_SW_Msk)) | RCC_CFGR_SW_PLL;

// Update the system with the new clock frequency
    SystemCoreClockUpdate();

}

/*****************************************************************/


int main(int argc, char* argv[]){

	SystemClock48MHz();

	myGPIO_Init();		/* Initialize I/O port PA */
	myTIM2_Init();		/* Initialize timer TIM2 */
	myEXTI_Init();		/* Initialize EXTI */
	myADC_Init();		/*Initialize ADC */
	myDAC_Init();		/*Initialize DAC */
	oled_config();		/*Configure OLED */

	while (1){
		ADC1->CR |= 0x4;
		if (((ADC1->ISR) & 0x4) != 0){ // check if channel conversion is complete
			Res = 5000* (ADC1->DR) / 4095; //calculate resistance from adc data
			DAC->DHR12R1 = ADC1->DR; //data from adc is sent to dac
		}
		refresh_OLED();
	}

	return 0;
}

/*Initializations *///----------------------------------------------------------------------------
void myGPIO_Init(){
	/* Enable clock for GPIOA peripheral */
	// Relevant register: RCC->AHBENR
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN; //enable gpioa clock
	RCC->AHBENR |= RCC_AHBENR_GPIOBEN; //enable gpiob clock
	RCC->APB2ENR |= RCC_APB2ENR_ADCEN; // enable adc clock
	RCC->APB1ENR |= RCC_APB1ENR_DACEN; //enable DAC clock
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN; //enable SPI clock

	/* Configure PA2 and 0 as input */
	// Relevant register: GPIOA->MODER
	GPIOA->MODER &= ~(GPIO_MODER_MODER2); //configure function generator as input
	GPIOA->MODER &= ~(GPIO_MODER_MODER1); //configure 555 timer as input
	GPIOA->MODER &= ~(GPIO_MODER_MODER0); //configure push button as input
	GPIOA->MODER |= (GPIO_MODER_MODER5); //PA5 as analog

	//GPIOB configuration PB3 and PB5
	GPIOB->MODER |= 0x880;
	//trying to configure to af0 not workingggg GPIOB->AFRL  = 0x0;
	GPIOB->MODER |= 0x5100; //configure PB4, PB 6,7

	/* Ensure no pull-up/pull-down for PA2 and 0*/
	// Relevant register: GPIOA->PUPDR
	GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPDR2);
	GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPDR0);
    GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPDR1);
    GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPDR5);

    GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPDR3);
    GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPDR4);
    GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPDR5);
    GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPDR6);
    GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPDR7);
}

void myTIM2_Init(){
	/* Enable clock for TIM2 peripheral */
	// Relevant register: RCC->APB1ENR
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

	/* Configure TIM2: buffer auto-reload, count up, stop on overflow,
	 * enable update events, interrupt on overflow only */
	// Relevant register: TIM2->CR1
	TIM2->CR1 = ((uint16_t)0x008C);

	/* Set clock prescaler value */
	TIM2->PSC = myTIM2_PRESCALER;
	/* Set auto-reloaded delay */
	TIM2->ARR = myTIM2_PERIOD;

	/* Update timer registers */
	// Relevant register: TIM2->EGR
	TIM2->EGR = ((uint16_t)0x0001);

	/* Assign TIM2 interrupt priority = 0 in NVIC */
	// Relevant register: NVIC->IP[3], or use NVIC_SetPriority
	NVIC_SetPriority(TIM2_IRQn, 2);
	// Same as: NVIC->IP[3] = ((uint32_t)0x00FFFFFF);

	/* Enable TIM2 interrupts in NVIC */
	// Relevant register: NVIC->ISER[0], or use NVIC_EnableIRQ
	NVIC_EnableIRQ(TIM2_IRQn);
		// Same as: NVIC->ISER[0] = ((uint32_t)0x00008000) */

	/* Enable update interrupt generation */
	// Relevant register: TIM2->DIER
	TIM2->DIER |= TIM_DIER_UIE;
}

void myTIM3_Init(){
	/* Enable clock for TIM3 peripheral */
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

	/* Assign TIM3 interrupt priority = 0 in NVIC */
	NVIC_SetPriority(TIM3_IRQn, 3); //<< QQ what would be the priority for TIM3

	/* Enable TIM3 interrupts in NVIC */
	NVIC_EnableIRQ(TIM3_IRQn);

	/* Enable update interrupt generation */
	// Relevant register: TIM2->DIER
	TIM3->DIER |= TIM_DIER_UIE;
}

void myEXTI_Init(){
	/* Map EXTI2 line to PA2 */
	// Relevant register: SYSCFG->EXTICR[0]
	SYSCFG->EXTICR[0] = 0x00000000;

	/* EXTI2 1 and 0 line interrupts: set rising-edge trigger */
	// Relevant register: EXTI->RTSR
	EXTI->RTSR = 0x0007;

	/* Unmask interrupts from EXTI2 1 and 0 line */
	// Relevant register: EXTI->IMR
	EXTI->IMR = 0x0005;

	/* Assign EXTI2 interrupt priority = 0 in NVIC */
	// Relevant register: NVIC->IP[2], or use NVIC_SetPriority
	NVIC_SetPriority(EXTI2_3_IRQn, 1);
	NVIC_SetPriority(EXTI0_1_IRQn, 0);

	/* Enable EXTI2 interrupts in NVIC */
	// Relevant register: NVIC->ISER[0], or use NVIC_EnableIRQ
	NVIC_EnableIRQ(EXTI0_1_IRQn);
	NVIC_EnableIRQ(EXTI2_3_IRQn);
}

void myADC_Init(){
	ADC1->CFGR1 = 0x3000;
	//float hello = ADC1->CFGR1;
		//trace_printf("%f",hello);
	ADC1->SMPR = 0x7;
	ADC1->CHSELR = 0x20;
	ADC1->CR = 0x1;
	//trace_printf("%f",ADC1->CR);
	//trace_printf("ddd:");
	while((ADC1->ISR) ==0);
	//trace_printf("deee:");

}

void myDAC_Init(){
	DAC->CR = 0x1; //channel 1 enable (PA4), enable tri-state buffer and disable channel 1 trigger
}

// LED Display Functions
// OLED initialization: 1.initialize SPI 2.enable SPI 3.Reset display 4.Send init to oled cmd 5.Clear Display
void oled_config( void ){

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

// Initialize the SPI interface
    HAL_SPI_Init( &SPI_Handle );

// Enable the SPI
    __HAL_SPI_ENABLE( &SPI_Handle );

    /* Reset LED Display (RES# = PB4):
       - make pin PB4 = 0, wait for a few ms
       - make pin PB4 = 1, wait for a few ms*/

    TIM3->CNT= 480000;         //set TIM3 to 480000 (10ms)
    TIM3->CR1 |= TIM_CR1_CEN;   //start timer

    GPIOB->ODR |= 0x00;         // 1.activate reset output data register for PB4, make pin PB4 = 0 , RES# active low

    while(TIM3->CNT != 0);      // wait until timer reaches zero
    TIM3->CR1 |= TIM_CR1_UDIS;  //Stop timer, UDIS update disable

    TIM3->CR1 |= TIM_CR1_CEN;   //Start timer TIM3, CEN counter enable

    GPIOB->ODR |= 0x10;         // 2. make pin PB4 = 1

    while(TIM3->CNT != 0);      // wait until timer reaches zero
    TIM3->CR1 |= TIM_CR1_UDIS;  //Stop timer, UDIS update disable

// Send initialization commands to LED Display

    for ( unsigned int i = 0; i < sizeof( oled_init_cmds ); i++ ){
        oled_Write_Cmd( oled_init_cmds[i] );
    }

    /* Fill LED Display data memory (GDDRAM) with zeros:
       - for each PAGE = 0, 1, ..., 7
           set starting SEG = 0
           call oled_Write_Data( 0x00 ) 128 times*/

    for(int n=0; n<8; n++){
    	oled_Write_Cmd(176+n);	//select page
        oled_Write_Cmd(0x02);	//select segment lower half
        oled_Write_Cmd(0x10); 	//select segment upper half

        for (int i=0; i<128; i++){
        	oled_Write_Data(0x00);
        }
    }

}

void oled_Write_Cmd( unsigned char cmd ){
	GPIOB->ODR |= 0x40; // make PB6 = CS# = 1
	GPIOB->ODR &= ~0x80; // make PB7 = D/C# = 0
	GPIOB->ODR &= ~0x40; // make PB6 = CS# = 0

    oled_Write( cmd );
    GPIOB->ODR |= 0x40; // make PB6 = CS# = 1

}

void oled_Write_Data( unsigned char data ){
	GPIOB->ODR |= 0x40; // make PB6 = CS# = 1
	GPIOB->ODR |= 0x80; // make PB7 = D/C# = 1
	GPIOB->ODR &= ~0x40; // make PB6 = CS# = 0
    oled_Write( data );
    GPIOB->ODR |= 0x40; // make PB6 = CS# = 1
}

void oled_Write( unsigned char Value ){

    /* Wait until SPI1 is ready for writing (TXE = 1 in SPI1_SR) */

    while((SPI1->SR && 0x2) == 0);

    /* Send one 8-bit character:
       - This function also sets BIDIOE = 1 in SPI1_CR1
    */
    HAL_SPI_Transmit( &SPI_Handle, &Value, 1, HAL_MAX_DELAY );


    /* Wait until transmission is complete (TXE = 1 in SPI1_SR) */

    while((SPI1->SR && 0x2) == 0);

}

void refresh_OLED( void ){
    // Buffer size = at most 16 characters per PAGE + terminating '\0'
    char Buffer[17];
    snprintf( Buffer, sizeof( Buffer ), "R: %5u Ohms", Res );
    /* Buffer now contains your character ASCII codes for LED Display
       - select PAGE (LED Display line) and set starting SEG (column)
       - for each c = ASCII code = Buffer[0], Buffer[1], ...,
           send 8 bytes in Characters[c][0-7] to LED Display
    */

    oled_Write_Cmd(0xB2);
    oled_Write_Cmd(0x05);
    oled_Write_Cmd(0x10);

    for (int i=0; i<16; i++){
    	if(Buffer[i]== 0){
    	    break;
    	}
    	for(int j=0; j<8; j++){
    		oled_Write_Data(Characters[(unsigned char)Buffer[i]][j]); //write the 8 bytes for the character to the oled

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

    for (int i=0; i<16; i++){
    	if(Buffer[i]== 0){
    		break;
    	}
        	for(int j=0; j<8; j++){
        		oled_Write_Data(Characters[(unsigned char)Buffer[i]][j]); //write the 8 bytes for the character to the oled

        	}
        }

	/* Wait for ~100 ms (for example) to get ~10 frames/sec refresh rate
       - You should use TIM3 to implement this delay (e.g., via polling)
    */

        //set TIM3 to 480000
        TIM3->CNT= 4800000;
        // when need to make sure counter is counting down not up, TIM3->CR1 |= 0x10;
        //start timer
        TIM3->CR1 |= TIM_CR1_CEN;   //Start timer TIM3, CEN counter enable
        // wain until timer reaches zero
        while(TIM3->CNT != 0);
        //stop timer
        TIM3->CR1 |= TIM_CR1_UDIS;  //Stop timer , UDIS update disable
       // TIM3->CNT= 0x0;

}

/*Interrupts*/ //----------------------------------------------------------------------------

void TIM2_IRQHandler(){
	/* Check if update interrupt flag is indeed set */
	if ((TIM2->SR & TIM_SR_UIF) != 0)
	{
		trace_printf("\n*** Overflow! ***\n");

		/* Clear update interrupt flag */
		// Relevant register: TIM2->SR
		TIM2->SR &= ~(TIM_SR_UIF);

		/* Restart stopped timer */
		// Relevant register: TIM2->CR1
		TIM2->CR1 |= TIM_CR1_CEN;
	}
}

void EXTI0_1_IRQHandler(){
	/* Check if EXTI0 interrupt pending flag is indeed set */
		if ((EXTI->PR & EXTI_PR_PR1) != 0){ //interupts for ne355 timer
			//This code section is for measuring frequency on EXTI1 when inSig=0
				if(firstedge==1){
						//	- Clear count register (TIM2->CNT).
							TIM2->CNT = 0x0;

						//	- Start timer (TIM2->CR1).
							TIM2->CR1 |= TIM_CR1_CEN;
							firstedge = 0;
						}else{
						//    Else (this is the second edge):
						//	- Stop timer (TIM2->CR1).
							TIM2->CR1 |= TIM_CR1_UDIS;
						//	- Read out count register (TIM2->CNT).
							count = TIM2->CNT;
						//	- Calculate signal period and frequency.
							period = count/48000000;
							frequency = 1/period;
							firstedge = 1;
						}

			EXTI->PR = 0x2;
		}

		if ((EXTI->PR & EXTI_PR_PR0) != 0){//if button is pressed
			if (inSig == 0){
				inSig = 1;
				EXTI->IMR = 0x0005; //disable EXTI1, enable EXTI2 interrupts
				trace_printf("button1\n");
			}else{
				inSig = 0;
				EXTI->IMR = 0x0003; //disable EXTI2, enable EXTI1 interrupts
				trace_printf("button2\n");
			}
			EXTI->PR = 0x1;
		}
}

void EXTI2_3_IRQHandler(){
	/* Check if EXTI2 interrupt pending flag is indeed set */
	if ((EXTI->PR & EXTI_PR_PR2) != 0)
	{
		//
		// 1. If this is the first edge:
		if(firstedge==1){
		//	- Clear count register (TIM2->CNT).
			TIM2->CNT = 0x0;

		//	- Start timer (TIM2->CR1).
			TIM2->CR1 |= TIM_CR1_CEN;
			firstedge = 0;
		}else{
		//    Else (this is the second edge):
		//	- Stop timer (TIM2->CR1).
			TIM2->CR1 |= TIM_CR1_UDIS;
		//	- Read out count register (TIM2->CNT).
			count = TIM2->CNT;
		//	- Calculate signal period and frequency.
			period = count/48000000;
			frequency = 1/period;

			firstedge = 1;

		}

		EXTI->PR = 0x4; // clear EXTI interrupt pending flag by writing 1 to it
	}
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
