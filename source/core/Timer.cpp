#include "Timer.h"

void Timer::tick() {
    if (m_value > 0) --m_value;
}

bool Timer::isOn() const {
  return m_value > 0;
}

uint8_t Timer::getValue() const {
    return m_value;
}

void Timer::setValue(uint8_t value) {
    m_value = value;
}


