#include "Instruction.h"

uint16_t Instruction::getValue() const {
    return m_value;
}

uint16_t Instruction::getX() const {
    return (m_value & 0x0F00) >> 8;
}

uint16_t Instruction::getY() const {
    return (m_value & 0x00F0) >> 4;
}

uint16_t Instruction::getInstructionCode() const {
    return m_value >> 12;
}

uint16_t Instruction::getN() const {
    return m_value & 0x000F;
}

uint16_t Instruction::getNN() const {
    return m_value & 0x00FF;
}

uint16_t Instruction::getNNN() const {
    return m_value & 0x0FFF;
}
