#ifndef __MISHA_CONFIGURATION_H
#define __MISHA_CONFIGURATION_H

#include "stdint.h"

const uint8_t msgEndSigSize = 2;
const uint8_t msgEndSig[msgEndSigSize] =  {'\r', '\n'};

const uint16_t rxBufferSize = 500;
const uint8_t uartBuffersCount = 3;

#endif
