#include "SwLoader.h"
#include "Common.h"
#include "string.h"
#include "HwService.h"
#include "stdbool.h"

static void EraseFlash(BinaryStream* request){

  uint32_t typeErase = Read_uint32_t(request);
  uint32_t banks = Read_uint32_t(request);
  uint32_t sector = Read_uint32_t(request);
  uint32_t nbSectors = Read_uint32_t(request);
  uint32_t voltageRange = Read_uint32_t(request);

  HWS_FLASHEx_Erase(banks, nbSectors, sector, typeErase, voltageRange);
}

static bool WritePacket(BinaryStream* request, uint32_t address, uint32_t typeProgram){
  uint64_t data = Read_uint32_t(request);

  bool result = HWS_FLASH_Program(typeProgram, address, data);
  
  return result;
}

static bool WriteBytes(BinaryStream* request, uint8_t numberOfPackets){
  
  uint32_t typeProgram = Read_uint32_t(request);
  uint32_t address = Read_uint32_t(request);
  
  uint8_t bytesInPacket[] = {1, 2, 4, 8};
  
  bool result = true;
  
  for(uint8_t i = 0;i<numberOfPackets;i++){
    
    result = WritePacket(request, address, typeProgram);
    
    address += bytesInPacket[typeProgram];
    
    if(!result)
      break;
  }
  
  return result;
}

static bool WritePackets(BinaryStream* request){

  uint8_t numberOfPackets = Read_uint8_t(request);
  
  return WriteBytes(request, numberOfPackets);
}

static ParseResult SwLoaderMessageParser(const uint8_t* data, uint16_t length){
  bool writeResult = false;
  
  ParseResult result = ParseNotFound;
  BinaryStream* requestStream = CreateStream(data, length);
  uint8_t actionType = Read_uint8_t(requestStream);  

  switch(actionType)
  {
    case 1:
      EraseFlash(requestStream);
      result = ParseOk;
      break;
    case 2:
      writeResult = WriteBytes(requestStream, 1);
      if(writeResult == true)
      {
        result = ParseOk;
      }
      break;
    case 3:
      HWS_NVIC_SystemReset();
      break;
    case 4:
      writeResult = WritePackets(requestStream);
      if(writeResult == true)
      {
        result = ParseOk;
      }
      break;
    default:
      break;
  }
  
  free(requestStream);
  
  return result;
}

void InitSwLoader(void (*MessageSource)(const Parser* parser)){ 
  Parser* newParser = new(Parser);
  memcpy(newParser->Filter, "swu", parserFilterSize);
  newParser->TryParse = SwLoaderMessageParser;

  MessageSource(newParser);
}
