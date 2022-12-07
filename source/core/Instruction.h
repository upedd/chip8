#ifndef CHIP8_INSTRUCTION_H
#define CHIP8_INSTRUCTION_H


#include <cstdint>

class Instruction {
    uint16_t m_value;
public:
    explicit Instruction(uint16_t value) : m_value {value} {};

    [[nodiscard]] uint16_t getValue() const;
    [[nodiscard]] uint16_t getX() const;
    [[nodiscard]] uint16_t getY() const;
    [[nodiscard]] uint16_t getInstructionCode() const;
    [[nodiscard]] uint16_t getN() const;
    [[nodiscard]] uint16_t getNN() const;
    [[nodiscard]] uint16_t getNNN() const;
};


#endif //CHIP8_INSTRUCTION_H
