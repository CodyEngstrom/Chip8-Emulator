#include "../inc/chip8.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <random>

Chip8::Chip8()
{
    memset(stack, 0x0, STACK_SIZE * sizeof(uint16_t));
    memset(ram, 0x0, RAM_SIZE * sizeof(uint8_t));
    memset(V, 0x0, NUM_REGISTERS * sizeof(uint8_t));
    memset(display, 0x0, DISPLAY_SIZE * sizeof(uint8_t));
    memset(keys, 0x0, NUM_KEYS * sizeof(uint8_t));
    pc = ROM_START_ADDRESS;
    sp = 0;
    I = 0;
    opcode = 0;
    sound_timer = 0;
    delay_timer = 0;

    srand(time(0));

    for (size_t i = 0; i < FONT_ARRAY_SIZE; i++)
    {
        ram[i] = FONT[i];
    }
}

uint16_t Chip8::getOpcode()
{
    uint16_t opcode = ram[pc] << 8 | ram[pc + 1];
    std::cout << "Address 0x" << std::hex << pc << ": 0x" << std::hex << static_cast<unsigned>(opcode) << " "
              << "pc: " << pc << " "
              << "sp: " << sp << " I: " << I << std::endl;
    for (int i = 0; i < 16; i++)
    {
        std::cout << static_cast<unsigned>(V[i]) << ", ";
    }
    std::cout << std::endl;
    return opcode;
}

void Chip8::decode(uint16_t opcode)
{

    switch ((opcode >> 12) & 0xF) // First Byte
    {
    case (0x0):
        switch (opcode & 0xF) // Last Byte
        {
        case (0x0):
            OP_00E0();
            break;
        case (0xE):
            OP_00EE();
            break;
        default:
            std::cerr << "bad opcode zero" << std::endl;
            break;
        }
        break;
    case (0x1):
        OP_1NNN(opcode & 0x0FFF);
        break;
    case (0x2):
        OP_2NNN(opcode & 0x0FFF);
        break;
    case (0x3):
        OP_3XNN((opcode >> 8) & 0xF, opcode & 0xFF);
        break;
    case (0x4):
        OP_4XNN((opcode >> 8) & 0xF, opcode & 0xFF);
        break;
    case (0x5):
        OP_5XY0((opcode >> 8) & 0xF, (opcode >> 4) & 0xF);
        break;
    case (0x6):
        OP_6XNN((opcode >> 8) & 0xF, opcode & 0xFF);
        break;
    case (0x7):
        OP_7XNN((opcode >> 8) & 0xF, opcode & 0xFF);
        break;
    case (0x8):
        switch (opcode & 0xF) // Last Byte
        {
        case (0x0):
            OP_8XY0((opcode >> 8) & 0xF, (opcode >> 4) & 0xF);
            break;
        case (0x1):
            OP_8XY1((opcode >> 8) & 0xF, (opcode >> 4) & 0xF);
            break;
        case (0x2):
            OP_8XY2((opcode >> 8) & 0xF, (opcode >> 4) & 0xF);
            break;
        case (0x3):
            OP_8XY3((opcode >> 8) & 0xF, (opcode >> 4) & 0xF);
            break;
        case (0x4):
            OP_8XY4((opcode >> 8) & 0xF, (opcode >> 4) & 0xF);
            break;
        case (0x5):
            OP_8XY5((opcode >> 8) & 0xF, (opcode >> 4) & 0xF);
            break;
        case (0x6):
            OP_8XY6((opcode >> 8) & 0xF);
            break;
        case (0x7):
            OP_8XY7((opcode >> 8) & 0xF, (opcode >> 4) & 0xF);
            break;
        case (0xE):
            OP_8XYE((opcode >> 8) & 0xF);
            break;

        default:
            std::cerr << "bad opcode 8" << std::endl;
            break;
        }
        break;
    case (0x9):
        OP_9XY0((opcode >> 8) & 0xF, (opcode >> 4) & 0xF);
        break;
    case (0xA):
        OP_ANNN(opcode & 0x0FFF);
        break;
    case (0xB):
        OP_BNNN(opcode & 0x0FFF);
        break;
    case (0xC):
        OP_CXNN((opcode >> 8) & 0xF, opcode & 0xFF);
        break;
    case (0xD):
        OP_DXYN((opcode >> 8) & 0xF, (opcode >> 4) & 0xF, opcode & 0xF);
        break;
    case (0xE):
        switch (opcode & 0xF) // Last Byte
        {
        case (0xE):
            OP_EX9E((opcode >> 8) & 0xF);
            break;
        case (0x1):
            OP_EXA1((opcode >> 8) & 0xF);
            break;
        default:
            std::cerr << "bad opcode E" << std::endl;
            break;
        }
        break;
    case (0xF):
        switch (opcode & 0xFF) // Last Two Bytes
        {
        case (0x07):
            OP_FX07((opcode >> 8) & 0xF);
            break;
        case (0x0A):
            OP_FX0A((opcode >> 8) & 0xF);
            break;
        case (0x15):
            OP_FX15((opcode >> 8) & 0xF);
            break;
        case (0x18):
            OP_FX18((opcode >> 8) & 0xF);
            break;
        case (0x1E):
            OP_FX1E((opcode >> 8) & 0xF);
            break;
        case (0x29):
            OP_FX29((opcode >> 8) & 0xF);
            break;
        case (0x33):
            OP_FX33((opcode >> 8) & 0xF);
            break;
        case (0x55):
            OP_FX55((opcode >> 8) & 0xF);
            break;
        case (0x65):
            OP_FX65((opcode >> 8) & 0xF);
            break;
        default:
            std::cerr << "bad opcode F" << std::endl;
            break;
        }
        break;
    default:
        std::cerr << "bad opcode end" << std::endl;
        break;
    }

    // TODO Fix delay_timer and sound_timer to decrement on their own cycle
    if (delay_timer > 0)
    {
        delay_timer--;
    }
    if (sound_timer > 0)
    {
        sound_timer--;
    }

    // Increment process counter after every instruction
    pc += 2;
}

void Chip8::clearRomMemory()
{
    memset(ram + ROM_START_ADDRESS, 0, RAM_SIZE - ROM_START_ADDRESS);
}

bool Chip8::loadROM(const char *filepath)
{

    // Clear Rom memory in ram before running
    clearRomMemory();

    std::ifstream rom_file(filepath, std::ios::binary | std::ios::ate);
    if (!rom_file.is_open())
    {
        return true;
    }
    std::streampos file_size = rom_file.tellg();
    rom_file.seekg(0, std::ios::beg);

    std::vector<uint8_t> rom_data(file_size);
    rom_file.read(reinterpret_cast<char *>(rom_data.data()), file_size);
    rom_file.close();

    std::cout << "ROM Size: " << file_size << std::endl;

    for (size_t i = 0; i < file_size; i++)
    {
        ram[ROM_START_ADDRESS + i] = rom_data[i];
    }

    return false;
}
