#include <iostream>
#include <fstream>
#include <vector>
#include "../core/Chip8.h"
#include "SDL.h"
#include <cmath>

void setPixels(const Display& display, void* rawPixels) {
    for (int i = 0; i < 32; ++i) {
        uint64_t line = display.getLine(i);

        for (int j = 0; j < 64; j++) {
            bool isPixelOn = (line >> j) & 1;
            ((uint32_t*)rawPixels)[(63 - j) + (64 * i)] = isPixelOn ? 0xFFFFFFFF : 0x000000FF;
        }
    }
}
// #TODO sierpinski not working

int main() {
    Chip8 chip8;
    Keypad& keypad = chip8.getKeypad();
    std::ifstream file{"roms/BC_test.ch8"};
    if (!file.is_open()) {
        std::cout << "Failed to load file";
    }
    std::vector<uint8_t> buffer((std::istreambuf_iterator<char>(file)),
                                std::istreambuf_iterator<char>());

    chip8.loadRom(buffer);

    int scale = 8;
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow("Chip 8 emulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                          64 * scale, 32 * scale, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, 64, 32);
    void* rawPixels = nullptr;
    int rawPitch = 0;
    SDL_Event e;
    bool quit = false;
    int instructionsPerSecond = 700;
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) quit = true;
            if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_1:
                        keypad.press(0x1);
                        break;
                    case SDLK_2:
                        keypad.press(0x2);
                        break;
                    case SDLK_3:
                        keypad.press(0x3);
                        break;
                    case SDLK_4:
                        keypad.press(0xC);
                        break;
                    case SDLK_q:
                        keypad.press(0x4);
                        break;
                    case SDLK_w:
                        keypad.press(0x5);
                        break;
                    case SDLK_e:
                        keypad.press(0x6);
                        break;
                    case SDLK_r:
                        keypad.press(0xD);
                        break;
                    case SDLK_a:
                        keypad.press(0x7);
                        break;
                    case SDLK_s:
                        keypad.press(0x8);
                        break;
                    case SDLK_d:
                        keypad.press(0x9);
                        break;
                    case SDLK_f:
                        keypad.press(0xE);
                        break;
                    case SDLK_z:
                        keypad.press(0xA);
                        break;
                    case SDLK_x:
                        keypad.press(0x0);
                        break;
                    case SDLK_c:
                        keypad.press(0xB);
                        break;
                    case SDLK_v:
                        keypad.press(0xF);
                        break;
                }
            }

            if (e.type == SDL_KEYUP) {
                switch (e.key.keysym.sym) {
                    case SDLK_1:
                        keypad.release(0x1);
                        break;
                    case SDLK_2:
                        keypad.release(0x2);
                        break;
                    case SDLK_3:
                        keypad.release(0x3);
                        break;
                    case SDLK_4:
                        keypad.release(0xC);
                        break;
                    case SDLK_q:
                        keypad.release(0x4);
                        break;
                    case SDLK_w:
                        keypad.release(0x5);
                        break;
                    case SDLK_e:
                        keypad.release(0x6);
                        break;
                    case SDLK_r:
                        keypad.release(0xD);
                        break;
                    case SDLK_a:
                        keypad.release(0x7);
                        break;
                    case SDLK_s:
                        keypad.release(0x8);
                        break;
                    case SDLK_d:
                        keypad.release(0x9);
                        break;
                    case SDLK_f:
                        keypad.release(0xE);
                        break;
                    case SDLK_z:
                        keypad.release(0xA);
                        break;
                    case SDLK_x:
                        keypad.release(0x0);
                        break;
                    case SDLK_c:
                        keypad.release(0xB);
                        break;
                    case SDLK_v:
                        keypad.release(0xF);
                        break;
                }
            }
        }
        chip8.tickTimers();
        for (int i = 0; i < instructionsPerSecond / 60; ++i) {
            chip8.getCpu().cycle();
        }
        // should update texture
        if (true) {
            SDL_LockTexture(texture, nullptr, &rawPixels, &rawPitch);
            setPixels(chip8.getDisplay(), rawPixels);
            SDL_UnlockTexture(texture);

        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer, texture, nullptr, nullptr);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
