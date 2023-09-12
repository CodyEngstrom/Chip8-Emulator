#include <iostream>
#include <SDL2/SDL.h>

#include "../inc/chip8.h"

void render(SDL_Renderer *renderer, uint8_t display[], int displayWidth, int displayHeight, int scale)
{
    for (int i = 0; i < displayWidth * displayHeight; i++)
    {
        int x = i % displayWidth;
        int y = i / displayWidth;

        int elementValue = display[i];

        int screenX = x * scale;
        int screenY = y * scale;

        SDL_Rect destRect = {screenX, screenY, scale, scale};

        if (elementValue == 1)
        {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderFillRect(renderer, &destRect);
        }
    }
}

int main(int argc, char *argv[])
{

    if (argc != 2)
    {
        std::cout << "Use: ./chip8_emulator <filename>" << std::endl;
        return 1;
    }

    char *filename = argv[1];

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    {
        std::cerr << "SDL_Init failed." << std::endl;
        return 1;
    }

    // Create a window for Chip8 64*32 with scale DISPLAY_SCALE
    SDL_Window *window = SDL_CreateWindow("CHIP8 Emulator",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          DISPLAY_WIDTH * DISPLAY_SCALE,
                                          DISPLAY_HEIGHT * DISPLAY_SCALE,
                                          SDL_WINDOW_SHOWN);
    if (!window)
    {
        std::cerr << "SDL_CreateWindow failed." << std::endl;
        return 1;
    }

    // Create a renderer
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        std::cerr << "SDL_CreateRenderer failed." << std::endl;
        return 1;
    }

    // Set the size of the renderer
    SDL_RenderSetLogicalSize(renderer,
                             DISPLAY_WIDTH * DISPLAY_SCALE,
                             DISPLAY_HEIGHT * DISPLAY_SCALE);

    // Scancode keymap
    const SDL_Scancode keyMap[16]{
        SDL_SCANCODE_X, SDL_SCANCODE_1, SDL_SCANCODE_2, SDL_SCANCODE_3,
        SDL_SCANCODE_Q, SDL_SCANCODE_W, SDL_SCANCODE_E, SDL_SCANCODE_A,
        SDL_SCANCODE_S, SDL_SCANCODE_D, SDL_SCANCODE_Z, SDL_SCANCODE_C,
        SDL_SCANCODE_4, SDL_SCANCODE_R, SDL_SCANCODE_F, SDL_SCANCODE_V};

    Chip8 chip8 = Chip8();
    if (chip8.loadROM(filename))
    {
        std::cerr << "ROM file failed to open." << std::endl;
        return 1;
    }

    bool isRunning = true;

    while (isRunning)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event) != 0)
        {
            if (event.type == SDL_QUIT)
            {
                isRunning = false;
            }
            if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP)
            {
                SDL_Scancode scancode = event.key.keysym.scancode;
                bool isKeyDown = (event.type == SDL_KEYDOWN);
                for (size_t i = 0; i < 16; i++)
                {
                    if (scancode == keyMap[i])
                    {
                        chip8.keys[i] = isKeyDown ? 1 : 0;
                    }
                }
            }
        }

        uint16_t opcode = chip8.getOpcode();
        chip8.decode(opcode);

        // These are the only two drawing opcodes
        if ((opcode & 0xF000) == 0xD000 || opcode == 0x00E0)
        {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            render(renderer, chip8.display, DISPLAY_WIDTH, DISPLAY_HEIGHT, DISPLAY_SCALE);
            SDL_RenderPresent(renderer);
        }

        // TODO: fix the delay so that it is a more consistant CPU hz
        // For now change this for different emulators
        SDL_Delay(5);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}