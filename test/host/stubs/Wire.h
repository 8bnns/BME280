#pragma once
#include <stdint.h>
struct TwoWire{ int endRet=0; void begin(){} void beginTransmission(uint8_t){} void write(uint8_t){} uint8_t endTransmission(){return endRet;} uint8_t requestFrom(uint8_t,uint8_t){return 0;} int available(){return 0;} int read(){return 0;} };
extern TwoWire Wire;
