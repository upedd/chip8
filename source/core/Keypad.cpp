#include "Keypad.h"

void Keypad::press(uint8_t key) {
    if (key > 0xF) assert("key out of range");
    if (isWaitingForKey && !lastPressedKey.has_value()) {
        lastPressedKey = key;
    }
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

std::optional<uint8_t> Keypad::getLastPressedKey() {
    if (!isWaitingForKey) {
        isWaitingForKey = true;
    }
    if (lastPressedKey.has_value()) {
        auto temp = lastPressedKey;
        isWaitingForKey = false;
        lastPressedKey = {};
        return temp;
    }
    return {};
}




