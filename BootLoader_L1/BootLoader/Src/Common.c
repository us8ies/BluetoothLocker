#include "Common.h"
#include "HwService.h"
#include "Debug.h"

bool AR_StartsWith(const uint8_t* data, const uint8_t* filter, const uint8_t filterLength){

  for(uint8_t i=0; i<filterLength;i++){
    if(data[i] != filter[i])
      return false;
  }
  
  return true;
}

static uint64_t ReadBytes(BinaryStream* stream, uint8_t count){
  uint64_t result = 0;
  
  uint8_t len = 0;
  
  while(len < count){
    result = result << 8;
    
    DBG_assert_param(stream->currPos < stream->maxLen); 
    
    result += stream->data[stream->currPos++];
    len++;
  }
  
  return result;
}

uint32_t Read_uint32_t(BinaryStream* stream){
  uint32_t result = (uint32_t)ReadBytes(stream, 4);

  return result;
}

uint64_t Read_uint64_t(BinaryStream* stream){
  uint64_t result = ReadBytes(stream, 8);

  return result;
}

uint8_t Read_uint8_t(BinaryStream* stream){
  uint8_t result = (uint8_t)ReadBytes(stream, 1);

  return result;
}

BinaryStream* CreateStream(const uint8_t* data, uint16_t maxLen){
  BinaryStream * result = new(BinaryStream);
  
  result->data = data;
  result->maxLen = maxLen;
  result->currPos = 0;
  
  return result;
}
