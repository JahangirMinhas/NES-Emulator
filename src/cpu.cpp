#include "..\includes\cpu.h"
#include "..\includes\ins.h"

// Construct an instruction
ins::ins(std::string name, void (Cpu::*operation)(), void (Cpu::*addr_mode)(), uint8_t bytes, uint8_t cycles){
    this->name = name;
    this->addr_mode = addr_mode;
    this->operation = operation;
    this->cycles = cycles;
    this->bytes = bytes;
}

// Connect CPU to Main Bus
Cpu::Cpu(MBus* m_bus){
    this->m_bus = m_bus;
}

// Read from Main Bus
uint8_t* Cpu::read(uint16_t addr){
    return m_bus->read(addr);
}

// Write to Main Bus
void Cpu::write(uint8_t data, uint16_t addr){
    m_bus->write(data, addr);
}

// Fetch opcode byte from memory address
uint8_t Cpu::fetch_opcode(uint16_t addr){
    return *read(addr);
}

// Set or unset the flag at pos
void Cpu::setFlag(uint8_t pos, bool val){
    if(val == true){
        sr |= (0x01 << pos);
    }else {
        sr &= ~(0x01 << pos);
    }
}

// Get the bit in flag at pos
uint8_t Cpu::getFlag(uint8_t pos){
    return sr & (0x01 << pos);
}

// Set the Program Counter
void Cpu::setPc(uint16_t n_pc){
    pc = n_pc;
}

// Get the Program Counter
uint16_t Cpu::getPc(){
    return pc;
}

// Get the Program Counter
uint16_t Cpu::getSp(){
    return sp;
}

// Get the x register.
uint8_t Cpu::getX(){
    return x;
}

// Get the x register.
uint8_t Cpu::getY(){
    return y;
}

// Get the x register.
uint8_t Cpu::getAcc(){
    return acc;
}

// Get the status register.
uint8_t Cpu::getSr(){
    return sr;
}

// Get the status register.
uint8_t Cpu::get_cyc(){
    return cycles;
}

// Push a byte to the CPU stack
void Cpu::push_stk(uint8_t byte){
    write(byte, sp);
    sp--;
}

// Pop a byte from CPU stack
uint8_t Cpu::pop_stk(){
    sp++;
    return *read(sp);
}

// Branch to 16 bit address defined from REL addr mode
void Cpu::branch(){
    cycles++;
    if((abs_addr & 0xFF00) != (pc & 0xFF00)){
        cycles ++;
    }
    pc = abs_addr;
}

// Reset the CPU state, including all registers. Set pc to reset vector at $FFFC and $FFFD
void Cpu::reset(){
    pc = ((uint16_t)*read(0xFFFD) << 8) | (uint16_t)*read(0xFFFC);
    acc = 0x00;
    x = 0x00;
    y = 0x00;
    sr = 0x00 | (0x01 << U);
    sp = 0x01FD;

    cycles = 8;
}

// Handles an IRQ interrupt
void Cpu::irq(){
    push_stk((uint8_t)(pc & 0xFF00));
    push_stk((uint8_t)(pc & 0x00FF));

    setFlag(I, true);
    setFlag(B, false);
    push_stk(sr);

    pc = ((uint16_t)*read(0xFFFF) << 8) | (uint16_t)*read(0xFFFE);

    cycles = 7;
}

// Handles an NMI Interrupt
void Cpu::nmi(){
    push_stk((uint8_t)(pc & 0xFF00));
    push_stk((uint8_t)(pc & 0x00FF));

    setFlag(I, true);
    setFlag(B, false);
    push_stk(sr);

    pc = ((uint16_t)*read(0xFFFB) << 8) | (uint16_t)*read(0xFFFA);

    cycles = 8;
}

// Fetches an opcode and executes a single instruction.
void Cpu::exec_ins(){
    if (cycles == 0){
        opcode = fetch_opcode(pc);
        pc++;
        ins lookup = instructions[opcode];
        cycles = lookup.cycles;
        (this->*lookup.addr_mode)();
        (this->*lookup.operation)();
    }
    cycles--;
}

// Operand is value from accumulator register.
void Cpu::ACC(){
    operand = &acc;
}

// Read operand from second byte of instruction.
void Cpu::IMM(){
    operand = read(pc);
    pc++;
}

