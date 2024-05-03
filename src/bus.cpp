#include "bus.h"

Bus::Bus(){
    cpu = new Cpu(this);
    for(uint8_t a: ram){
        a = 0x00;
    };
};

void Bus::write(uint8_t data, uint16_t addr) {
    ram[addr] = data;
};

uint8_t Bus::read(uint16_t addr) {
    return ram[addr];
};