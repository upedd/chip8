#include "Chip8.h"

Cpu &Chip8::getCpu() {
    return cpu;
}

void Chip8::loadRom(const std::vector<uint8_t> &bytes) {
    uint16_t pos = 0x200;
    for (uint8_t byte: bytes) {
        memory.set(pos, byte);
        ++pos;
    }
}

const Display &Chip8::getDisplay() {
    return display;
}

void Chip8::tickTimers() {
    soundTimer.tick();
    delayTimer.tick();
}

Keypad &Chip8::getKeypad() {
    return keypad;
}
