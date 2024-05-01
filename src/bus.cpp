#include "bus.h"

void Bus::write(uint8_t data, uint16_t addr) {
    ram[addr] = data;
};

uint8_t Bus::read(uint16_t addr) {
    return ram[addr];
};