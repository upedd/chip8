#ifndef CHIP8_KEYPAD_H
#define CHIP8_KEYPAD_H

#include <vector>
#include <optional>

class Keypad {
    uint16_t pressedKeys { 0 };
    bool isWaitingForKey { false };
    std::optional<uint8_t> lastPressedKey { };

public:
    void press(uint8_t key);
    void release(uint8_t key);
    [[nodiscard]] bool isKeyPressed(uint8_t key) const;

    std::optional<uint8_t> getLastPressedKey();

};


#endif //CHIP8_KEYPAD_H
