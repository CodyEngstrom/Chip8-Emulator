# Chip-8 Emulator

## Description

The Chip-8 Emulator is a lightweight and cross-platform emulator designed to recreate the experience of running vintage Chip-8 programs and games on modern hardware. Chip-8, originally developed in the 1970s, was a simple interpreted programming language designed for early microcomputers. This emulator allows you to step back in time and enjoy the nostalgia of classic Chip-8 games and programs.

## Table of Contents

- [Getting Started](#getting-started)
  - [Prerequisites](#prerequisites)
  - [Installation](#installation)
- [Usage](#usage)
- [Features](#features)
- [ROMs](#roms)
- [Todo](#todo)

## Getting Started

To get started, make sure all prerequisites are installed.  Then follow the usage commands and play some games! :)

### Prerequisites

This project was written and tested in Linux with the following dependencies installed:
- gcc/gpp
- cmake
- SDL2

### Installation

To install the emulator, first create a build directory then cd into it:

```shell
$ mkdir build;
$ cd build;
```

We can now compile the emulator into the build directory: 

```shell
$ cmake;
$ make
```

These commands will give us the compiled Chip-8 program in the file  ./chip8-emulator under the build directory.

## Usage

To run the emmulator, the following command starts the emulator with a specified file:

```
$ ./chip8-emulator ../roms/<ROM FILE NAME>; 
```

Notes: 
- The ../roms/ path is needed when accessing the rom files located in the rom directory.
- Game speed may need to be adjusted for different ROMs.  This can be done by changing the delay number at the end of the game loop in file ./src/main.cpp.

## Features

1. Interpreter: 
   - Executes the Chip-8 interpreted language to run program.
2. Display: 
   - Rendering of graphics, including sprites and characters using SDL2.
   - Emulates Chip-8's 64x32 monochrome display.
3. Input Handling: 
   -  Maps the correct inputs onto a modern keyboard based on the Chip-8 keypad.
4. ROM Loading:
   - Loading of Chip-8 ROMs from files.
   - Support for standard Chip-8 ROM file formats. 

## ROMs

Supported ROMs can be found in the ./roms directory.  Many additional ROMs can be found through online sources.  Any ROM formatted for Chip-8 will work.

## TODO

Additional Improvements coming soon for the Emulator:

- [ ] Implement audio support in SDL2
- [ ] Modify the emulator to automatically set the delay to run at its intended clock speed (Hz)
- [ ] Improve documentation
