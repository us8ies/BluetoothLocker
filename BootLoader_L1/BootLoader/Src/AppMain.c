#include "Uart.h"
#include "Debug.h"
#include "Parser.h"
#include "Bootloader.h"
#include "SwLoader.h"

#include "stdbool.h"

#include "HwService.h"

void ProcessUartMessage(const uint8_t* data, uint16_t length){
  
  ParseResult result = ParseMessage(data, length);
  
  switch(result)
  {
    case ParseNotFound:
      WriteToHc("ParseNotFound");
    break;
    case ParseOk:
      WriteToHc("ParseOk");
    break;
    case ParseError:
      WriteToHc("ParseError");
    break;
    default:
      WriteToHc("Unknown ParseResult");
    break;
  }

  WriteToHc("\r\n");
}

void APP_SysTick_Handler(void){

  HWS_IncTick();
  
  Boot1MsTick();
}

void APP_UART4_IRQHandler(void){
  UART_IRQHandler();
}

static bool BootloaderMustRun(void){
  
  bool extReq = RequestedFromMainApp();
  
  bool mainAppIsWritten = MainAppIsWritten();
  
  if(extReq || !mainAppIsWritten)
    return true;
  
  return false;
}

int APP_main(void){
  HWS_Init();
	HWS_RCC_OscConfig();

  InitDebug();

  InitUart();

  InitParser();

  InitSwLoader(&RegisterParser);
  
  if(!BootloaderMustRun())
  {
    RunMainApp();
  }
  
  WriteToHc("bootready\r\n");

  while (1)
  {
    CheckUart();
  }
}
