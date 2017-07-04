#ifndef __MISHA_DEBUG_H
#define __MISHA_DEBUG_H

#include "Common.h"

typedef enum{
  HWS_ERROR_UART_PE,
  HWS_ERROR_UART_FE,
  HWS_ERROR_UART_NE,
  HWS_ERROR_UART_ORE,
  HWS_ERROR_LAST_ENUM_MARKER
}HWS_ERRORS_TypeDef;

int Log(const char *format, ...);
void InitDebug(void);
void LogError(HWS_ERRORS_TypeDef error);
uint32_t GetErrosCount(HWS_ERRORS_TypeDef error);

#define DBG_assert_param(expr) ((expr) ? (void)0 : DBG_assert_failed((uint8_t *)__FILE__, __LINE__))
  
void DBG_assert_failed(uint8_t* file, uint32_t line);

#endif
