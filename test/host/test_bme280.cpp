#include "BME280.h"
#include "EnvironmentCalculations.h"
#include <Wire.h>
#include <SPI.h>
#include <assert.h>
#include <string.h>
unsigned long g_ms=0; TwoWire Wire; SPIClass SPI;
// Fake BME280: forced trigger stores a "pending" measurement only available after the needed time.
struct Fake: BME280 {
  uint8_t reg[256]; unsigned long trigAt=0; bool pending=false; int measN=0;
  Fake(uint8_t id):BME280(Settings()){ memset(reg,0,256); reg[0xD0]=id;
    // trim from a real sensor (Bosch example values)
    uint8_t t[]={0x70,0x6B,0x43,0x67,0x18,0xFC}; memcpy(&reg[0x88],t,6);
    uint8_t p[]={0x7D,0x8E,0x43,0xD6,0xD0,0x0B,0x27,0x0B,0x8C,0x00,0xF9,0xFF,0x8C,0x3C,0xF8,0xC6,0x70,0x17}; memcpy(&reg[0x8E],p,18);
    reg[0xA1]=0x4B; uint8_t h[]={0x6B,0x01,0x00,0x13,0x2C,0x03,0x1E}; memcpy(&reg[0xE1],h,7);
    set(0); }
  void set(int n){ // n=0 stale sample, n=1 new sample
    uint8_t d0[]={0x50,0,0,0x80,0,0,0x80,0}; uint8_t d1[]={0x65,0x5A,0xC0,0x7E,0xED,0x00,0x66,0x75};
    memcpy(&reg[0xF7], n?d1:d0, 8); }
  bool WriteRegister(uint8_t a,uint8_t d) override { reg[a]=d; if(a==0xF4 && (d&3)==1){ trigAt=g_ms; pending=true; } return true; }
  bool ReadRegister(uint8_t a,uint8_t* d,uint8_t l) override {
    if(pending && g_ms>=trigAt+8){ pending=false; set(1); }
    reg[0xF3]= pending?0x08:0;
    memcpy(d,&reg[a],l); return true; }
};
int main(){
  Fake f(0x60); assert(f.begin());
  float p,t,h; f.read(p,t,h);
  printf("BME280: T=%.2f P=%.2f H=%.2f\n",t,p,h);
  assert(t>20 && t<30 && p>900 && p<1100 && h>0 && h<100);
  Fake b(0x58); assert(b.begin()); b.read(p,t,h); printf("BMP280: H=%f\n",h); assert(isnan(h)&&isnan(b.hum()));
  Fake u(0x11); assert(!u.begin());
  // EnvironmentCalculations, expected values from the NWS / Magnus reference formulas.
  using namespace EnvironmentCalculations;
  #define NEAR(a,b,eps) (fabs((a)-(b)) < (eps))
  assert(NEAR(HeatIndex(90.0, 60.0, TempUnit_Fahrenheit), 99.678, 0.01));
  // Low-humidity adjustment: a non-integer |T-95| must not be truncated.
  assert(NEAR(HeatIndex(95.5, 10.0, TempUnit_Fahrenheit), 89.904, 0.01));
  assert(NEAR(DewPoint(25.0, 50.0), 13.858, 0.01));
  assert(NEAR(Altitude(899.9), 1011.85, 0.5));
  assert(isnan(DewPoint(NAN, 50.0)));
  puts("OK");
}
