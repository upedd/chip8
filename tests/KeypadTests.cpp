#include "doctest.h"
#include "../source/core/Keypad.h"

TEST_CASE("Keypad tests") {
    Keypad keypad {};
    SUBCASE("key pressing should be implemented correctly") {
        keypad.press(0x3);
        CHECK(keypad.isKeyPressed(0x3) == true);
    }

    SUBCASE("key releasing should be implemented correctly") {
        keypad.release(0x3);
        CHECK(keypad.isKeyPressed(0x3) == false);
    }

    SUBCASE("pressing key twice without releasing should work correctly") {
        keypad.press(0x4);
        keypad.press(0x4);
        CHECK(keypad.isKeyPressed(0x4) == true);
    }

    SUBCASE("pressing and releasing multiple keys should work") {
        keypad.press(0xF);
        keypad.press(0xA);
        keypad.press(0x0);
        CHECK(keypad.isKeyPressed(0xF) == true);
        CHECK(keypad.isKeyPressed(0xA) == true);
        CHECK(keypad.isKeyPressed(0x0) == true);
        keypad.release(0xA);
        CHECK(keypad.isKeyPressed(0xF) == true);
        CHECK(keypad.isKeyPressed(0xA) == false);
        CHECK(keypad.isKeyPressed(0x0) == true);
        keypad.release(0x0);
        CHECK(keypad.isKeyPressed(0xF) == true);
        CHECK(keypad.isKeyPressed(0xA) == false);
        CHECK(keypad.isKeyPressed(0x0) == false);
        keypad.release(0xF);
        CHECK(keypad.isKeyPressed(0xF) == false);
        CHECK(keypad.isKeyPressed(0xA) == false);
        CHECK(keypad.isKeyPressed(0x0) == false);
    }

    SUBCASE("releasing unpressed key should work correctly") {
        CHECK(keypad.isKeyPressed(0xC) == false);
        keypad.release(0xC);
        CHECK(keypad.isKeyPressed(0xC) == false);
    }
}