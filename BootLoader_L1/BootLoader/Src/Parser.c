#include "Parser.h"
#include "Common.h"
#include "Debug.h"
#include "Uart.h"
#include "Bootloader.h"

#include "stdlib.h"
#include "string.h"

static Parser* _parsers;
static bool areParsersInitialized = false;

static uint8_t CharToByte(int c){
    char hex[] = "00112233445566778899aAbBcCdDeEfF";
    int i;
    uint8_t result = 0;

    for(i = 0; result == 0 && hex[i] != '\0'; i++)
    {
        if(hex[i] == c)
        {
            result = i >> 1;
        }
    }

    return result;
}

static uint8_t ReadByte(const uint8_t* data){
  uint8_t result;
  
  result = CharToByte( data[0]) * 16;
  result += CharToByte( data[1]);
  
  return result;
}

static ParseResult ReadErrosCount(const uint8_t* data, uint16_t length){
  uint32_t errorId = data[0];
  
  uint32_t count = GetErrosCount((HWS_ERRORS_TypeDef)errorId);
  
  WriteToHc("errors %d: %d\r\n", errorId, count);
  
  return ParseOk;
}

static ParseResult SwLoadComplete(const uint8_t* data, uint16_t length){
  
  ClearJumpToBootloader();
  
  DelayReset(5000);

  return ParseOk;  
}

static ParseResult JumpToBootloaderRequest(const uint8_t* data, uint16_t length){
  
  SetJumpToBootloader();
  
  DelayReset(5000);

  return ParseOk;  
}

static const Parser parsers_[] = {
  {ReadErrosCount, {'a','b','c'}},
  {SwLoadComplete, {'b','b','b'}},
  {JumpToBootloaderRequest, {'a','a','a'}},
  {NULL, {0,0,0}}
};

void InitParser(void){
  
  if(areParsersInitialized)
    return;

  _parsers = new(Parser);

  _parsers[0].TryParse = NULL;
  
  RegisterParser(&parsers_[0]);
  RegisterParser(&parsers_[1]);
  RegisterParser(&parsers_[2]);
  
  areParsersInitialized = true;
}

void RegisterParser(const Parser * parser){
  uint8_t oldLen = 0;
  
  while(_parsers[oldLen].TryParse != NULL){
    oldLen++;
  }
  
  oldLen++;
  
  Parser * newStorage = calloc(oldLen + 1, sizeof *_parsers);
  
  memmove(newStorage, _parsers, oldLen * sizeof *_parsers);
  
  newStorage[oldLen] = newStorage[oldLen-1];
  
  newStorage[oldLen-1] = *parser;
  
  free(_parsers);
  
  _parsers = newStorage;
}

static const uint8_t* ConvertToBinary(const uint8_t* data, uint16_t hexLength, uint16_t* binLength){

  //hex string cant be odd length
  if(hexLength & 1)
    return NULL;
  
  *binLength = hexLength >> 1;
  
  uint8_t* ptr = malloc(*binLength);
  
  for(uint16_t i = 0;i<*binLength;i++){
    ptr[i] = ReadByte(&data[i*2]);
  }
  
  return ptr;
}

ParseResult ParseMessage(const uint8_t* data, uint16_t length){
  int i=0;
  
  ParseResult parseResult = ParseNotFound;
  
  while(_parsers[i].TryParse!= NULL){
    bool isFilterFits = AR_StartsWith(data, _parsers[i].Filter, parserFilterSize);
    
    if(isFilterFits)
    {
      uint16_t hexLength = length - parserFilterSize;
      uint16_t binLength = 0;
      
      const uint8_t* binaryData = ConvertToBinary(&data[parserFilterSize], hexLength, &binLength);
      
      if(binaryData != NULL){
        parseResult = _parsers[i].TryParse(binaryData, binLength);
      }
      else{
        parseResult = ParseError;
      }
        
      free((void *)binaryData);
      
      break;
    }
    
    i++;
  }
  
  return parseResult;
}
