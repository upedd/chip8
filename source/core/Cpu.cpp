#include "Cpu.h"
#include "Instruction.h"
#include <iostream>

/**
 * Fetches, decodes and executes the instruction at current program counter address.
 */
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

/**
 * Returns the instruction at current program counter address and sets it to the next instruction.
 * @return Instruction
 */
Instruction Cpu::fetch() {
    uint16_t a = memory.get(pc);
    uint8_t b = memory.get(pc + 1);
    Instruction instruction{static_cast<uint16_t>(a << 8 | b)};
    pc += 2;
    return instruction;
}

/**
 * Sets program counter to given address
 * @param address
*/
void Cpu::jump(uint16_t address) {
    pc = address;
}

/**
 * Clears display
 */
void Cpu::clearScreen() {
    display.clear();
}

/**
 * jumps to top element of the stack and pops it
 */
void Cpu::returnFromSubroutine() {
    pc = stack.top();
    stack.pop();
}

/**
 * Pushes current program counter value to the stack and jumps to given address
 * @param address
 */
void Cpu::startSubroutine(uint16_t address) {
    stack.push(pc);
    pc = address;
}

/**
 * Increments program counter by 2 (skips 1 instruction) if value in register of given index equals given value
 * @param register_index index of register of which value will be compared
 * @param value value to be compared to
 */
void Cpu::skipIfEqual(uint8_t register_index, uint8_t value) {
    if (registers[register_index] == value)
        pc += 2;
}

/**
 * Increments pc by 2 (skips one instruction) if value in register of given index does not equals given value
 * @param register_index index of register of which value will be compared
 * @param value value to be compared to
 */
void Cpu::skipIfNotEqual(uint8_t register_index, uint8_t value) {
    if (registers[register_index] != value)
        pc += 2;
}

/**
 * Sets register of given index to given value
 * @param register_index
 * @param value
 */
void Cpu::set(uint8_t register_index, uint8_t value) {
    registers[register_index] = value;
}

/**
 * Adds given value to value already stored of register with given index
 * @param register_index
 * @param value
 */
void Cpu::add(uint8_t register_index, uint8_t value) {
    registers[register_index] = registers[register_index] + value;
}

/**
 * Sets register with index x to binary or of values stored in registers with indexes x and y
 * @param x index of register
 * @param y index of register
 */
void Cpu::binaryOr(uint8_t x, uint8_t y) {
    registers[x] = registers[x] | registers[y];
}

/**
 * Sets register with index x to binary and of values stored in registers with indexes x and y
 * @param x index of register
 * @param y index of register
 */
void Cpu::binaryAnd(uint8_t x, uint8_t y) {
    registers[x] = registers[x] & registers[y];
}

/**
 * Sets register with index x to binary xor of values stored in registers with indexes x and y
 * @param x index of register
 * @param y index of register
 */
void Cpu::binaryXor(uint8_t x, uint8_t y) {
    registers[x] = registers[x] ^ registers[y];
}

/**
 * Adds value stored in register with index y to value stored in register with index x and stores it
 * in register with index x. Sets carry register VF to 1 if overflow happened and to 0 otherwise.
 * @param x index of register
 * @param y index of register
 */
void Cpu::addWithCarry(uint8_t x, uint8_t y) {
    // We check for overflow by casting our 8-bit values to 16 bit ones and checking if
    // addition result is larger than maximum possible value we can store in 8-bits
    registers[0xF] = (static_cast<uint16_t>(registers[x]) + registers[y]) > 255;
    add(x, registers[y]);
}

/**
 * Subtracts value stored in register with index x with value stored in register with index y.
 * If reverse is set to true subtracts value stored in register with index y with value stored in register with index x.
 * Stores result in register with index x.
 * Carry register VF is set to 1 if borrowing did not happen, that is if the first operand is larger than the second operand.
 * @param x index of register
 * @param y index of register
 * @param reverse if true switches operands.
 */
