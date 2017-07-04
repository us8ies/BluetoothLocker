#ifndef __MISHA_UART_H
#define __MISHA_UART_H

#include "stdint.h"

struct __FILE{
  int (*putc_ptr)(int ch);
};

int WriteToHc(const char *format, ...);

void InitUart(void);

void CheckUart(void);

void UART_IRQHandler(void);

void ProcessUartMessage(const uint8_t* data, uint16_t length);

#endif
