cmake_minimum_required(VERSION 3.23)
project(chip8)

set(CMAKE_CXX_STANDARD 20)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(chip8 source/app/main.cpp source/core/Chip8.cpp source/core/Chip8.h source/core/Instruction.cpp source/core/Instruction.h source/core/RandomGenerator.cpp source/core/RandomGenerator.h source/core/Memory.cpp source/core/Memory.h source/core/Display.cpp source/core/Display.h source/core/Cpu.cpp source/core/Cpu.h source/core/Keypad.cpp source/core/Keypad.h source/core/Timer.cpp source/core/Timer.h)
target_link_libraries(chip8 ${SDL2_LIBRARIES})

add_subdirectory(tests)