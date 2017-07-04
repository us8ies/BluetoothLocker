#ifndef __MISHA_PARSER_H
#define __MISHA_PARSER_H

#include "stdint.h"
#include "stdbool.h"

#define parserFilterSize 3

typedef enum {
  ParseNotFound,
  ParseOk,
  ParseError
}ParseResult;

typedef struct{
  ParseResult (*TryParse)(const uint8_t* data, uint16_t length);
  uint8_t Filter[parserFilterSize];
}Parser;

ParseResult ParseMessage(const uint8_t* data, uint16_t length);

void RegisterParser(const Parser * parser);

void InitParser(void);

#endif
