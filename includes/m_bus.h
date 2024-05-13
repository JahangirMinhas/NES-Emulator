#ifndef M_BUS_H
#define M_BUS_H

#include <cstdint>
#include <iostream>
#include <string>
#include <cstring>
#include "cpu.h"

class Cpu;

// Represent the main cpu bus where all NES components will be connected.
class MBus
{
    public:
        MBus();
        Cpu* cpu;
        uint8_t cpu_mem[64*1024];
        uint8_t* read(uint16_t addr);
        void write(uint8_t data, uint16_t addr);
};

#endif