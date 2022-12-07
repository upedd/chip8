#ifndef CHIP8_DISPLAY_H
#define CHIP8_DISPLAY_H


#include <array>

class Display {
    std::array<uint64_t, 32> m_display {};

public:
    [[nodiscard]] uint64_t getLine(int y) const;

    void setLine(int y, uint64_t line);

    void clear();
};


#endif //CHIP8_DISPLAY_H
