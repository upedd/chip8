#ifndef CHIP8_TIMER_H
#define CHIP8_TIMER_H


#include <cstdint>

class Timer {
    uint8_t m_value;
public:
    void tick();
    [[nodiscard]] bool isOn() const;
    [[nodiscard]] uint8_t getValue() const;
    void setValue(uint8_t value);
};


#endif //CHIP8_TIMER_H
