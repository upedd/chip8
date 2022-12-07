#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "../source/core/Instruction.h"

TEST_CASE("Instruction tests") {
    uint16_t value {static_cast<uint16_t>(0xD68F)};
    Instruction instruction {value};

    SUBCASE("getValue() should return correct value") {
        CHECK(instruction.getValue() == value);
    }

    SUBCASE("getX() should return correct value") {
        CHECK(instruction.getX() == 0x6);
    }

    SUBCASE("getY() should return correct value") {
        CHECK(instruction.getY() == 0x8);
    }

    SUBCASE("getInstructionCode() should return correct value") {
        CHECK(instruction.getInstructionCode() == 0xD);
    }

    SUBCASE("getN() should return correct value") {
        CHECK(instruction.getN() == 0xF);
    }

    SUBCASE("getNN() should return correct value") {
        CHECK(instruction.getNN() == 0x8F);
    }

    SUBCASE("getNNN() should return correct value") {
        CHECK(instruction.getNNN() == 0x68F);
    }
}