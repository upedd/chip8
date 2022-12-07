#include "Keypad.h"

void Keypad::press(uint8_t key) {
    if (key > 0xF) assert("key out of range");
    pressedKeys |= 1 << key;
}

void Keypad::release(uint8_t key) {
    if (key > 0xF) assert("key out of range");
    pressedKeys &= ~(1 << key);
}

bool Keypad::isKeyPressed(uint8_t key) const {
    if (key > 0xF) assert("key out of range");
    return (pressedKeys >> key) & 1;
}