void Cpu::subtract(uint8_t x, uint8_t y, bool reverse) {
    int a = registers[reverse ? y : x];
    int b = registers[reverse ? x : y];
    registers[0xF] = a > b;
    registers[x] = a - b;
}

/**
 * Shifts the value stored in register with index x by 1 bit to right.
 * Sets carry register VF to the bit that was shifted out.
 * On old interpreters it first set the value of register with index x to value of the register with index y.
 * This behaviour is controlled by Cpu::bitshiftLegacyBehaviourQuirk
 * @param x index of register
 * @param y index of register
 */
void Cpu::rshift(uint8_t x, uint8_t y) {
    if (bitshiftLegacyBehaviourQuirk) set(x, registers[y]);
    registers[0xF] = registers[x] & 1;
    registers[x] = registers[x] >> 1;
};

/**
 * Shifts the value stored in register with index x by 1 bit to left.
 * Sets carry register VF to the bit that was shifted out.
 * On old interpreters it first set the value of register with index x to value of the register with index y.
 * This behaviour is controlled by Cpu::bitshiftLegacyBehaviourQuirk
 * @param x index of register
 * @param y index of register
 */
void Cpu::lshift(uint8_t x, uint8_t y) {
    if (bitshiftLegacyBehaviourQuirk) set(x, registers[y]);
    registers[0xF] = (registers[x] & 128) >> 7;
    registers[x] = registers[x] << 1;
}

/**
 * Sets the index register to the given value
 * @param value
 */
void Cpu::setIndexRegister(uint16_t value) {
    indexRegister = value;
}

/**
 * Sets program counter to given address plus the value of register with index 0.
 * If Cpu::jumpWithOffsetModernBehaviorQuirk is enabled it will instead add the value stored in register with index x.
 * @param address
 * @param x index of register
 */
void Cpu::jumpWithOffset(uint16_t address, uint8_t x) {
    if (jumpWithOffsetModernBehaviorQuirk) {
        pc = address + registers[x];
    } else {
        pc = address + registers[0x0];
    }
}

/**
 * Generates random value between 0-255, binary ands it with given value and stores it in register with index x.
 * @param x index of register
 * @param value
 */
void Cpu::generateRandom(uint8_t x, uint8_t value) {
    assert(random != nullptr && "random in the cpu was not set!");
    registers[x] = random->get() & value;
}

/**
 * Draws n lines at position x = value stored in register with index x,
 * y = value stored in register with index y.
 * Reads sprite data from memory starting at address stored in index register.
 * Sets carry register VF to 1 if it draws any pixel over already drawn pixel.
 * @param x
 * @param y
 * @param n
 */
void Cpu::draw(uint8_t xIndex, uint8_t yIndex, uint8_t n) {
    // #TODO refactor
    uint8_t x = registers[xIndex] & 63;
    uint8_t y = registers[yIndex] & 31;
    registers[0xF] = 0;
    for (int i = 0; i < n; ++i) {
        uint8_t line = memory.get(indexRegister + i);
        if (x < 56) {
            registers[0xF] = registers[0xF] | ((display.getLine(y) & (static_cast<uint64_t>(line) << (56 - x))) > 0);
            display.setLine(y, display.getLine(y) ^ (static_cast<uint64_t>(line) << (56 - x)));
        } else {
            registers[0xF] = registers[0xF] | ((display.getLine(y) & (static_cast<uint64_t>(line) >> (x - 56))) > 0);
            display.setLine(y, display.getLine(y) ^ (static_cast<uint64_t>(line) >> (x - 56)));
        }
        ++y;
        if (y >= 32) break;
    }
}

/**
 * Adds value stored in register with index x to the index register.
 * Sets the carry register VF to 1 if it "overflows" to above 0x0FFF (outside the addressing range)
 * @param x index of register
 */
