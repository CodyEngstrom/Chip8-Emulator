cmake_minimum_required(VERSION 3.10)
project(Chip8Emulator)

set(CMAKE_CXX_STANDARD 11)

set(SOURCES
    src/chip8.cpp
    src/InstructionsChip8.cpp
    src/main.cpp
)

set(HEADERS
    inc/chip8.h
)

add_executable(chip8_emulator ${SOURCES} ${HEADERS})

find_package(SDL2 REQUIRED)

target_link_libraries(chip8_emulator SDL2)

target_include_directories(chip8_emulator PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/inc)