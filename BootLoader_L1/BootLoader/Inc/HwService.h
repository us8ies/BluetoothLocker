#ifndef __MISHA_HWSERVICE_H
#define __MISHA_HWSERVICE_H

#include "stdbool.h"
#include "stdint.h"

bool HWS_FLASH_Program(uint32_t TypeProgram, uint32_t Address, uint64_t Data);

void HWS_FLASHEx_Erase(uint32_t banks, uint32_t nbSectors, uint32_t sector, uint32_t typeErase, uint32_t voltageRange);
void HWS_RCC_OscConfig(void);

void HWS_IncTick(void);
void HWS_Init(void);
void HWS_NVIC_SystemReset(void);

void HWS_WriteToBackup(uint32_t address, uint8_t value);
void HWS_ClearBackup(uint32_t address);
uint8_t HWS_ReadBackup(uint32_t address);

void HWS_InitHc06Uart(void);
void HWS_Hc06PutChar(int character);

uint32_t HWS_Hc06_GetUartErrors(void);
void HWS_Hc06_ClearUartErrors(void);
uint32_t HWS_Hc06_GetUartCharacter(void);

void __HWS_set_MSP(uint32_t topOfMainStack);

#endif