// Read operand from zero page with offset into zero page extracted from second byte of instruction.
void Cpu::ZP(){
    uint8_t offset = *read(pc);
    pc++;
    operand = read((uint16_t)offset);
}

// Offset from x register can cause overflow. If result exceeds 255 (0xFF), result will wrap around back into zero page.
void Cpu::ZP_X(){
    uint8_t offset = *read(pc);
    pc++;
    operand = read(((uint16_t)(offset + x)) & 0x00FF);
}

// Offset from y register can cause overflow. If result exceeds 255 (0xFF), result will wrap around back into zero page.
void Cpu::ZP_Y(){
    uint8_t offset = *read(pc);
    pc++;
    operand = read(((uint16_t)(offset + y)) & 0x00FF);
}

// Read operand from 16 bit memory address with low byte from second byte of instruction and high byte from third byte of instruction. 
void Cpu::ABS(){
    uint8_t low = *read(pc);
    pc++;
    uint8_t high = *read(pc);
    pc++;
    abs_addr = (uint16_t)((high << 8) | low);
    operand = read(abs_addr);
}

// Read operand using absolute addressing with offset from register x. In the case where page boundry is crossed, increment cycles.
void Cpu::ABS_X(){
    uint8_t low = *read(pc);
    pc++;
    uint8_t high = *read(pc);
    pc++;
    abs_addr = (uint16_t)((high << 8) | low);

    if (((abs_addr + x) & 0xFF00) != (abs_addr & 0xFF00)) cycles++;
    abs_addr += x;
    operand = read(abs_addr);
}

// Read operand using absolute addressing with offset from register y. In the case where page boundry is crossed, increment cycles.
void Cpu::ABS_Y(){
    uint8_t low = *read(pc);
    pc++;
    uint8_t high = *read(pc);
    pc++;
    abs_addr = (uint16_t)((high << 8) | low);

    if (((abs_addr + y) & 0xFF00) != (abs_addr & 0xFF00)) cycles++;
    abs_addr += y;
    operand = read(abs_addr);
}

// No additional data needs to be read with implied addressing as only operation needs to be executed.
void Cpu::IMP(){
    return;
}

// The signed offset value is first read at pc which tells us how far forward/backward (-128 or 127) we require to jump in memory. The target address is found by
// combining the pc register and offset.
void Cpu::REL(){
    int16_t offset = (uint16_t)*read(pc);
    pc++;
    if(offset & 0x80){
        offset |= 0xFF00;
    }
    abs_addr = pc + offset;
}

// Follow absolute addressing by getting low and high byte from instruction. This forms a memory location where low byte of taget address is. Next memory location after that
// contains high byte of target. Combine to get complete 16bit target memory address where operand can be read.
void Cpu::IND(){
    uint8_t low = *read(pc);
    pc++;
    uint8_t high = *read(pc);
    pc++;
    uint16_t addr = (uint16_t)((high << 8) | low);

    if(addr == 0x00FF){
        abs_addr = (*read(addr & 0xFF00) << 8) | *read(addr);
    }else{
        abs_addr = (*read(addr + 1) << 8) | *read(addr);
    }

    operand = read(abs_addr);
}

// Second byte from instruction is added with the x register to form an offset into ZP where low byte of target address can be read. The next memory location within ZP
// contains the high byte of target. Low byte and high byte are combined to form target address where operand can be read.
void Cpu::IND_X(){
    uint8_t offset = *read(pc);
    pc++;
    uint16_t addr = ((uint16_t)(offset + x)) & 0x00FF;

    abs_addr = (*read(addr + 1) << 8) | *read(addr);
    operand = read(abs_addr);
}

// Second byte from instruction is an offset into ZP where low byte of target address can be read. The next memory location within ZP contains the high byte of
// target. Low byte and high byte are combined and the y register is added to the combined address to form target address where operand can be read.
// In the case where page boundry is crossed, increment cycles.
void Cpu::IND_Y(){
    uint8_t offset = *read(pc);
    pc++;

    abs_addr = (*read((offset + 1) & 0x00FF) << 8) | *read(offset & 0x00FF);
    if (((abs_addr + (uint16_t)y) & 0xFF00) != (abs_addr & 0xFF00)) cycles++;

    abs_addr += y;
    operand = read(abs_addr);
}

