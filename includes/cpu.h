#ifndef CPU_H
#define CPU_H

#include <cstdint>
#include "bus.h"

class Bus;

class Cpu {
    public:
        Cpu(Bus* bus);

        Bus* bus;

        // Cpu Registers
        uint8_t x = 0; // X Register
        uint8_t y = 0; // Y Register
        uint8_t acc = 0; // Accumulator
        uint16_t pc; // Program Counter
        uint16_t sp = 0x01FD; // Stack Pointer

        #define C 0
        #define Z 1
        #define I 2
        #define D 3
        #define B 4
        #define U 5
        #define V 6
        #define N 7
        /* 8 bit word representing the flags (Status Register)
             Positions:
                0: Carry
                1: Zero
                2: IRQ Disable Interrupt
                3: Decimal Mode
                4: BRK Command
                5: Unused (always set)
                6: Overflow
                7: Negative
        */
        uint8_t sr = 0x00 | 0x01 << U;

        void setFlag(uint8_t pos, bool val);
        uint8_t getFlag(uint8_t pos);

        uint8_t cycles = 0, opcode;
        uint8_t* operand;
        uint16_t abs_addr;

        uint8_t fetch_opcode(uint16_t addr);
        void branch();

        void push_stk(uint8_t byte);
        uint8_t pop_stk();

        void exec_ins();
        void reset();
        void irq();
        void nmi();

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