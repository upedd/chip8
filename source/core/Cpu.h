#ifndef CHIP8_CPU_H
#define CHIP8_CPU_H

#include "Memory.h"
#include "Display.h"
#include "RandomGenerator.h"
#include "Instruction.h"
#include "Keypad.h"
#include "Timer.h"
#include <stack>
#include <utility>

class Cpu {
    Memory &memory;
    Display &display;
    Keypad &keypad;
    Timer& delayTimer;
    Timer& soundTimer;

    // #TODO probably could be instead implemented using smart pointers
    RandomGenerator *random;

    uint16_t pc;
    uint16_t indexRegister;
    std::stack<uint16_t> stack;
    std::array<uint8_t, 16> registers;

    // whatever to copy the value of vy into vx before performing bitshift
    bool bitshiftLegacyBehaviourQuirk{false};
    // jumps to address XNN + value of register X f.e. B220 will jump to address 220 plus the value in the register V2.
    bool jumpWithOffsetModernBehaviorQuirk{false};
    // whatever to increment the index register when performing load or store instructions
    bool loadStoreIncrementIndexQuirk{false};
public:
    Cpu(Memory &memory, Display &display, Keypad &keypad, Timer &delayTimer, Timer &soundTimer) : memory{memory}, display{display}, keypad{keypad},
                                                          delayTimer{delayTimer}, soundTimer{soundTimer}, random {nullptr},
                                                          pc{0x200}, indexRegister{0}, stack{},
                                                          registers{} {};

    void cycle();

    void setRandom(RandomGenerator* randomGenerator);
private:
    Instruction fetch();

    // === Start of cpu instructions ===
    void clearScreen();
    void returnFromSubroutine();
    void jump(uint16_t address);
    void startSubroutine(uint16_t address);
    void skipIfEqual(uint8_t register_index, uint8_t value);
    void skipIfNotEqual(uint8_t register_index, uint8_t value);
    void set(uint8_t register_index, uint8_t value);
    void add(uint8_t register_index, uint8_t value);
    void binaryOr(uint8_t x, uint8_t y);
    void binaryAnd(uint8_t x, uint8_t y);
    void binaryXor(uint8_t x, uint8_t y);
    void addWithCarry(uint8_t x, uint8_t y);
    void subtract(uint8_t x, uint8_t y, bool reverse);
    void rshift(uint8_t x, uint8_t y);
    void lshift(uint8_t x, uint8_t y);
    void setIndexRegister(uint16_t value);
    void jumpWithOffset(uint16_t address, uint8_t x);
    void generateRandom(uint8_t x, uint8_t value);
    void draw(uint8_t x, uint8_t y, uint8_t n);
    void addToIndexRegister(uint8_t x);
    void binaryDecimalConversion(uint16_t x);
    void store(uint8_t x);
    void load(uint8_t x);
    void skipIfKey(uint8_t x);
    void skipIfNotKey(uint8_t x);
    void getDelayTimer(uint8_t x);
    void setDelayTimer(uint8_t x);
    void setSoundTimer(uint8_t x);
    void getFontCharacter(uint8_t x);
    void waitForKey(uint8_t x);
    // === End of cpu instructions ===
};


#endif //CHIP8_CPU_H
