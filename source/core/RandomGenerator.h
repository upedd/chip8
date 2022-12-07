#ifndef CHIP8_RANDOMGENERATOR_H
#define CHIP8_RANDOMGENERATOR_H


#include <cstdint>
#include <random>

class RandomGenerator {
public:
    virtual uint8_t get() = 0;
    virtual ~RandomGenerator() = default;
};

class DefaultRandomGenerator : public RandomGenerator {
    std::mt19937 mt19937 {std::random_device {}()};
    std::uniform_int_distribution<> intDistribution {0, 255};
public:
    uint8_t get() override;
};

#endif //CHIP8_RANDOMGENERATOR_H
