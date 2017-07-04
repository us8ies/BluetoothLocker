#include "stdio.h"
#include "Uart.h"
#include "stdarg.h"
#include "Common.h"
#include "Debug.h"

FILE * __debug;

#define ITM_Port8(n)    (*((volatile unsigned char *)(0xE0000000+4*n)))
#define ITM_Port16(n)   (*((volatile unsigned short*)(0xE0000000+4*n)))
#define ITM_Port32(n)   (*((volatile unsigned long *)(0xE0000000+4*n)))

#define DEMCR           (*((volatile unsigned long *)(0xE000EDFC)))
#define TRCENA          0x01000000

uint32_t ErrorsStorage[HWS_ERROR_LAST_ENUM_MARKER];

static int PutCharToDebug (int ch){
  if (DEMCR & TRCENA) {
    while (ITM_Port32(0) == 0);
    ITM_Port8(0) = ch;
  }
  return(ch);
}

int Log(const char *format, ...){
  va_list argptr;
  va_start(argptr, format);
  vfprintf(__debug, format, argptr);
  va_end(argptr);
  
  return 0;
}

void InitDebug(void){
  __debug = new(FILE);
  __debug->putc_ptr = &PutCharToDebug;
}

void LogError(HWS_ERRORS_TypeDef error){
  ErrorsStorage[error]++;
}

uint32_t GetErrosCount(HWS_ERRORS_TypeDef error){
  return ErrorsStorage[error];
}

void DBG_assert_failed(uint8_t* file, uint32_t line){ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  
  Log("Wrong parameters value: file %s on line %d\r\n", file, line);

  /* Infinite loop */
  while (1)
  {
  }
}

void assert_failed(uint8_t* file, uint32_t line){
  DBG_assert_failed(file, line);
}
