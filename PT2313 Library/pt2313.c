/**
 * @file     pt2313.c
 * @author   HS6502
 * @version  V2.0
 * @date     23-Oct-2025  || First Version : 11-Sep-2024
 * @brief    PT2313 Audio Processor Library for STM8 
 */
#include "pt2313.h"

static uint8_t PT2313_Atten_Register;          // Speaker Attenuators     
static uint8_t PT2313_Switch_Register = 0x5F;  // See PT2313 Datasheet

void PT2313_Init (void){
    I2C->CR1    = I2C_CR2_SWRST;            /* Reset I2C Peripheral Registers */
    I2C->FREQR  = 16;                       /* I2C Input Clock in MHz */
    I2C->TRISER = 17;                       /* Maximum Rise Time = FREQR + 1 */
    I2C->CCRL   = 50;                       /* I2C SCL Frequency = (Fmaster / FREQR) / (CCRL / 2) */
    I2C->CR1    = I2C_CR1_PE;               /* Enable I2C Peripheral */    
}

void _PT2313_Send (uint8_t Data){ 
    I2C->CR2 = I2C_CR2_START;               /* Generate Start Condition */
    while ((I2C->SR1 & I2C_SR1_SB) == 0);   /* Wait for Start Generation */
    I2C->DR  = PT2313_ADDR << 1;
    while ((I2C->SR1 & I2C_SR1_ADDR) == 0); /* Wait for Transmit Address */
    I2C->DR  = Data;
    while (((I2C->SR1) & I2C_SR1_TXE) == 0);/* Wait for Transmit Data */
    I2C->CR2 = I2C_CR2_STOP;
    while (I2C->SR3 & I2C_SR3_MSL);         /* Wait for Stop Generation */        
}

int8_t Check_Parameters (int8_t Value, int8_t Max, int8_t Min){
    if(Value > Max){
        Value = Max;
    }
    else if(Value < Min){
         Value = Min;
    }
    return Value;
}

void PT2313_Volume (uint8_t Volume){
    Volume = Check_Parameters(Volume,63,0);
    Volume = 63 - Volume ;
    _PT2313_Send(Volume);   
}

void PT2313_Bass (int8_t Bass){
    Bass = Check_Parameters(Bass,7,-7);
    if(Bass > 0){                       // Value Bigger than 0    
        Bass = 15 - Bass;
    }
    else{                               // Value is 0 or Smaller
        Bass = Bass + 7;
    }
    Bass +=  0x60 ;
    _PT2313_Send(Bass);
}

void PT2313_Treble (int8_t Treble){
    Treble = Check_Parameters(Treble,7,-7);
    if(Treble > 0){                     // Value Bigger than 0
        Treble = 15 - Treble;
    }
    else{                               // Value is 0 or Smaller
        Treble = Treble + 7;
    }
    Treble += 0x70 ;
    _PT2313_Send(Treble);
}
/*----------------------PT2313 Attenuatios Registers----------------------*/
void PT2313_Balance (int8_t Balance){
    Check_Parameters(Balance,7,-7);
    if(Balance < 0){                     // Negative Value = Left Attrib
        _PT2313_Send(0x80);              // PT2313 Front Left Attrib to 0
        _PT2313_Send(0xA0 | (-Balance));
        _PT2313_Send(0xC0);              // PT2313 Rear Left Attrib to 0      
        _PT2313_Send(0xE0 | (-Balance));     
        //Balance += 0xC0;                
        //_PT2313_Send(Balance);
    }
    else{                             // Positive Value = Right Attrib
        _PT2313_Send(0x80 | Balance);  // Front Left Set Attenuation
        _PT2313_Send(0xA0);            // Front Right Attrib to 0
        _PT2313_Send(0xC0 | Balance);  // Rear Left Set Attenuation
        _PT2313_Send(0xE0);            // Rear Right Attrib to 0
        //Balance += 0xE0;                // PT2313 Rear Right Register
        //_PT2313_Send(Balance);
    }
}

void PT2313_Mute (bool Mute_State){

    if(Mute_State){
        PT2313_Atten_Register = 0xFF;   // Enable Mute
    }
    else{
        PT2313_Atten_Register = 0xE0;   // Disable Mute
    }
    _PT2313_Send(PT2313_Atten_Register);
}
/*----------------------Attenuation Registers End----------------------*/

/*----------------------PT2313 Switch Registers----------------------*/

void PT2313_Input (uint8_t Input){
    Input = Check_Parameters(Input,4,1);
    PT2313_Switch_Register &= 0x5C;         // Clear bit 7,5,1,0
    PT2313_Switch_Register |= Input;
    _PT2313_Send(PT2313_Switch_Register);
}

void PT2313_Gain (uint8_t Gain){
    Gain = Check_Parameters(Gain,3,0);
    PT2313_Switch_Register &= 0x47;         // Clear bit 7,5,4,3   
    Gain = 0x03 - Gain;
    PT2313_Switch_Register |= (Gain << 3);
    _PT2313_Send(PT2313_Switch_Register);
}

void PT2313_Loudness (bool Loudness_Status){               // ON or 1 = Enable, OFF or 0 = Disable
    PT2313_Switch_Register &= 0x5B;                        // Clear bit 7,5,2
    PT2313_Switch_Register |= ((~Loudness_Status) << 1);
    _PT2313_Send(PT2313_Switch_Register);
}
/* End of File */