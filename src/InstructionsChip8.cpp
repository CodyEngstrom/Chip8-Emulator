#include "../inc/chip8.h"
#include <string.h>
#include <iostream>
#include <random>

// Clear Display
void Chip8::OP_00E0()
{
    memset(display, 0x0, DISPLAY_SIZE);
};

// Exit a subroutine on the stack
void Chip8::OP_00EE()
{
    if (sp == 0)
    {
        std::cerr << "Return when stack pointer equals 0." << std::endl;
    }
    else
    {
        sp--;
        pc = stack[sp];
    }
};

// Jump to memory address NNN in ram
void Chip8::OP_1NNN(uint16_t NNN)
{
    pc = NNN;
    pc -= 2;
};

// Call a subroutine on the stack
void Chip8::OP_2NNN(uint16_t NNN)
{
    if (sp >= STACK_SIZE)
    {
        std::cerr << "chip8 stack overflow." << std::endl;
    }
    else
    {
        stack[sp] = pc;
        sp++;
        pc = NNN;
        pc -= 2; // this keeps pc in same spot since pc += 2 in decode function
    }
};

// Skip next instruction if VX == NN
void Chip8::OP_3XNN(uint8_t X, uint8_t NN)
{
    if (V[X] == NN)
    {
        pc += 2;
    }
};

// Skip next instruction if VX != NN
void Chip8::OP_4XNN(uint8_t X, uint8_t NN)
{
    if (V[X] != NN)
    {
        pc += 2;
    }
};

// Skip the next instruction if VX == VY
void Chip8::OP_5XY0(uint8_t X, uint8_t Y)
{
    if (V[X] == V[Y])
    {
        pc += 2;
    }
};

// Store NN in VX
void Chip8::OP_6XNN(uint8_t X, uint8_t NN)
{
    std::cout << "OP_6XNN: " << static_cast<unsigned>(X) << " " << static_cast<unsigned>(NN) << std::endl;
    V[X] = NN;
};

// Add NN to VX
void Chip8::OP_7XNN(uint8_t X, uint8_t NN)
{
    V[X] += NN;
};

// Store the value of VY in VX
void Chip8::OP_8XY0(uint8_t X, uint8_t Y)
{
    V[X] = V[Y];
};

void Chip8::OP_8XY1(uint8_t X, uint8_t Y)
{
    V[X] |= V[Y];
};

void Chip8::OP_8XY2(uint8_t X, uint8_t Y)
{
    V[X] &= V[Y];
};

void Chip8::OP_8XY3(uint8_t X, uint8_t Y)
{
    V[X] ^= V[Y];
};

void Chip8::OP_8XY4(uint8_t X, uint8_t Y)
{
    if (UINT8_MAX - V[X] < V[Y])
    {
        V[0xF] = 1;
    }
    else
    {
        V[0xF] = 0;
    }
    V[X] += V[Y];
};

void Chip8::OP_8XY5(uint8_t X, uint8_t Y)
{
    if (V[Y] > V[X])
    {
        V[0xF] = 0;
    }
    else
    {
        V[0xF] = 1;
    }
    V[X] -= V[Y];
};

void Chip8::OP_8XY6(uint8_t X)
{
    V[0xF] = V[X] & 0x1;
    V[X] >>= 1;
};

void Chip8::OP_8XY7(uint8_t X, uint8_t Y)
{
    if (V[X] > V[Y])
    {
        V[0xF] = 0;
    }
    else
    {
        V[0xF] = 1;
    }
    V[X] = V[Y] - V[X];
};

void Chip8::OP_8XYE(uint8_t X)
{
    V[0xF] = (V[X] << 15) & 0x1;
    V[X] <<= 1;
};

void Chip8::OP_9XY0(uint8_t X, uint8_t Y)
{
    if (V[X] != V[Y])
    {
        pc += 2;
    }
};

void Chip8::OP_ANNN(uint16_t NNN)
{
    I = NNN;
};

void Chip8::OP_BNNN(uint16_t NNN)
{
    pc = NNN + V[0];
    pc -= 2; // Adding pc += 2 in decode function
};

void Chip8::OP_CXNN(uint8_t X, uint8_t NN)
{
    V[X] = (rand() % (UINT8_MAX + 1)) & NN;
};

void Chip8::OP_DXYN(uint8_t X, uint8_t Y, uint8_t N)
{
    uint8_t x = V[X];
    uint8_t y = V[Y];
    V[0xF] = 0;

    for (uint8_t col = 0; col < N; col++)
    {
        uint8_t pixelData = ram[I + col];
        for (uint8_t row = 0; row < 8; row++)
        {
            if ((pixelData & (0x80 >> row)) != 0)
            {
                uint8_t px = (x + row);
                uint8_t py = (y + col);
                if (display[px + (py * 64)] == 1)
                {
                    V[0xF] = 1;
                }
                display[px + (py * 64)] ^= 1;
            }
        }
    }
};

void Chip8::OP_EX9E(uint8_t X)
{
    if (keys[V[X]] != 0)
    {
        pc += 2;
    }
};

void Chip8::OP_EXA1(uint8_t X)
{
    if (!keys[V[X]] == 0)
    {
        pc += 2;
    }
};

void Chip8::OP_FX07(uint8_t X)
{
    V[X] = delay_timer;
};

void Chip8::OP_FX0A(uint8_t X)
{

    for (int i = 0; i < NUM_KEYS; i++)
    {
        if (keys[i] != 0)
        {
            V[X] = i;
            return;
        }
    }
    pc -= 2; // Loop until key pressed
};

void Chip8::OP_FX15(uint8_t X)
{
    delay_timer = V[X];
};

void Chip8::OP_FX18(uint8_t X)
{
    sound_timer = V[X];
};

// TODO
void Chip8::OP_FX1E(uint8_t X)
{
    if (I + V[X] > 0xFFF)
    {
        V[0xF] = 1;
    }
    else
    {
        V[0xF] = 0;
    }
    I += V[X];
};

void Chip8::OP_FX29(uint8_t X)
{
    I = V[X] * 0x5;
};

void Chip8::OP_FX33(uint8_t X)
{
    ram[I] = V[X] / 100;
    ram[I + 1] = (V[X] / 10) % 10;
    ram[I + 2] = V[X] % 10;
};

void Chip8::OP_FX55(uint8_t X)
{
    for (size_t i = 0; i <= X; i++)
    {
        ram[I + i] = V[i];
        I++;
    }
};

void Chip8::OP_FX65(uint8_t X)
{
    for (size_t i = 0; i <= X; i++)
    {
        V[i] = ram[I + i];
        I++;
    }
};