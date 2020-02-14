#ifndef __AT93C66B_H
#define __AT93C66B_H

#include "stm32f1xx.h"
#include "stm32f1xx_hal.h"
#include <stdbool.h>
#include "includes.h"


//bool At93c66b_WriteByte (uint16_t iAddress, uint8_t iData, char *strMsg);
//bool At93c66b_ReadByte (uint16_t iAddress, uint8_t *iData, char *strMsg);
//bool At93c66b_WriteWord (uint8_t iAddress, uint16_t iData, char *strMsg);
//bool At93c66b_ReadWord (uint8_t iAddress, uint8_t *iData, char *strMsg);
//bool At93c66b_WriteAll (uint8_t iData, char *strMsg);
//bool At93c66b_EraseAll(char *strMsg);
//bool At93c66b_Erase(uint8_t iAddress, char *strMsg);

u16 EEread9366(u16 addr);
void EEwrite9366(u16 addr,u16 eData);

#endif
