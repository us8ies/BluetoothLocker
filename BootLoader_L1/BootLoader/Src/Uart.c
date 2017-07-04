#include "stdio.h"
#include "stdint.h"
#include "stdbool.h"
#include "stdlib.h"
#include "stdarg.h"

#include "uart.h"
#include "Common.h"
#include "Debug.h"
#include "Configuration.h"
#include "HwService.h"

FILE * __hc06;

typedef struct 
{
  uint8_t Data[rxBufferSize + msgEndSigSize + 1];
  bool IsReady;
  uint16_t Len;
} UartBufferTypeDef;

UartBufferTypeDef UartBuffers[uartBuffersCount];
uint8_t currentRxBuffer = 0;

static bool IsBufferReady(UartBufferTypeDef * buffer){
  if(buffer->Len < msgEndSigSize)
    return false;
  
  for(int i = 0; i < msgEndSigSize; i++)
  {
    if(msgEndSig[i] != buffer->Data[buffer->Len - msgEndSigSize + i])
      return false;
  }
  
  return true;
}


static void InitUart4(void){
  HWS_InitHc06Uart();
}

static int PutCharToHc06(int ch){
  
  HWS_Hc06PutChar(ch);

  return ch;
}

int WriteToHc(const char *format, ...){

  //if uart is not initialised __hc06 will be null
  if(__hc06 == NULL)
    return 1;
  
  va_list argptr;
  va_start(argptr, format);
  vfprintf(__hc06, format, argptr);
  va_end(argptr);
  
  return 0;
}

void InitUart(void){  
  __hc06 = new(FILE);
  __hc06->putc_ptr = &PutCharToHc06;
  
  InitUart4();
}


__weak void ProcessUartMessage(const uint8_t* data, uint16_t length){

}

void CheckUart(void){
  
  for(int i=0;i<uartBuffersCount;i++)
  {    
    if(!UartBuffers[i].IsReady)
      continue;
    
    UartBufferTypeDef* buffer = &UartBuffers[i];
    
    ProcessUartMessage(buffer->Data, buffer->Len);
    
    buffer->IsReady = false;
    buffer->Len = 0;
      
      //WriteToHc("CheckUart(): %d\r\n", i);
  }
}

static bool CheckUartError(){  
  
  uint32_t sr = HWS_Hc06_GetUartErrors();

  if(sr & (1 << HWS_ERROR_UART_PE)){  
    LogError(HWS_ERROR_UART_PE);
  }
  
  if(sr & (1 << HWS_ERROR_UART_FE)){  
    LogError(HWS_ERROR_UART_FE);
  }
  
  if(sr & (1 << HWS_ERROR_UART_NE)){  
    LogError(HWS_ERROR_UART_NE);
  }
  
  if(sr & (1 << HWS_ERROR_UART_ORE)){  
    LogError(HWS_ERROR_UART_ORE);
  }
  
  if(sr > 0)
  {
    HWS_Hc06_ClearUartErrors();

    return true;
  }

  return false;  
}

void UART_IRQHandler(void){
  
  uint32_t character = HWS_Hc06_GetUartCharacter();
  
  CheckUartError();
  
  UartBufferTypeDef *currBuff = &UartBuffers[currentRxBuffer];
  
  currBuff->Data[currBuff->Len++] = character;
  
  if(!IsBufferReady(currBuff))
    return;
  
  currBuff->IsReady = true;
  
  currentRxBuffer++;
  
  if(currentRxBuffer == uartBuffersCount)
    currentRxBuffer = 0;
  
  if(UartBuffers[currentRxBuffer].IsReady)
  {
    //TODO: log error, as buffer was not processed or not cleared on time
  }
  
  UartBuffers[currentRxBuffer].IsReady = false;
  UartBuffers[currentRxBuffer].Len = 0;
}

int fputc(int ch, FILE *f) {

  return f->putc_ptr(ch);
}
