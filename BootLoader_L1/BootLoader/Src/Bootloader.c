#include "CommonDefines.h"
#include "stdbool.h"

#include "HwService.h"

typedef void (*pFunction)(void);

uint16_t timeToReset = 0;

void SetJumpToBootloader(void){

  HWS_WriteToBackup(BootloaderBkpSramRequestAddress, BootloaderMagicNumber);
}

void ClearJumpToBootloader(void){

  HWS_ClearBackup(BootloaderBkpSramRequestAddress);
}

bool RequestedFromMainApp(void){

  uint8_t extRequest = HWS_ReadBackup(BootloaderBkpSramRequestAddress);

  return extRequest == BootloaderMagicNumber;
}

bool MainAppIsWritten(void){

  //TODO: this approach is bad, need to store some flag afte sw is loaded, beginning of eeprom?
  
  return ((*(volatile uint32_t*)APPLICATION_ADDRESS) & 0x2FFE0000 ) == 0x20000000;
}

void RunMainApp(void){
  /* Jump to user application */
  uint32_t JumpAddress = *(volatile uint32_t*) (APPLICATION_ADDRESS + 4);

  pFunction Jump_To_Application = (pFunction)JumpAddress;

  /* Initialize user application's Stack Pointer */
  __HWS_set_MSP(APPLICATION_ADDRESS);

  Jump_To_Application();
}

void Boot1MsTick(void){
  if(timeToReset == 0) return;

  timeToReset--;

  if(timeToReset == 0){
    HWS_NVIC_SystemReset();
  }
}

void DelayReset(uint16_t delayTime){
  timeToReset = delayTime;
}
