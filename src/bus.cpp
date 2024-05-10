#include "bus.h"

Bus::Bus(){
    cpu = new Cpu(this);
    std::memset(ram, 0x00, sizeof(ram));
}

void Bus::write(uint8_t data, uint16_t addr) {
    ram[addr] = data;
}

uint8_t* Bus::read(uint16_t addr) {
    return &ram[addr];
}