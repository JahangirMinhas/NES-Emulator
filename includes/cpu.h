#ifndef CPU_H
#define CPU_H

#include <cstdint>
#include "m_bus.h"

class MBus;

#define C 0 // Carry
#define Z 1 // Zero
#define I 2 // Disable IRQ Interrupts
#define D 3 // Decimal Mode
#define B 4 // BRK Command
#define U 5 // Unused (ALWAYS SET)
#define V 6 // Overflow
#define N 7 // Negative

class Cpu {
    private:
        // Cpu Registers
        uint8_t x = 0; // X Register
        uint8_t y = 0; // Y Register
        uint8_t acc = 0; // Accumulator
        uint16_t pc; // Program Counter
        uint16_t sp = 0x01FD; // Stack Pointer
        uint8_t sr = 0x00 | 0x01 << U; // Status Register

        // CPU Vars
        uint8_t cycles = 0, opcode;
        uint8_t* operand;
        uint16_t abs_addr;

        // CPU read and write to main bus
        uint8_t* read(uint16_t addr);
        void write(uint8_t data, uint16_t addr);

        // Get opcode from cpu_mem
        uint8_t fetch_opcode(uint16_t addr);

        // Pop and push to cpu_mem stack on page 0x01
        void push_stk(uint8_t byte);
        uint8_t pop_stk();

        // Used when a branch operation is called
        void branch();

        void reset(); // Reset CPU state
        void irq(); // Called for IRQ Interrupts
        void nmi(); // Called for NMI Interrupts
    public:
        // Connect CPU to Main Bus
        MBus* m_bus;
        Cpu(MBus* m_bus);

        // Set and Get flags from SR
        void setFlag(uint8_t pos, bool val);
        uint8_t getFlag(uint8_t pos);

        // Getters for registers
        uint8_t getX(), getY(), getAcc(), getSr();
        uint16_t getSp(), getPc();

        uint8_t get_cyc(); // Getter for cycles

        // Set Program Counter Register
        void setPc(uint16_t pc);

        void exec_ins(); // Execute an instruction

        // Addressing Modes
        void ACC(), IMM(), ABS(), ABS_X(), ABS_Y(), ZP(), ZP_X();
        void ZP_Y(), IMP(), REL(), IND_X(), IND_Y(), IND();

        // Operations
        void ADC(), AND(), ASL(), BCC(), BCS(), BEQ(), BIT(), BMI(), BNE(), BPL(), BRK(), BVC(), BVS(), CLC();
        void CLD(), CLI(), CLV(), CMP(), CPX(), CPY(), DEC(), DEX(), DEY(), EOR(), INC(), INX(), INY(), JMP();
        void JSR(), LDA(), LDX(), LDY(), LSR(), NOP(), ORA(), PHA(), PHP(), PLA(), PLP(), ROL(), ROR(), RTI();
        void RTS(), SBC(), SEC(), SED(), SEI(), STA(), STX(), STY(), TAX(), TAY(), TSX(), TXA(), TXS(), TYA();
        void XXX(); // Illegal Opcode
};

#endif