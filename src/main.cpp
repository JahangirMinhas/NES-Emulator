#include "..\includes\m_bus.h"

MBus* m_bus;

// Draw CPU Registers
void draw_reg(){
    std::cout << " CPU REGISTERS" << std::endl;
    std::cout << " -------------" << std::endl;
    printf("  ACC: %d\n", m_bus->cpu->getAcc());
    printf("  X: %d\n", m_bus->cpu->getX());
    printf("  Y: %d\n\n", m_bus->cpu->getY());
    printf("  PC: %04X\n", m_bus->cpu->getPc());
    printf("  SP: %04X\n\n", m_bus->cpu->getSp());
    printf("  N V - B D I Z C\n  ");
    for (int i = 7; i >= 0; i--) {
        uint8_t bit = (m_bus->cpu->getSr() & (1 << i)) ? 1 : 0;
        printf("%d ", bit);
    }
    printf(" Cycles Remaining: %d", m_bus->cpu->get_cyc());
    printf("\n\n\n");
}

// Draw a page in ram
void draw_pg(uint8_t pg){
    // Print column headers
    std::cout << "  ";
    for (uint8_t col = 0; col < 16; col++){
        printf("%01X  ", col);
    }
    std::cout << std::endl;

    // Initialize for row headers
    uint8_t row = 0;
    printf("%01X ", row);
    row++;

    // Print every byte in pg with row header
    for (uint16_t addr = pg << 8; addr <= (uint16_t)(pg << 8) + 0xFF; addr++){
        uint8_t byte = *m_bus->read(addr);
        printf("%02X ", byte);
        if((uint8_t)(addr & 0xF) == 0xF && (uint8_t)(addr & 0xFF) != 0xFF){
            printf("\n%01X ", row);
            row++;
        }
    }
}

void draw_state(){
    printf("\n 6502 EMULATOR (FOR DEBUGGING PURPOSES)\n -------------\n");
    printf(" Commands: ");
    printf("\n  S: Step\n  R: Reset\n  Q: Quit\n\n\n");

    draw_reg();

    printf(" ZERO PAGE\n-------------\n");
    draw_pg(0x00);
    printf("\n\n STACK (PAGE 0x01)\n-------------\n");
    draw_pg(0x01);
    printf("\n\n PROGRAM (PAGE 0x80)\n-------------\n");
    draw_pg(0x80);
    printf("\n\n Enter a command: ");
}

void start(){
    draw_state();
    while(true){
        char in = std::cin.get();
        switch(in){
            case 's':
                m_bus->cpu->exec_ins();
                draw_state();
                break;
            case 'q':
                return;
        }
    }
}

void load_prog(char* prog){
    char *ptr = strtok (prog, " ");
    m_bus->cpu->setPc(0x8000);
    uint16_t addr = 0x8000;
    while (ptr != NULL) {
        m_bus->write(std::stoul(ptr, nullptr, 16), addr);
        ptr = strtok(NULL, " ");
        addr++;
    }
}

int main(){
    m_bus = new MBus();
    m_bus->write(0x05, 0x0032);
    m_bus->write(0xFF, 0x00FF);
    m_bus->write(0x74, 0x0104);
    char* prog = new char[200];
    std::strcpy(prog, "7D FF 00");
    load_prog(prog);
    start();
    return 0;
}