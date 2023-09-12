#ifndef CHIP_8_H
#define CHIP_8_H

#include <stdint.h>
#include <array>

#define STACK_SIZE 16
#define RAM_SIZE 4096
#define NUM_REGISTERS 16
#define ROM_START_ADDRESS 0x200 // 512
#define DISPLAY_HEIGHT 32
#define DISPLAY_WIDTH 64
#define DISPLAY_SCALE 16
#define DISPLAY_SIZE DISPLAY_HEIGHT *DISPLAY_WIDTH
#define NUM_KEYS 16
#define FONT_ARRAY_SIZE 80

class Chip8
{
public:
    Chip8();
    // ~Chip8();

    bool loadROM(const char *filepath);
    uint16_t getOpcode();
    void decode(uint16_t opcode);

    uint8_t     keys[NUM_KEYS];
    uint8_t     display[DISPLAY_SIZE];

private:
    uint16_t    stack[STACK_SIZE]; // stack
    uint16_t    sp;                // stack pointer

    uint8_t     ram[RAM_SIZE];    // ram memory
    uint8_t     V[NUM_REGISTERS]; // registers

    uint16_t    pc; // process counter
    uint16_t    I;  // memory index register

    uint8_t     sound_timer, delay_timer;

    uint16_t    opcode;

    const unsigned char FONT[FONT_ARRAY_SIZE] =
        {
            0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
            0x20, 0x60, 0x20, 0x20, 0x70, // 1
            0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
            0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
            0x90, 0x90, 0xF0, 0x10, 0x10, // 4
            0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
            0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
            0xF0, 0x10, 0x20, 0x40, 0x40, // 7
            0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
            0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
            0xF0, 0x90, 0xF0, 0x90, 0x90, // A
            0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
            0xF0, 0x80, 0x80, 0x80, 0xF0, // C
            0xE0, 0x90, 0x90, 0x90, 0xE0, // D
            0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
            0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };

    void clearRomMemory();

    // Opcode functions
    void OP_00E0(); // Clear Display
    void OP_00EE();
    void OP_1NNN(uint16_t NNN);
    void OP_2NNN(uint16_t NNN);
    void OP_3XNN(uint8_t X, uint8_t NN);
    void OP_4XNN(uint8_t X, uint8_t NN);
    void OP_5XY0(uint8_t X, uint8_t Y);
    void OP_6XNN(uint8_t X, uint8_t NN);
    void OP_7XNN(uint8_t X, uint8_t NN);
    void OP_8XY0(uint8_t X, uint8_t Y);
    void OP_8XY1(uint8_t X, uint8_t Y);
    void OP_8XY2(uint8_t X, uint8_t Y);
    void OP_8XY3(uint8_t X, uint8_t Y);
    void OP_8XY4(uint8_t X, uint8_t Y);
    void OP_8XY5(uint8_t X, uint8_t Y);
    void OP_8XY6(uint8_t X);
    void OP_8XY7(uint8_t X, uint8_t Y);
    void OP_8XYE(uint8_t X);
    void OP_9XY0(uint8_t X, uint8_t Y);
    void OP_ANNN(uint16_t NNN);
    void OP_BNNN(uint16_t NNN);
    void OP_CXNN(uint8_t X, uint8_t NN);
    void OP_DXYN(uint8_t X, uint8_t Y, uint8_t N);
    void OP_EX9E(uint8_t X);
    void OP_EXA1(uint8_t X);
    void OP_FX07(uint8_t X);
    void OP_FX0A(uint8_t X);
    void OP_FX15(uint8_t X);
    void OP_FX18(uint8_t X);
    void OP_FX1E(uint8_t X);
    void OP_FX29(uint8_t X);
    void OP_FX33(uint8_t X);
    void OP_FX55(uint8_t X);
    void OP_FX65(uint8_t X);
};

#endif // CHIP_8_H