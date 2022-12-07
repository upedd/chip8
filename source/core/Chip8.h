#ifndef CHIP8_CHIP8_H
#define CHIP8_CHIP8_H


#include <array>
#include <stack>
#include <vector>
#include <iostream>
#include "Instruction.h"
#include "RandomGenerator.h"
#include "Memory.h"
#include "Display.h"
#include "Cpu.h"
#include "Keypad.h"
#include "Timer.h"

class Chip8 {
    Memory memory;
    Display display;
    Cpu cpu;
    Keypad keypad;
    Timer delayTimer;
    Timer soundTimer;

    std::shared_ptr<RandomGenerator> random;


public:
    explicit Chip8(std::shared_ptr<RandomGenerator> randomGenerator = std::make_shared<DefaultRandomGenerator>())
            : memory{}, display{}, delayTimer{}, soundTimer{}, random{std::move(randomGenerator)},
              cpu{memory, display, keypad, delayTimer, soundTimer, random} {
        memory.loadFont();
    };

    Cpu &getCpu();

    void loadRom(const std::vector<uint8_t> &bytes);

    const Display &getDisplay();

    void tickTimers();

    Keypad &getKeypad();
};


#endif //CHIP8_CHIP8_H
