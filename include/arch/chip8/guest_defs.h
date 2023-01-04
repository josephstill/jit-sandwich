#ifndef GUEST_DEFS_H
#define GUEST_DEFS_H

#include <cstdint>

typedef uint16_t GuestAddress;

typedef union Chip8Instruction
{
    struct 
    {
        unsigned nibble0: 4;
        unsigned nibble1: 4;
        unsigned nibble2: 4;
        unsigned nibble3: 4;
    };
    uint16_t instruction;
} Chip8Instruction; 

#define ROM_LOCATION (0x0200)
#define SYSTEM_MEM   (0x0EA0)
#define SCREEN_MEM   (0x0F00)
#define END_MEM      (0x0FFF)
#define MEM_SIZE     (END_MEM + 1)

#endif