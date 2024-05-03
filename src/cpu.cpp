#include "cpu.h"
#include "instruct.h"

instruct::instruct(std::string name, void (Cpu::*operation)(), void (Cpu::*addr_mode)(), uint8_t bytes, uint8_t cycles){
    this->name = name;
    this->addr_mode = addr_mode;
    this->operation = operation;
    this->cycles = cycles;
    this->bytes = bytes;
}

Cpu::Cpu(Bus* bus){
    this->bus = bus;
}

/*
    Set a cpu flag at pos. Val == True (Set Bit) and Val == False (Clear Bit).
    Pos:
        0: Carry
        1: Zero
        2: IRQ Interrupt
        3: Decimal Mode
        4: BRK Command
        5: Unused
        6: Overflow
        7: Negative
*/
void Cpu::setFlag(uint8_t pos, bool val){
    if(val == true){
        flags |= (0x01 << pos);
    }else {
        flags &= ~(0x01 << pos);
    }
}

// Get the bit in flag at pos.
uint8_t Cpu::getFlag(uint8_t pos){
    return flags & (0x01 << pos);
}

// Fetch opcode byte from memory address.
uint8_t Cpu::fetch_opcode(uint16_t addr){
    return bus->read(addr);
}

// One cycle consists of fetching opcode, using it to index opcode matrix and calling respective addressing mode/operation.
void Cpu::cycle(){
    if (cycles == 0){
        opcode = fetch_opcode(pc);
        pc++;
        instruct lookup = instructions[opcode];
        cycles = lookup.cycles;
        (this->*lookup.addr_mode)();
        (this->*lookup.operation)();
    }
    cycles--;
}

// Operand is value from accumulator register.
void Cpu::ACC(){
    operand = acc;
}

// Read operand from second byte of instruction.
void Cpu::IMM(){
    operand = bus->read(pc);
    pc++;
}

// Read operand from zero page with offset into zero page extracted from second byte of instruction.
void Cpu::ZP(){
    low = bus->read(pc);
    pc++;
    operand = bus->read((uint16_t)low);
}

// Offset from x register can cause overflow. If result exceeds 255 (0xFF), result will wrap around back into zero page.
void Cpu::ZP_X(){
    low = bus->read(pc);
    pc++;
    operand = bus->read(((uint16_t)(low + x)) & 0x00FF);
}

// Offset from y register can cause overflow. If result exceeds 255 (0xFF), result will wrap around back into zero page.
void Cpu::ZP_Y(){
    low = bus->read(pc);
    pc++;
    operand = bus->read(((uint16_t)(low + y)) & 0x00FF);
}

// Read operand from 16 bit memory address with low byte from second byte of instruction and high byte from third byte of instruction. 
void Cpu::ABS(){
    low = bus->read(pc);
    pc++;
    high = bus->read(pc);
    pc++;
    new_addr = (uint16_t)((high << 8) | low);
    operand = bus->read(new_addr);
}

// Read operand using absolute addressing with offset from register x. In the case where page boundry is crossed, increment cycles.
void Cpu::ABS_X(){
    low = bus->read(pc);
    pc++;
    high = bus->read(pc);
    pc++;
    new_addr = (uint16_t)((high << 8) | low);

    uint8_t start_page = new_addr & 0xFF00;
    new_addr += (uint16_t)x;
    uint8_t end_page = new_addr & 0xFF00;

    if (start_page != end_page) cycles++;
    operand = bus->read(new_addr);
}

// Read operand using absolute addressing with offset from register y. In the case where page boundry is crossed, increment cycles.
void Cpu::ABS_Y(){
    low = bus->read(pc);
    pc++;
    high = bus->read(pc);
    pc++;
    new_addr = (uint16_t)((high << 8) | low);

    uint8_t start_page = new_addr & 0xFF00;
    new_addr += y;
    uint8_t end_page = new_addr & 0xFF00;

    if (start_page != end_page) cycles++;
    operand = bus->read(new_addr);
}

// No additional data needs to be read with implied addressing as only operation needs to be executed.
void Cpu::IMP(){
    return;
}

// The signed offset value is first read at pc which tells us how far forward/backward (-128 or 127) we require to jump in memory. The target address is found by
// combining the pc register and offset. The operand is then read at this target address.
void Cpu::REL(){
    int8_t offset = bus->read(pc);
    pc++;
    new_addr = pc + (uint16_t)offset;
    operand = bus->read(new_addr);
}

// Follow absolute addressing by getting low and high byte from instruction. This forms a memory location where low byte of taget address is. Next memory location after that
// contains high byte of target. Combine to get complete 16bit target memory address where operand can be read.
void Cpu::IND(){
    low = bus->read(pc);
    pc++;
    high = bus->read(pc);
    pc++;
    new_addr = (uint16_t)((high << 8) | low);
    uint8_t low_target = bus->read(new_addr);
    uint8_t high_target = bus->read(new_addr + 1);
    new_addr = (uint16_t)((high_target << 8) | low_target);
    operand = bus->read(new_addr);
}

// Second byte from instruction is added with the x register to form an offset into ZP where low byte of target address can be read. The next memory location within ZP
// contains the high byte of target. Low byte and high byte are combined to form target address where operand can be read.
void Cpu::IND_X(){
    low = bus->read(pc);
    pc++;
    new_addr = ((uint16_t)(low + x)) & 0x00FF;
    uint8_t low_target = bus->read(new_addr);
    uint8_t high_target = bus->read(new_addr + 1) & 0x00FF;
    new_addr = (uint16_t)((high_target << 8) | low_target);
    operand = bus->read(new_addr);
}

// Second byte from instruction is an offset into ZP where low byte of target address can be read. The next memory location within ZP contains the high byte of
// target. Low byte and high byte are combined and the y register is added to the combined address to form target address where operand can be read.
// In the case where page boundry is crossed, increment cycles.
void Cpu::IND_Y(){
    low = bus->read(pc);
    pc++;
    new_addr = ((uint16_t)low) & 0x00FF;
    uint8_t low_target = bus->read(new_addr);
    uint8_t high_target = bus->read(new_addr + 1) & 0x00FF;
    new_addr = (uint16_t)((high_target << 8) | low_target);
    uint8_t start_page = new_addr & 0xFF00;
    new_addr += y;
    uint8_t end_page = new_addr & 0xFF00;
    if (start_page != end_page) cycles++;
    operand = bus->read(new_addr);
}

void Cpu::ADC(){
    uint16_t add = (uint16_t)acc + (uint16_t)operand + (uint16_t)getFlag(0);
    setFlag(0, add > 0xFF);
    setFlag(7, add & 0x80);
    setFlag(1, (add & 0x00FF) == 0x0000);
    if((acc & 0x80 == 0) && (operand & 0x80 == 0) && (add & 0x0080 == 1)){
        setFlag(6, true);
    }else if((acc & 0x80 == 1) && (operand & 0x80 == 1) && (add & 0x0080 == 0)){
        setFlag(6, true);
    }else {
        setFlag(6, false);
    }
    acc = add & 0x00FF;
}

void Cpu::AND(){
    acc &= operand;
}