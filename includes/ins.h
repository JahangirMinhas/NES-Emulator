#ifndef INS_H
#define INS_H

#include <cstdint>
#include <string>
#include "cpu.h"

class ins {
    public:
        std::string name;
        void (Cpu::*addr_mode)();
        void (Cpu::*operation)();
        uint8_t cycles;
        uint8_t bytes;
        ins(std::string name, void (Cpu::*operation)(), void (Cpu::*addr_mode)(), uint8_t bytes, uint8_t cycles);
};

using c = Cpu;
ins instructions[256] = {
    // Row 0
    ins("BRK", &c::BRK, &c::IMP, 1, 7),
    ins("ORA", &c::ORA, &c::IND_X, 2, 6),
    ins("XXX", &c::XXX, &c::XXX, 0, 0),
    ins("XXX", &c::XXX, &c::XXX, 0, 0),
    ins("XXX", &c::XXX, &c::XXX, 0, 0),
    ins("ORA", &c::ORA, &c::ZP, 2, 3),
    ins("ASL", &c::ASL, &c::ZP, 2, 5),
    ins("XXX", &c::XXX, &c::XXX, 0, 0),
    ins("PHP", &c::PHP, &c::IMP, 1, 3),
    ins("ORA", &c::ORA, &c::IMM, 2, 2),
    ins("ASL", &c::ASL, &c::ACC, 1, 2),
    ins("XXX", &c::XXX, &c::XXX, 0, 0),
    ins("XXX", &c::XXX, &c::XXX, 0, 0),
    ins("ORA", &c::ORA, &c::ABS, 3, 4),
    ins("ASL", &c::ASL, &c::ABS, 3, 6),
    ins("XXX", &c::XXX, &c::XXX, 0, 0),

    // Row 1
    ins("BPL", &c::BPL, &c::REL, 2, 2),
    ins("ORA", &c::ORA, &c::IND_Y, 2, 5),
    ins("XXX", &c::XXX, &c::XXX, 0, 0),
    ins("XXX", &c::XXX, &c::XXX, 0, 0),
    ins("XXX", &c::XXX, &c::XXX, 0, 0),
    ins("ORA", &c::ORA, &c::ZP_X, 2, 4),
    ins("ASL", &c::ASL, &c::ZP_X, 2, 6),
    ins("XXX", &c::XXX, &c::XXX, 0, 0),
    ins("CLC", &c::CLC, &c::IMP, 1, 2),
    ins("ORA", &c::ORA, &c::ABS_Y, 3, 4),
    ins("XXX", &c::XXX, &c::XXX, 0, 0),
    ins("XXX", &c::XXX, &c::XXX, 0, 0),
    ins("XXX", &c::XXX, &c::XXX, 0, 0),
    ins("ORA", &c::ORA, &c::ABS_X, 3, 4),
    ins("ASL", &c::ASL, &c::ABS_X, 3, 7),
    ins("XXX", &c::XXX, &c::XXX, 0, 0),

    // Row 2
    ins("JSR", &c::JSR, &c::ABS, 3, 6),
    ins("AND", &c::AND, &c::IND_X, 2, 6),
    ins("XXX", &c::XXX, &c::XXX, 0, 0),
    ins("XXX", &c::XXX, &c::XXX, 0, 0),
    ins("BIT", &c::BIT, &c::ZP, 2, 3),
    ins("AND", &c::AND, &c::ZP, 2, 3),
    ins("ROL", &c::ROL, &c::ZP, 2, 5),
    ins("XXX", &c::XXX, &c::XXX, 0, 0),
    ins("PLP", &c::PLP, &c::IMP, 1, 4),
    ins("AND", &c::AND, &c::IMM, 2, 2),
    ins("ROL", &c::ROL, &c::ACC, 1, 2),
    ins("XXX", &c::XXX, &c::XXX, 0, 0),
    ins("BIT", &c::BIT, &c::ABS, 3, 4),
    ins("AND", &c::AND, &c::ABS, 3, 4),
    ins("ROL", &c::ROL, &c::ABS, 3, 6),
    ins("XXX", &c::XXX, &c::XXX, 0, 0),

    // Row 3
    ins("BMI", &c::BMI, &c::REL, 2, 2),
    ins("AND", &c::AND, &c::IND_Y, 2, 5),
    ins("XXX", &c::XXX, &c::XXX, 0, 0),
    ins("XXX", &c::XXX, &c::XXX, 0, 0),
    ins("XXX", &c::XXX, &c::XXX, 0, 0),
    ins("AND", &c::AND, &c::ZP_X, 2, 4),
    ins("ROL", &c::ROL, &c::ZP_X, 2, 6),
    ins("XXX", &c::XXX, &c::XXX, 0, 0),
    ins("SEC", &c::SEC, &c::IMP, 1, 2),
    ins("AND", &c::AND, &c::ABS_Y, 3, 4),
    ins("XXX", &c::XXX, &c::XXX, 0, 0),
    ins("XXX", &c::XXX, &c::XXX, 0, 0),
    ins("XXX", &c::XXX, &c::XXX, 0, 0),
    ins("AND", &c::AND, &c::ABS_X, 3, 4),
    ins("ROL", &c::ROL, &c::ABS_X, 3, 7),
    ins("XXX", &c::XXX, &c::XXX, 0, 0),

    // Row 4
    ins("RTI", &c::RTI, &c::IMP, 1, 6),
    ins("EOR", &c::EOR, &c::IND_X, 2, 6),
    ins("XXX", &c::XXX, &c::XXX, 0, 0),
    ins("XXX", &c::XXX, &c::XXX, 0, 0),
    ins("XXX", &c::XXX, &c::XXX, 0, 0),
    ins("EOR", &c::EOR, &c::ZP, 2, 3),
    ins("LSR", &c::LSR, &c::ZP, 2, 5),
    ins("XXX", &c::XXX, &c::XXX, 0, 0),
    ins("PHA", &c::PHA, &c::IMP, 1, 3),
    ins("EOR", &c::EOR, &c::IMM, 2, 2),
    ins("LSR", &c::LSR, &c::ACC, 1, 2),
    ins("XXX", &c::XXX, &c::XXX, 0, 0),
    ins("JMP", &c::JMP, &c::ABS, 3, 3),
    ins("EOR", &c::EOR, &c::ABS, 3, 4),
    ins("LSR", &c::LSR, &c::ABS, 3, 6),
    ins("XXX", &c::XXX, &c::XXX, 0, 0),

    // Row 5
    ins("BVC", &c::BVC, &c::REL, 2, 2),
    ins("EOR", &c::EOR, &c::IND_Y, 2, 5),
    ins("XXX", &c::XXX, &c::XXX, 0, 0),
    ins("XXX", &c::XXX, &c::XXX, 0, 0),
    ins("XXX", &c::XXX, &c::XXX, 0, 0),
    ins("EOR", &c::EOR, &c::ZP_X, 2, 4),
    ins("LSR", &c::LSR, &c::ZP_X, 2, 6),
    ins("XXX", &c::XXX, &c::XXX, 0, 0),
    ins("CLI", &c::CLI, &c::IMP, 1, 2),
    ins("EOR", &c::EOR, &c::ABS_Y, 3, 4),
    ins("XXX", &c::XXX, &c::XXX, 0, 0),
    ins("XXX", &c::XXX, &c::XXX, 0, 0),
    ins("XXX", &c::XXX, &c::XXX, 0, 0),
    ins("EOR", &c::EOR, &c::ABS_X, 3, 4),
    ins("LSR", &c::LSR, &c::ABS_X, 3, 7),
    ins("XXX", &c::XXX, &c::XXX, 0, 0),

    // Row 6
    ins("RTS", &c::RTS, &c::IMP, 1, 6),
    ins("ADC", &c::ADC, &c::IND_X, 2, 6),
    ins("XXX", &c::XXX, &c::XXX, 0, 0),
    ins("XXX", &c::XXX, &c::XXX, 0, 0),
    ins("XXX", &c::XXX, &c::XXX, 0, 0),
    ins("ADC", &c::ADC, &c::ZP, 2, 3),
    ins("ROR", &c::ROR, &c::ZP, 2, 5),
    ins("XXX", &c::XXX, &c::XXX, 0, 0),
    ins("PLA", &c::PLA, &c::IMP, 1, 4),
    ins("ADC", &c::ADC, &c::IMM, 2, 2),
    ins("ROR", &c::ROR, &c::ACC, 1, 2),
    ins("XXX", &c::XXX, &c::XXX, 0, 0),
    ins("JMP", &c::JMP, &c::IND, 3, 5),
    ins("ADC", &c::ADC, &c::ABS, 3, 4),
    ins("ROR", &c::ROR, &c::ABS, 3, 6),
    ins("XXX", &c::XXX, &c::XXX, 0, 0),

    // Row 7
    ins("BVS", &c::BVS, &c::REL, 2, 2),
    ins("ADC", &c::ADC, &c::IND, 2, 5),
    ins("XXX", &c::XXX, &c::XXX, 0, 0),
    ins("XXX", &c::XXX, &c::XXX, 0, 0),
    ins("XXX", &c::XXX, &c::XXX, 0, 0),
    ins("ADC", &c::ADC, &c::ZP_X, 2, 4),
    ins("ROR", &c::ROR, &c::ZP_X, 2, 6),
    ins("XXX", &c::XXX, &c::XXX, 0, 0),
    ins("SEI", &c::SEI, &c::IMP, 1, 2),
    ins("ADC", &c::ADC, &c::ABS_Y, 3, 4),
    ins("XXX", &c::XXX, &c::XXX, 0, 0),
    ins("XXX", &c::XXX, &c::XXX, 0, 0),
    ins("XXX", &c::XXX, &c::XXX, 0, 0),
    ins("ADC", &c::ADC, &c::ABS_X, 3, 4),
    ins("ROR", &c::ROR, &c::ABS_X, 3, 7),
    ins("XXX", &c::XXX, &c::XXX, 0, 0),

    // Row 8
    ins("XXX", &c::XXX, &c::XXX, 0, 0),
    ins("STA", &c::STA, &c::IND_X, 2, 6),
    ins("XXX", &c::XXX, &c::XXX, 0, 0),
    ins("XXX", &c::XXX, &c::XXX, 0, 0),
    ins("STY", &c::STY, &c::ZP, 2, 3),
    ins("STA", &c::STA, &c::ZP, 2, 3),
    ins("STX", &c::STX, &c::ZP, 2, 3),
    ins("XXX", &c::XXX, &c::XXX, 0, 0),
    ins("DEY", &c::DEY, &c::IMP, 1, 2),
    ins("XXX", &c::XXX, &c::XXX, 0, 0),
    ins("TXA", &c::TXA, &c::IMP, 1, 2),
    ins("XXX", &c::XXX, &c::XXX, 0, 0),
    ins("STY", &c::STY, &c::ABS, 3, 4),
    ins("STA", &c::STA, &c::ABS, 3, 4),
    ins("STX", &c::STX, &c::ABS, 3, 4),
    ins("XXX", &c::XXX, &c::XXX, 0, 0),

    // Row 9
    ins("BCC", &c::BCC, &c::REL, 2, 2),
    ins("STA", &c::STA, &c::IND_Y, 2, 6),
    ins("XXX", &c::XXX, &c::XXX, 0, 0),
    ins("XXX", &c::XXX, &c::XXX, 0, 0),
    ins("STY", &c::STY, &c::ZP_X, 2, 4),
    ins("STA", &c::STA, &c::ZP_X, 2, 4),
    ins("STX", &c::STX, &c::ZP_Y, 2, 4),
    ins("XXX", &c::XXX, &c::XXX, 0, 0),
    ins("TYA", &c::TYA, &c::IMP, 1, 2),
    ins("STA", &c::STA, &c::ABS_Y, 3, 5),
    ins("TXS", &c::TXS, &c::IMP, 1, 2),
    ins("XXX", &c::XXX, &c::XXX, 0, 0),
    ins("XXX", &c::XXX, &c::XXX, 0, 0),
    ins("STA", &c::STA, &c::ABS_X, 3, 5),
    ins("XXX", &c::XXX, &c::XXX, 0, 0),
    ins("XXX", &c::XXX, &c::XXX, 0, 0),

    // Row A
    ins("LDY", &c::LDY, &c::IMM, 2, 2),
    ins("LDA", &c::LDA, &c::IND_X, 2, 6),
    ins("LDX", &c::LDX, &c::IMM, 2, 2),
    ins("XXX", &c::XXX, &c::XXX, 0, 0),
    ins("LDY", &c::LDY, &c::ZP, 2, 3),
    ins("LDA", &c::LDA, &c::ZP, 2, 3),
    ins("LDX", &c::LDX, &c::ZP, 2, 3),
    ins("XXX", &c::XXX, &c::XXX, 0, 0),
    ins("TAY", &c::TAY, &c::IMP, 1, 2),
    ins("LDA", &c::LDA, &c::IMM, 2, 2),
    ins("TAX", &c::TAX, &c::IMP, 1, 2),
    ins("XXX", &c::XXX, &c::XXX, 0, 0),
    ins("LDY", &c::LDY, &c::ABS, 3, 4),
    ins("LDA", &c::LDA, &c::ABS, 3, 4),
    ins("LDX", &c::LDX, &c::ABS, 3, 4),
    ins("XXX", &c::XXX, &c::XXX, 0, 0),

    // Row B
    ins("BCS", &c::BCS, &c::REL, 2, 2),
    ins("LDA", &c::LDA, &c::IND_Y, 2, 5),
    ins("XXX", &c::XXX, &c::XXX, 0, 0),
    ins("XXX", &c::XXX, &c::XXX, 0, 0),
    ins("LDY", &c::LDY, &c::ZP_X, 2, 4),
    ins("LDA", &c::LDA, &c::ZP_X, 2, 4),
    ins("LDX", &c::LDX, &c::ZP_Y, 2, 4),
    ins("XXX", &c::XXX, &c::XXX, 0, 0),
    ins("CLV", &c::CLV, &c::IMP, 1, 2),
    ins("LDA", &c::LDA, &c::ABS_Y, 3, 4),
    ins("TSX", &c::TSX, &c::IMP, 1, 2),
    ins("XXX", &c::XXX, &c::XXX, 0, 0),
    ins("LDY", &c::LDY, &c::ABS_X, 3, 4),
    ins("LDA", &c::LDA, &c::ABS_X, 3, 4),
    ins("LDX", &c::LDX, &c::ABS_Y, 3, 4),
    ins("XXX", &c::XXX, &c::XXX, 0, 0),

    // Row C
    ins("CPY", &c::CPY, &c::IMM, 2, 2),
    ins("CMP", &c::CMP, &c::IND_X, 2, 6),
    ins("XXX", &c::XXX, &c::XXX, 0, 0),
    ins("XXX", &c::XXX, &c::XXX, 0, 0),
    ins("CPY", &c::CPY, &c::ZP, 2, 3),
    ins("CMP", &c::CMP, &c::ZP, 2, 3),
    ins("DEC", &c::DEC, &c::ZP, 2, 5),
    ins("XXX", &c::XXX, &c::XXX, 0, 0),
    ins("INY", &c::INY, &c::IMP, 1, 2),
    ins("CMP", &c::CMP, &c::IMM, 2, 2),
    ins("DEX", &c::DEX, &c::IMP, 1, 2),
    ins("XXX", &c::XXX, &c::XXX, 0, 0),
    ins("CPY", &c::CPY, &c::ABS, 3, 4),
    ins("CMP", &c::CMP, &c::ABS, 3, 4),
    ins("DEC", &c::DEC, &c::ABS, 3, 6),
    ins("XXX", &c::XXX, &c::XXX, 0, 0),

    // Row D
    ins("BNE", &c::BNE, &c::REL, 2, 2),
    ins("CMP", &c::CMP, &c::IND_Y, 2, 5),
    ins("XXX", &c::XXX, &c::XXX, 0, 0),
    ins("XXX", &c::XXX, &c::XXX, 0, 0),
    ins("XXX", &c::XXX, &c::XXX, 0, 0),
    ins("CMP", &c::CMP, &c::ZP_X, 2, 4),
    ins("DEC", &c::DEC, &c::ZP_X, 2, 6),
    ins("XXX", &c::XXX, &c::XXX, 0, 0),
    ins("CLD", &c::CLD, &c::IMP, 1, 2),
    ins("CMP", &c::CMP, &c::ABS_Y, 3, 4),
    ins("XXX", &c::XXX, &c::XXX, 0, 0),
    ins("XXX", &c::XXX, &c::XXX, 0, 0),
    ins("XXX", &c::XXX, &c::XXX, 0, 0),
    ins("CMP", &c::CMP, &c::ABS_X, 3, 4),
    ins("DEC", &c::DEC, &c::ABS_X, 3, 7),
    ins("XXX", &c::XXX, &c::XXX, 0, 0),

    // Row E
    ins("CPX", &c::CPX, &c::IMM, 2, 2),
    ins("SBC", &c::SBC, &c::IND_X, 2, 6),
    ins("XXX", &c::XXX, &c::XXX, 0, 0),
    ins("XXX", &c::XXX, &c::XXX, 0, 0),
    ins("CPX", &c::CPX, &c::ZP, 2, 3),
    ins("SBC", &c::SBC, &c::ZP, 2, 3),
    ins("INC", &c::INC, &c::ZP, 2, 5),
    ins("XXX", &c::XXX, &c::XXX, 0, 0),
    ins("INX", &c::INX, &c::IMP, 1, 2),
    ins("SBC", &c::SBC, &c::IMM, 2, 2),
    ins("NOP", &c::NOP, &c::IMM, 1, 2),
    ins("XXX", &c::XXX, &c::XXX, 0, 0),
    ins("CPX", &c::CPX, &c::ABS, 3, 4),
    ins("SBC", &c::SBC, &c::ABS, 3, 4),
    ins("INC", &c::INC, &c::ABS, 3, 6),
    ins("XXX", &c::XXX, &c::XXX, 0, 0),

    // Row F
    ins("BEQ", &c::BEQ, &c::REL, 2, 2),
    ins("SBC", &c::SBC, &c::IND_Y, 2, 5),
    ins("XXX", &c::XXX, &c::XXX, 0, 0),
    ins("XXX", &c::XXX, &c::XXX, 0, 0),
    ins("XXX", &c::XXX, &c::XXX, 0, 0),
    ins("SBC", &c::SBC, &c::ZP_X, 2, 4),
    ins("INC", &c::INC, &c::ZP_X, 2, 6),
    ins("XXX", &c::XXX, &c::XXX, 0, 0),
    ins("SED", &c::SED, &c::IMP, 1, 2),
    ins("SBC", &c::SBC, &c::ABS_Y, 3, 4),
    ins("XXX", &c::XXX, &c::XXX, 0, 0),
    ins("XXX", &c::XXX, &c::XXX, 0, 0),
    ins("XXX", &c::XXX, &c::XXX, 0, 0),
    ins("SBC", &c::SBC, &c::ABS_X, 3, 4),
    ins("INC", &c::INC, &c::ABS_X, 3, 7),
    ins("XXX", &c::XXX, &c::XXX, 0, 0),
};

#endif