#include "RandomGenerator.h"

uint8_t DefaultRandomGenerator::get() {
    return static_cast<uint8_t>(intDistribution(mt19937));
}
