#include <iostream>
#include "Display.h"

uint64_t Display::getLine(int y) const {
    //assert((y < 0 || y >= 32) && "Accessing line out of range");
    return m_display[y];
}

void Display::setLine(int y, uint64_t line) {
    //assert((y < 0 || y >= 32) && "Accessing line out of range");
    m_display[y] = line;
}

void Display::clear() {
    m_display.fill(0);
}
