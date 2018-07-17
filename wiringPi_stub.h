#ifndef WIRINGPI_STUB_H
#define WIRINGPI_STUB_H

#include <cstdint>


//stubs for non pi compiling..
void wiringPiSetupGpio()
{
    
}

#define INPUT 1
#define OUTPUT 0

#define INT_EDGE_FALLING 0

void pinMode(uint16_t pin, bool readwrite)
{
    
}

void digitalWrite(uint16_t pin, bool pinval)
{
    
}

void wiringPiISR(uint16_t pin, uint16_t edge , void(* func)(void))
{
    
}
#endif /* WIRINGPI_STUB_H */

