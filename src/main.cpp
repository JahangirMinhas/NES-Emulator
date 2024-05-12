#include "..\includes\bus.h"

Bus* bus;

// Draw CPU Registers
void draw_reg(){
    std::cout << " CPU REGISTERS" << std::endl;
    std::cout << " -------------" << std::endl;
    printf("  ACC: %d\n", bus->cpu->acc);
    printf("  X: %d\n", bus->cpu->x);
    printf("  Y: %d\n\n", bus->cpu->y);
    printf("  PC: %04X\n", bus->cpu->pc);
    printf("  SP: %04X\n\n", bus->cpu->sp);
    printf("  N V - B D I Z C\n  ");
    for (int i = 7; i >= 0; i--) {
        uint8_t bit = (bus->cpu->sr & (1 << i)) ? 1 : 0;
        printf("%d ", bit);
    }
    printf(" Cycles Remaining: %d", bus->cpu->cycles);
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
        uint8_t byte = *bus->read(addr);
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
                bus->cpu->exec_ins();
                draw_state();
                break;
            case 'q':
                return;
        }
    }
}

void load_prog(char* prog){
    char *ptr = strtok (prog, " ");
    bus->cpu->pc = 0x8000;
    uint16_t addr = 0x8000;
    while (ptr != NULL) {
        bus->write(std::stoul(ptr, nullptr, 16), addr);
        ptr = strtok(NULL, " ");
        addr++;
    }
}

int main(){
    bus = new Bus();
    bus->write(0x05, 0x0032);
    bus->write(0xFF, 0x00FF);
    bus->write(0x74, 0x0104);
    bus->cpu->x = 0x05;
    char* prog = new char[200];
    std::strcpy(prog, "7D FF 00");
    load_prog(prog);
    start();
}