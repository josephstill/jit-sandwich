#ifndef GUEST_DEFS_H
#define GUEST_DEFS_H

#include <cstdint>

typedef uint16_t  GuestAddress;

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


#endif