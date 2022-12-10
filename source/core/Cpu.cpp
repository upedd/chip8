#include "Cpu.h"
#include "Instruction.h"
#include <iostream>

void Cpu::cycle() {
    Instruction instruction = fetch();
    std::cout << "Instruction " << std::hex << (int) instruction.getValue() << "\n";

    switch (instruction.getInstructionCode()) {
        case 0x0:
            switch (instruction.getValue()) {
                case 0x00E0:
                    clearScreen();
                    break;
                case 0x00EE:
                    returnFromSubroutine();
                    break;
                default:
                    assert("Unknown instruction beginning with 0x0.");
            }
            break;
        case 0x1:
            jump(instruction.getNNN());
            break;
        case 0x2:
            startSubroutine(instruction.getNNN());
            break;
        case 0x3:
            skipIfEqual(instruction.getX(), instruction.getNN());
            break;
        case 0x4:
            skipIfNotEqual(instruction.getX(), instruction.getNN());
            break;
        case 0x5:
            skipIfEqual(instruction.getX(), registers[instruction.getY()]);
            break;
        case 0x6:
            set(instruction.getX(), instruction.getNN());
            break;
        case 0x7:
            add(instruction.getX(), instruction.getNN());
            break;
        case 0x8:
            switch (instruction.getN()) {
                case 0x0:
                    set(instruction.getX(), registers[instruction.getY()]);
                    break;
                case 0x1:
                    binaryOr(instruction.getX(), instruction.getY());
                    break;
                case 0x2:
                    binaryAnd(instruction.getX(), instruction.getY());
                    break;
                case 0x3:
                    binaryXor(instruction.getX(), instruction.getY());
                    break;
                case 0x4:
                    addWithCarry(instruction.getX(), instruction.getY());
                    break;
                case 0x5:
                    subtract(instruction.getX(), instruction.getY(), false);
                    break;
                case 0x7:
                    subtract(instruction.getX(), instruction.getY(), true);
                    break;
                case 0x6:
                    rshift(instruction.getX(), instruction.getY());
                    break;
                case 0xE:
                    lshift(instruction.getX(), instruction.getY());
                    break;
                default:
                    assert("Unknown instruction starting with 0x8");
            }
            break;
        case 0x9:
            skipIfNotEqual(instruction.getX(), registers[instruction.getY()]);
            break;
        case 0xA:
            setIndexRegister(instruction.getNNN());
            break;
        case 0xB:
            jumpWithOffset(instruction.getNNN(), instruction.getX());
            break;
        case 0xC:
            generateRandom(instruction.getX(), instruction.getNN());
            break;
        case 0xD:
            draw(instruction.getX(), instruction.getY(), instruction.getN());
            break;
        case 0xE:
            switch (instruction.getNN()) {
                case 0x9E:
                    skipIfKey(instruction.getX());
                    break;
                case 0xA1:
                    skipIfNotKey(instruction.getX());
                    break;
                default:
                    assert(true && "Unknown instruction starting with 0xE");
            }
            break;
        case 0xF:
            switch (instruction.getNN()) {
                case 0x07:
                    getDelayTimer(instruction.getX());
                    break;
                case 0x15:
                    setDelayTimer(instruction.getX());
                    break;
                case 0x18:
                    setSoundTimer(instruction.getX());
                    break;
                case 0x1E:
                    addToIndexRegister(instruction.getX());
                    break;
                case 0x0A:
                    waitForKey(instruction.getX());
                    break;
                case 0x29:
                    getFontCharacter(instruction.getX());
                    break;
                case 0x33:
                    binaryDecimalConversion(instruction.getX());
                    break;
                case 0x55:
                    store(instruction.getX());
                    break;
                case 0x65:
                    load(instruction.getX());
                    break;
            }

    }
}

Instruction Cpu::fetch() {
    uint16_t a = memory.get(pc);
    uint8_t b = memory.get(pc + 1);
    Instruction instruction {static_cast<uint16_t>(a << 8 | b)};
    pc += 2;
    return instruction;
}

void Cpu::jump(uint16_t address) { pc = address; }
void Cpu::clearScreen() { display.clear(); };
void Cpu::returnFromSubroutine() {
    pc = stack.top();
    stack.pop();
}
void Cpu::startSubroutine(uint16_t address) {
    stack.push(pc);
    pc = address;
}

void Cpu::skipIfEqual(uint8_t register_index, uint8_t value) {
    if (registers[register_index] == value)
        pc += 2;
}

void Cpu::skipIfNotEqual(uint8_t register_index, uint8_t value) {
    if (registers[register_index] != value)
        pc += 2;
}

void Cpu::set(uint8_t register_index, uint8_t value) {
    registers[register_index] = value;
}
void Cpu::add(uint8_t register_index, uint8_t value) {
    registers[register_index] = registers[register_index] + value;
}

