#ifndef __MISHA_BOOTLOADER_H
#define __MISHA_BOOTLOADER_H

#include "stdbool.h"

void SetJumpToBootloader(void);
void ClearJumpToBootloader(void);
bool RequestedFromMainApp(void);
bool MainAppIsWritten(void);
void RunMainApp(void);
void Boot1MsTick(void);
void DelayReset(uint16_t);

#endif