void Cpu::ADC(){
    uint16_t tmp = (uint16_t)acc + (uint16_t)(*operand) + (uint16_t)getFlag(0);
    setFlag(C, tmp > 0xFF);
    setFlag(N, tmp & 0x80);
    setFlag(Z, (tmp & 0x00FF) == 0x0000);
    if(((acc & 0x80) == 0) && ((*operand & 0x80) == 0) && ((tmp & 0x0080) == 1)){
        setFlag(V, true);
    }else if(((acc & 0x80) == 1) && ((*operand & 0x80) == 1) && ((tmp & 0x0080) == 0)){
        setFlag(V, true);
    }else {
        setFlag(V, false);
    }
    acc = tmp & 0x00FF;
}

void Cpu::AND(){
    acc &= *operand;
    setFlag(Z, acc == 0x00);
    setFlag(N, acc == 0x80);
}

void Cpu::ASL(){
    *operand = *operand << 1;
    setFlag(Z, (*operand) == 0x00);
    setFlag(N, (*operand) == 0x80);
}

void Cpu::BCC(){
    if(getFlag(C) == 0x00){
        branch();
    }
}

void Cpu::BCS(){
    if(getFlag(C) == 0x01){
        branch();
    }
}

void Cpu::BEQ(){
    if(getFlag(Z) == 0x01){
        branch();
    }
}

void Cpu::BIT(){
    uint8_t tmp = acc & *operand;
    setFlag(Z, tmp == 0x00);
    setFlag(V, tmp & (0x01 << V));
    setFlag(N, tmp & 0x80);
}

void Cpu::BMI(){
    if(getFlag(Z) == 0x01){
        branch();
    }
}

void Cpu::BNE(){
    if(getFlag(Z) == 0x00){
        branch();
    }
}

void Cpu::BPL(){
    if(getFlag(N) == 0x00){
        branch();
    }
}

void Cpu::BRK(){
    setFlag(I, true);
    pc++;

    push_stk((uint8_t)(pc & 0xFF00));
    push_stk((uint8_t)(pc & 0x00FF));

    setFlag(B, true);
    push_stk(sr);
    setFlag(I, false);

    pc = (uint16_t)*read(0xFFFE) | ((uint16_t)*read(0xFFFF) << 8);
}

void Cpu::BVC(){
    if(getFlag(V) == 0x00){
        branch();
    }
}

void Cpu::BVS(){
    if(getFlag(V) == 0x01){
        branch();
    }
}

void Cpu::CLC(){
    setFlag(C, false);
}

void Cpu::CLD(){
    setFlag(D, false);
}

void Cpu::CLI(){
    setFlag(I, false);
}

void Cpu::CLV(){
    setFlag(V, false);
}

void Cpu::CMP(){
    setFlag(C, acc >= *operand);
    setFlag(N, (acc - *operand) & 0x80);
    setFlag(Z, (acc - *operand) == 0x00);
}

void Cpu::CPX(){
    setFlag(C, x >= *operand);
    setFlag(N, (x - *operand) & 0x80);
    setFlag(Z, (x - *operand) == 0x00);
}

void Cpu::CPY(){
    setFlag(C, y >= *operand);
    setFlag(N, (y - *operand) & 0x80);
    setFlag(Z, (y - *operand) == 0x00);
}

void Cpu::DEC(){
    (*operand)--;
    setFlag(N, (*operand) & 0x80);
    setFlag(Z, (*operand) == 0x00);
}

void Cpu::DEX(){
    x--;
    setFlag(N, x & 0x80);
    setFlag(Z, x == 0x00);
}

void Cpu::DEY(){
    y--;
    setFlag(N, y & 0x80);
    setFlag(Z, y == 0x00);
}

void Cpu::EOR(){
    acc = acc ^ *operand;
    setFlag(N, acc & 0x80);
    setFlag(Z, acc == 0x00);
}

void Cpu::INC(){
    (*operand)++;
    setFlag(N, (*operand) & 0x80);
    setFlag(Z, (*operand) == 0x00);
}

void Cpu::INX(){
    x++;
    setFlag(N, x & 0x80);
    setFlag(Z, x == 0x00);
}

void Cpu::INY(){
    y++;
    setFlag(N, y & 0x80);
    setFlag(Z, y == 0x00);
}

void Cpu::JMP(){
    pc = abs_addr;
}

void Cpu::JSR(){
    push_stk((uint8_t)(pc & 0xFF00));
    push_stk((uint8_t)(pc & 0x00FF));
    pc = abs_addr;
}

