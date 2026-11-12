#pragma once
#include <stdint.h>
#include <math.h>
#include <stdio.h>
#define HIGH 1
#define LOW 0
#define OUTPUT 1
#define INPUT 0
#define MSBFIRST 1
#define SPI_MODE0 0
extern unsigned long g_ms;
inline void delay(unsigned long m){ g_ms+=m; }
inline void pinMode(int,int){} inline void digitalWrite(int,int){} inline int digitalRead(int){return 0;}
