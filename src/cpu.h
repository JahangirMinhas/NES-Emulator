#ifndef CPU_H
#define CPU_H

#include <cstdint>
#include "bus.h"

class Bus;

class Cpu {
    private:
        Bus* bus;

        // Cpu Registers
        uint8_t acc; // Accumulator
        uint8_t x; // X Register
        uint8_t y; // Y Register
        uint16_t pc; // Program Counter
        uint16_t sp; // Stack Pointer

        /* 8 bit word representing the flags
             Positions:
                0: Carry
                1: Zero
                2: IRQ Interrupt
                3: Decimal Mode
                4: BRK Command
                5: Unused
                6: Overflow
                7: Negative
        */
        uint8_t flags = 0x00;
        void setFlag(uint8_t pos, bool val);
        uint8_t getFlag(uint8_t pos);


        uint8_t cycles = 0, opcode, operand, high, low;
        uint16_t new_addr;

        uint8_t fetch_opcode(uint16_t addr);
        void cycle();

    public:
        Cpu(Bus* bus);

        // Addressing Modes
        void ACC(), IMM(), ABS(), ABS_X(), ABS_Y(), ZP(), ZP_X();
        void ZP_Y(), IMP(), REL(), IND_X(), IND_Y(), IND();

        // Operations
        void ADC(), AND(), ASL(), BCC(), BCS(), BEQ(), BIT(), BMI(), BNE(), BPL(), BRK(), BVC(), BVS(), CLC();
        void CLD(), CLI(), CLV(), CMP(), CPX(), CPY(), DEC(), DEX(), DEY(), EOR(), INC(), INX(), INY(), JMP();
        void JSR(), LDA(), LDX(), LDY(), LSR(), NOP(), ORA(), PHA(), PHP(), PLA(), PLP(), ROL(), ROR(), RTI();
        void RTS(), SBC(), SEC(), SED(), SEI(), STA(), STX(), STY(), TAX(), TAY(), TSX(), TXA(), TXS(), TYA();

        // Filler function for empty cell
        void NAN();
};

#endif