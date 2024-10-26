/**
 * @file     pt2313.c
 * @author   HS6502
 * @version  1.3
 * @date     11-Sep-2024 -- Updated 16-Oct-2024
 * @brief    Library for Drive PT2313 Audio Processor IC With STM8
 */
#include <pt2313.h>

static uint8_t PT2313_Atten_Register;      // Speaker Attenuators     
static uint8_t PT2313_Switch_Register = 0x5F;  // See PT2313 Datasheet

/**
 * @brief Local Function for Write Data Register to PT2313
 * @param Data
 */
void PT2313_Write (uint8_t Data){ 
    I2C_EnableAcknowledge();
    I2C_Start();
    I2C_WriteAddress(PT2313_ADDR,Tx);
    I2C_WriteData(Data);
    I2C_Stop();
}
/**
 * @brief Local Function for Check input Value, if Value out
 * @brief of Correct Range , Value Changed to Correct Range 
 */
int8_t Check_Parameters (int8_t Value, int8_t Max, int8_t Min){
    if(Value > Max){
        Value = Max;
    }
    else if(Value < Min){
         Value = Min;
    }
    return Value;
}

/**
 * @brief  Function for Set Volume
 * @param  Volume Set Volume Value 0 to 63
 */
void PT2313_Volume (uint8_t Volume){
    Volume = Check_Parameters(Volume,63,0);
    Volume = 63 - Volume ;
    PT2313_Write(Volume);   
}

/**
 * @brief  Function for Set Bass Gain
 * @param  Bass Bass Range from -7 to 7
 */
void PT2313_Bass (int8_t Bass){
    Bass = Check_Parameters(Bass,7,-7);
    if(Bass > 0){                       // Value Bigger than 0    
        Bass = 15 - Bass;
    }
    else{                               // Value is 0 or Smaller
        Bass = Bass + 7;
    }
    Bass +=  0x60 ;
    PT2313_Write(Bass);
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
    PT2313_Write(Treble);
}
/*----------------------PT2313 Attenuatios Registers----------------------*/
void PT2313_Balance (int8_t Balance){
    Check_Parameters(Balance,7,-7);
    if(Balance < 0){                     // Negative Value = Left Attrib
        PT2313_Write(0x80);              // PT2313 Front Left Attrib to 0
        PT2313_Write(0xA0 | (-Balance));
        PT2313_Write(0xC0);              // PT2313 Rear Left Attrib to 0      
        PT2313_Write(0xE0 | (-Balance));     
        //Balance += 0xC0;                
        //PT2313_Write(Balance);
    }
    else{                             // Positive Value = Right Attrib
        PT2313_Write(0x80 | Balance);  // Front Left Set Attenuation
        PT2313_Write(0xA0);            // Front Right Attrib to 0
        PT2313_Write(0xC0 | Balance);  // Rear Left Set Attenuation
        PT2313_Write(0xE0);            // Rear Right Attrib to 0
        //Balance += 0xE0;                // PT2313 Rear Right Register
        //PT2313_Write(Balance);
    }
}

void PT2313_Mute (bool Mute_State){

    if(Mute_State){
        PT2313_Atten_Register = 0xFF;   // Enable Mute
    else{
        PT2313_Atten_Register = 0xE0;   // Disable Mute
    }
    PT2313_Write(PT2313_Atten_Register);
}
/*----------------------Attenuation Registers End----------------------*/

/*----------------------PT2313 Switch Registers----------------------*/
/**
 * @brief Switch Audio Input
 * @param Input: Channel 1 to 4
 */
void PT2313_Input (uint8_t Input_Channel){
    Input_Channel = Check_Parameters(Input_Channel,4,1);
    PT2313_Switch_Register &= 0x5C;         // Clear bit 7,5,1,0
    PT2313_Switch_Register |= Input_Channel;
    PT2313_Write(PT2313_Switch_Register);
}

void PT2313_Gain (uint8_t Gain){
    Gain = Check_Parameters(Gain,3,0);
    PT2313_Switch_Register &= 0x47;         // Clear bit 7,5,4,3   
    Gain = 0x03 - Gain;
    PT2313_Switch_Register |= (Gain << 3);
    PT2313_Write(PT2313_Switch_Register);
}

void PT2313_Loudness (bool Loudness_Status){           // ON or 1 = Enable, OFF or 0 = Disable
    PT2313_Switch_Register &= 0x5B;                        // Clear bit 7,5,2
    PT2313_Switch_Register |= ((~Loudness_Status) << 1);
    PT2313_Write(PT2313_Switch_Register);
}
/*----------------------Switch Registers End----------------------*/

void PT2313_Reset (void){
    PT2313_Volume(PT2313_Default_Volume);
    PT2313_Input(PT2313_Default_Input);
    PT2313_Balance(0);
    PT2313_Bass(0);
    PT2313_Treble(0);    
}