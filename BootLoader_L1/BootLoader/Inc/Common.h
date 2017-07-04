#ifndef __MISHA_COMMON_H
#define __MISHA_COMMON_H

#include "stdlib.h"
#include "stdint.h"
#include "stdbool.h"

#define new(type) ((type *)calloc(1, sizeof(type)));

#undef NULL
#define NULL 0

bool AR_StartsWith(const uint8_t* data, const uint8_t* filter, const uint8_t filterLength);

typedef struct __BinaryStream BinaryStream;

struct __BinaryStream{
  const uint8_t* data;
  uint16_t currPos;
  uint16_t maxLen;
};

uint8_t Read_uint8_t(BinaryStream* stream);
uint32_t Read_uint32_t(BinaryStream* stream);
uint64_t Read_uint64_t(BinaryStream* stream);

BinaryStream* CreateStream(const uint8_t* data, uint16_t maxLen);

#endif
