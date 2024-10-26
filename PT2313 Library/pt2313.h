/**
 * @file     pt2313.h
 * @author   HS6502
 * @version  1.2.0
 * @date     11-Sep-2024
 * @brief    Prototypes and Macros for PT2313 Library
 */

#ifndef  PT2313_H
#define  PT2313_H
#include <sfl_stm8s_i2c.h>

#define PT2313_ADDR 0x88
#define ON          1
#define OFF         0

#ifndef PT2313_Default_Volume
    #define PT2313_Default_Volume 20
#endif
#ifndef PT2313_Default_Input
    #define PT2313_Default_Input  1
#endif

void PT2313_Write(uint8_t Data);
int8_t Check_Parameters (int8_t Value, int8_t Max, int8_t Min);

void PT2313_Volume (uint8_t Volume);
void PT2313_Bass (int8_t Bass);
void PT2313_Treble (int8_t Treble);
void PT2313_Balance (int8_t Balance);
void PT2313_Mute (bool Mute_State);
void PT2313_Input (uint8_t Input);
void PT2313_Gain (uint8_t Gain);
void PT2313_Loudness (bool Loudness_Status);
void PT2313_Reset (void);

#endif