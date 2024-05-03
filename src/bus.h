#ifndef BUS_H
#define BUS_H

#include <cstdint>
#include "cpu.h"

class Cpu;

class Bus
{
    public:
        Bus();
        Cpu* cpu;
        uint8_t ram[64 * 1024];
        uint8_t read(uint16_t addr);
        void write(uint8_t data, uint16_t addr);
};

#endif