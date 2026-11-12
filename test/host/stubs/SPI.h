#pragma once
#include <stdint.h>
struct SPISettings{SPISettings(long,int,int){}};
struct SPIClass{void beginTransaction(SPISettings){} void endTransaction(){} uint8_t transfer(uint8_t){return 0;}};
extern SPIClass SPI;