void Cpu::LDA(){
    acc = *operand;
    setFlag(N, acc & 0x80);
    setFlag(Z, acc == 0x00);
}

void Cpu::LDX(){
    x = *operand;
    setFlag(N, x & 0x80);
    setFlag(Z, x == 0x00);
}

void Cpu::LDY(){
    y = *operand;
    setFlag(N, y & 0x80);
    setFlag(Z, y == 0x00);
}

void Cpu::LSR(){
    setFlag(C, (*operand) == 0x01);
    *operand = *operand >> 1;
    setFlag(Z, (*operand) == 0x00);
    setFlag(N, (*operand) == 0x80);
}

void Cpu::NOP(){
    return;
}

void Cpu::ORA(){
    acc |= *operand;
    setFlag(Z, acc == 0x00);
    setFlag(N, acc == 0x80);
}

void Cpu::PHA(){
    push_stk(acc);
}

void Cpu::PHP(){
    setFlag(B, true);
    push_stk(sr);
    setFlag(B, false);
}

void Cpu::PLA(){
    acc = pop_stk();
    setFlag(Z, acc == 0x00);
    setFlag(N, acc == 0x80);
}

void Cpu::PLP(){
    sr = pop_stk();
}

void Cpu::ROL(){
    uint16_t tmp = (uint16_t)*operand;
    tmp = (tmp << 1) | getFlag(C);
    setFlag(C, tmp & 0xFF00);
	setFlag(N, tmp & 0x0080);
    setFlag(Z, (tmp & 0x00FF) == 0);
    *operand = (uint8_t)(tmp & 0x00FF);
}

void Cpu::ROR(){
    uint16_t tmp = (uint16_t)*operand;
    tmp = (getFlag(C) << 7) | (tmp >> 1);
    setFlag(C, tmp & 0xFF00);
	setFlag(N, tmp & 0x0080);
    setFlag(Z, (tmp & 0x00FF) == 0);
    *operand = (uint8_t)(tmp & 0x00FF);
}

void Cpu::RTI(){
    sr = pop_stk();
    setFlag(B, false);
    pc = (int16_t)pop_stk() | ((uint16_t)pop_stk() << 8);
}

void Cpu::RTS(){
    pc = (int16_t)pop_stk() | ((uint16_t)pop_stk() << 8);
    pc++;
}

void Cpu::SBC(){
    uint16_t data = (uint16_t)(*operand) ^ 0x00FF;
    uint16_t tmp = (uint16_t)acc + data + (uint16_t)getFlag(0);
    setFlag(C, tmp > 0xFF);
    setFlag(N, tmp & 0x80);
    setFlag(Z, (tmp & 0x00FF) == 0x0000);
    if(((acc & 0x80) == 0) && ((*operand & 0x80) == 0) && ((tmp & 0x0080) == 1)){
        setFlag(V, true);
    }else if(((acc & 0x80) == 1) && ((*operand & 0x80) == 1) && ((tmp & 0x0080) == 0)){
        setFlag(V, true);
    }else {
        setFlag(V, false);
    }
    acc = tmp & 0x00FF;
}

void Cpu::SEC(){
    setFlag(C, true);
}

void Cpu::SED(){
    setFlag(D, true);
}

void Cpu::SEI(){
    setFlag(I, true);
}

void Cpu::STA(){
    write(acc, abs_addr);
}

void Cpu::STX(){
    write(x, abs_addr);
}

void Cpu::STY(){
    write(y, abs_addr);
}

void Cpu::TAX(){
    x = acc;
    setFlag(Z, x == 0x00);
    setFlag(N, x == 0x80);
}

void Cpu::TAY(){
    y = acc;
    setFlag(Z, y == 0x00);
    setFlag(N, y == 0x80);
}

void Cpu::TSX(){
    x = sp;
    setFlag(Z, x == 0x00);
    setFlag(N, x == 0x80);
}

void Cpu::TXA(){
    acc = x;
    setFlag(Z, acc == 0x00);
    setFlag(N, acc == 0x80);
}

void Cpu::TXS(){
    sp = x;
}

void Cpu::TYA(){
    acc = y;
    setFlag(Z, acc == 0x00);
    setFlag(N, acc == 0x80);
}

void Cpu::XXX()
{
	return;
}