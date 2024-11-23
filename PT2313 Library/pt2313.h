/**
 * @file     pt2313.h
 * @author   HS6502
 * @version  1.4
 * @date     11-Sep-2024  Updated : 21-Nov-2024
 * @brief    Prototypes and Macros for PT2313 Library
 */

#ifndef  __PT2313_H
#define  __PT2313_H
#include "sfl_stm8s_i2c.h"

#define PT2313_ADDR 0x44
#define ON          1
#define OFF         0
/* Registers : Vol = 0x00; FL = 0x80; FR = 0xA0; RL = 0xC0; RR = 0xE0; BASS = 0x60; TREBLE = 0x70 */
#ifndef PT2313_Default_Volume
    #define PT2313_Default_Volume 20
#endif
#ifndef PT2313_Default_Input
    #define PT2313_Default_Input  1
#endif

void PT2313_Write(uint8_t Data);                // Internal Function for Write Data to PT2313
int8_t Check_Parameters (int8_t Value, int8_t Max, int8_t Min); /* Internal Function for Correct 
        Under or Over Values*/
void PT2313_Volume (uint8_t Volume);            // Set Volume
void PT2313_Bass (int8_t Bass);                 // Set Bass
void PT2313_Treble (int8_t Treble);             // Set Treble
void PT2313_Balance (int8_t Balance);           // Set Output Balance
void PT2313_Mute (bool Mute_State);             // Mute or UnMute Output
void PT2313_Input (uint8_t Input);              // Select Audio Input
void PT2313_Gain (uint8_t Gain);                // Select Audio Output Boost Value
void PT2313_Loudness (bool Loudness_Status);    // Enable or Disable Loudness Sound
void PT2313_Reset (void);                       // Reset PT2313 Values to Default Value

#endif