void Cpu::addToIndexRegister(uint8_t x) {
    indexRegister += registers[x];
    registers[0xF] = indexRegister > 0x0FFF;
}

/**
 * Gets the value stored in register with index x and stores it digits in memory staring with address stored in index register.
 * For example, if VX contains 156 it will store 1 at address [index register], 5 at [index register + 1], and 6 at [index register + 2].
 * @param x index of register
 */
void Cpu::binaryDecimalConversion(uint16_t x) {
    uint8_t value = registers[x];
    memory.set(indexRegister + 2, value % 10);
    value /= 10;
    memory.set(indexRegister + 1, value % 10);
    value /= 10;
    memory.set(indexRegister, value);
}

/**
 * Stores the values of registers with indexes from 0 to x in memory starting at address stored in index register.
 * If Cpu::loadStoreIncrementIndexQuirk is enabled it will set the index register to address just after stored data, it will not modify index register otherwise.
 * @param x index of register
 */
void Cpu::store(uint8_t x) {
    for (int i = 0; i <= x; ++i) {
        memory.set(indexRegister + i, registers[i]);
    }
    if (loadStoreIncrementIndexQuirk) {
        indexRegister = indexRegister + x + 1;
    }
}

/**
 * Loads values from memory starting at address stored in index register to registers from 0 to x.
 * If Cpu::loadStoreIncrementIndexQuirk is enabled it will set the index register to address just after loaded data, it will not modify index register otherwise.
 * @param x index of register
 */
void Cpu::load(uint8_t x) {
    for (int i = 0; i <= x; ++i) {
        registers[i] = memory.get(indexRegister + i);
    }
    if (loadStoreIncrementIndexQuirk) {
        indexRegister = indexRegister + x + 1;
    }
}

/**
 * Increments program counter by 2 (skips 1 instruction)
 * if the key with value stored in register with index x is currently pressed.
 * @param x index of register
 */
void Cpu::skipIfKey(uint8_t x) {
    if (keypad.isKeyPressed(registers[x])) {
        pc += 2;
    }
}

/**
 * Increments program counter by 2 (skips 1 instruction)
 * if the key with value stored in register with index x is currently not pressed.
 * @param x index of register
 */
void Cpu::skipIfNotKey(uint8_t x) {
    if (!keypad.isKeyPressed(registers[x])) {
        pc += 2;
    }
}

/**
 * Stores the value of the delay timer in register with index x.
 * @param x index of register
 */
void Cpu::getDelayTimer(uint8_t x) {
    registers[x] = delayTimer.getValue();
}

/**
 * Sets the delay timer to the value stored in register with index x
 * @param x index of register
 */
void Cpu::setDelayTimer(uint8_t x) {
    delayTimer.setValue(registers[x]);
}

/**
 * Sets the sound timer to the value stored in register with index x
 * @param x index of register
 */
void Cpu::setSoundTimer(uint8_t x) {
    soundTimer.setValue(registers[x]);
}

/**
 * Sets the index register to the address of the sprite of character stored in register with index x.
 * @param x index of register
 */
void Cpu::getFontCharacter(uint8_t x) {
    // we truncate the value to the last nibble as characters are values only from 0-16
    uint8_t character = (registers[x] & 0x0F);
    // each character sprite is 5 bytes wide
    indexRegister = 0x50 + (character * 5);
}

/**
 * Does not proceed to the next instruction until user presses a key.
 * That pressed key value is stored in register with index x.
 * @param x index of the register
 */
void Cpu::waitForKey(uint8_t x) {
    std::optional<uint8_t> lastPress = keypad.getLastPressedKey();
    if (lastPress.has_value()) {
        registers[x] = lastPress.value();
    } else {
        // we need to decrement the pc by 2 to not proceed to next instruction
        // as it was previously incremented in instruction fetching.
        pc -= 2;
    }
}

void Cpu::setRandom(RandomGenerator *randomGenerator) {
    random = randomGenerator;
}
// #TODO logging and unit testing

