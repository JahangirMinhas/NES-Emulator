// Main Bus

#include "..\includes\m_bus.h"

MBus::MBus(){
    cpu = new Cpu(this);
    std::memset(cpu_mem, 0x00, sizeof(cpu_mem));
}

void MBus::write(uint8_t data, uint16_t addr) {
    cpu_mem[addr] = data;
}

uint8_t* MBus::read(uint16_t addr) {
    return &cpu_mem[addr];
}