void Cpu::binaryOr(uint8_t x, uint8_t y) {
    registers[x] = registers[x] | registers[y];
}

void Cpu::binaryAnd(uint8_t x, uint8_t y) {
    registers[x] = registers[x] & registers[y];
}

void Cpu::binaryXor(uint8_t x, uint8_t y) {
    registers[x] = registers[x] ^ registers[y];
}

void Cpu::addWithCarry(uint8_t x, uint8_t y) {
    // check for overflow
    registers[0xF] = (static_cast<uint16_t>(registers[x]) + registers[y]) > 255;
    add(x, registers[y]);
}

void Cpu::subtract(uint8_t x, uint8_t y, bool reverse) {
    int a = registers[reverse ? y : x];
    int b = registers[reverse ? x : y];
    registers[0xF] = a > b;
    registers[x] = a - b;
}

void Cpu::rshift(uint8_t x, uint8_t y) {
    if (bitshiftLegacyBehaviourQuirk) set(x, registers[y]);
    registers[0xF] = registers[x] & 1;
    registers[x] = registers[x] >> 1;
};

void Cpu::lshift(uint8_t x, uint8_t y) {
    if (bitshiftLegacyBehaviourQuirk) set(x, registers[y]);
    registers[0xF] = (registers[x] & 128) > 0;
    registers[x] = registers[x] << 1;
}

void Cpu::setIndexRegister(uint16_t value) {
    indexRegister = value;
}

void Cpu::jumpWithOffset(uint16_t address, uint8_t x) {
    if (jumpWithOffsetModernBehaviorQuirk) {
        pc = address + registers[x];
    } else {
        pc = address + registers[0x0];
    }
}

void Cpu::generateRandom(uint8_t x, uint8_t value) {
    registers[x] = random->get() & value;
}

void Cpu::draw(uint8_t xIndex, uint8_t yIndex, uint8_t n) {
    uint8_t x = registers[xIndex] & 63;
    uint8_t y = registers[yIndex] & 31;
    registers[0xF] = 0;
    for (int i = 0; i < n; ++i) {
        uint8_t line = memory.get(indexRegister + i);
        if (x < 56) {
            registers[0xF] = registers[0xF] | ((display.getLine(y) & (static_cast<uint64_t>(line) << (56 - x))) > 0);
            display.setLine(y, display.getLine(y) ^ (static_cast<uint64_t>(line) << (56 - x)));
        } else {
            registers[0xF] = registers[0xF] | ((display.getLine(y) & (static_cast<uint64_t>(line) << (x - 56))) > 0);
            display.setLine(y, display.getLine(y) ^ (static_cast<uint64_t>(line) >> (x - 56)));
        }
        ++y;
        if (y >= 32) break;
    }
}

void Cpu::addToIndexRegister(uint8_t x) {
    indexRegister += registers[x];
    registers[0xF] = indexRegister > 0x1FF;
}

void Cpu::binaryDecimalConversion(uint16_t x) {
    uint8_t value = registers[x];
    memory.set(indexRegister + 2, value % 10);
    value /= 10;
    memory.set(indexRegister + 1, value % 10);
    value /= 10;
    memory.set(indexRegister, value);
}

void Cpu::store(uint8_t x) {
    for (int i = 0; i <= x; ++i) {
        memory.set(indexRegister + i, registers[i]);
    }
    if (loadStoreIncrementIndexQuirk) {
        indexRegister = indexRegister + x + 1;
    }
}

void Cpu::load(uint8_t x) {
    for (int i = 0; i <= x; ++i) {
        registers[i] = memory.get(indexRegister + i);
    }
    if (loadStoreIncrementIndexQuirk) {
        indexRegister = indexRegister + x + 1;
    }
}

void Cpu::skipIfKey(uint8_t x) {
    if (keypad.isKeyPressed(registers[x])) {
        pc += 2;
    }
}

void Cpu::skipIfNotKey(uint8_t x) {
    if (!keypad.isKeyPressed(registers[x])) {
        pc += 2;
    }
}

void Cpu::getDelayTimer(uint8_t x) {
    registers[x] = delayTimer.getValue();
}

void Cpu::setDelayTimer(uint8_t x) {
    delayTimer.setValue(registers[x]);
}

void Cpu::setSoundTimer(uint8_t x) {
    soundTimer.setValue(registers[x]);
}

void Cpu::getFontCharacter(uint8_t x) {
    // take last nibble
    uint8_t character = (registers[x] & 0x0F);
    indexRegister = 0x50 + (character * 5);
}

void Cpu::waitForKey(uint8_t x) {
    std::optional<uint8_t> lastPress = keypad.getLastPressedKey();
    if (lastPress.has_value()) {
        registers[x] = lastPress.value();
    } else {
        pc -= 2;
    }
}
// #TODO comment, logging and unit testing

