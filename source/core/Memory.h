#ifndef CHIP8_MEMORY_H
#define CHIP8_MEMORY_H

#include <array>

class Memory {
    std::array<uint8_t, 4096> m_memory {};

public:
    [[nodiscard]] uint8_t get(uint16_t address) const;

    void set(uint16_t address, uint8_t value);

    void loadFont();
};


#endif //CHIP8_MEMORY_H
