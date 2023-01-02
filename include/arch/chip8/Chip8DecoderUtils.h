#ifndef CHIP8DECODERUTILS_H
#define CHIP8DECODERUTILS_H

#include "guest_defs.h"

typedef struct DecoderCallbacks 
{
    void *opaque;
    void (*callMachineCode)(void *opaque, uint16_t address);
    void (*clearDisplay)(void *opaque);
    void (*doReturn)(void *opaque);
    void (*doGoto)(void *opaque, uint16_t address);
    void (*call)(void *opaque, uint16_t address);
    void (*ifEqualsImm)(void *opaque, uint8_t reg, uint8_t imm);
    void (*ifNotEqualsImm)(void *opaque, uint8_t reg, uint8_t imm);
    void (*ifEqualsReg)(void *opaque, uint8_t regX, uint8_t regY);
    void (*setRegisterImm)(void *opaque, uint8_t reg, uint8_t imm);
    void (*incrementImm)(void *opaque, uint8_t reg, uint8_t imm);
    void (*setRegister)(void *opaque, uint8_t regX, uint8_t regY);
    void (*orEquals)(void *opaque, uint8_t regX, uint8_t regY);
    void (*andEquals)(void *opaque, uint8_t regX, uint8_t regY);
    void (*xorEquals)(void *opaque, uint8_t regX, uint8_t regY);
    void (*incrementReg)(void *opaque, uint8_t regX, uint8_t regY);
    void (*decrementReg)(void *opaque, uint8_t regX, uint8_t regY);
    void (*shiftRighImm)(void *opaque, uint8_t regX, uint8_t regY);
    void (*reverseSubtract)(void *opaque, uint8_t regX, uint8_t regY);
    void (*shiftLeftImm)(void *opaque, uint8_t regX, uint8_t regY);
    void (*ifNotEqualsReg)(void *opaque, uint8_t regX, uint8_t regY);
    void (*setAddress)(void *opaque, uint16_t address);
    void (*jumpOffset)(void *opaque, uint16_t offset);
    void (*rand)(void *opaque, uint8_t reg, uint8_t mask);
    void (*draw)(void *opaque, uint8_t regX, uint8_t regY, uint8_t height);
    void (*keyEquals)(void *opaque, uint8_t regX);
    void (*keyNotEquals)(void *opaque, uint8_t regX);
    void (*getDelay)(void *opaque, uint8_t regX);
    void (*getKey)(void *opaque, uint8_t regX);
    void (*setDelay)(void *opaque, uint8_t regX);
    void (*setSound)(void *opaque, uint8_t regX);
    void (*incrementI)(void *opaque, uint8_t regX);
    void (*loadSprite)(void *opaque, uint8_t regX);
    void (*storeBCD)(void *opaque, uint8_t regX);
    void (*push)(void *opaque, uint8_t regX);
    void (*pop)(void *opaque, uint8_t regX); 
    void (*illeagleInstruction)(void *opaque, Chip8Instruction instruction);
} DecoderCallbacks;

static void decodeChip8(Chip8Instruction ins, DecoderCallbacks *callbacks)
{
    switch (ins.nibble3)
    {
        case 0x0:
            {
                if (ins.instruction == 0x00E0)
                {
                    callbacks->clearDisplay(callbacks->opaque);
                }
                else if (ins.instruction == 0x00EE)
                {
                    callbacks->doReturn(callbacks->opaque);
                }
                else
                {
                    callbacks->callMachineCode(callbacks->opaque, ins.instruction);
                }
            }
            break;
        case 0x1:
            {
                callbacks->doGoto(callbacks->opaque, ins.instruction & 0xFFF);
            }
            break;
        case 0x2:
            {
                callbacks->call(callbacks->opaque, ins.instruction & 0xFFF);
            }
            break;
        case 0x3:
            {
                uint8_t reg = ins.nibble2;
                uint8_t imm = 0xFF & ins.instruction;
                callbacks->ifEqualsImm(callbacks->opaque, reg, imm);
            }
            break;
        case 0x4:
            {
                uint8_t reg = ins.nibble2;
                uint8_t imm = 0xFF & ins.instruction;
                callbacks->ifNotEqualsImm(callbacks->opaque, reg, imm);
            }
            break;
        case 0x5:
            {
                uint8_t regX = ins.nibble2;
                uint8_t regY = ins.nibble1;
                callbacks->ifEqualsReg(callbacks->opaque, regX, regY);
            }
            break;
        case 0x6:
            {
                uint8_t reg = ins.nibble2;
                uint8_t imm = 0xFF & ins.instruction;                
                callbacks->setRegisterImm(callbacks->opaque, reg, imm);
            }
            break;
        case 0x7:
            {
                uint8_t reg = ins.nibble2;
                uint8_t imm = 0xFF & ins.instruction;                
                callbacks->incrementImm(callbacks->opaque, reg, imm);                
            }
            break;
        case 0x8:
            {
                uint8_t regX = ins.nibble2;
                uint8_t regY = ins.nibble1;
                switch (ins.nibble0)
                {
                    case 0x0:
                        {
                            callbacks->setRegister(callbacks->opaque, regX, regY);
                        }
                        break;
                    case 0x1:
                        {
                            callbacks->orEquals(callbacks->opaque, regX, regY);
                        }
                        break;
                    case 0x2:
                        {
                            callbacks->andEquals(callbacks->opaque, regX, regY);
                        }
                        break;
                    case 0x3:
                        {
                            callbacks->xorEquals(callbacks->opaque, regX, regY);
                        }
                        break;    
                    case 0x4:
                        {
                            callbacks->incrementReg(callbacks->opaque, regX, regY);
                        }
                        break;
                    case 0x5:
                        {
                            callbacks->decrementReg(callbacks->opaque, regX, regY);
                        }
                        break;
                    case 0x6:
                        {
                            callbacks->shiftRighImm(callbacks->opaque, regX, regY);
                        }
                        break;
                    case 0x7:
                        {
                            callbacks->reverseSubtract(callbacks->opaque, regX, regY);
                        }
                        break;
                    case 0xE:
                        {
                            callbacks->shiftLeftImm(callbacks->opaque, regX, regY);
                        }
                        break;
                    default:
                        {
                            callbacks->illeagleInstruction(callbacks->opaque, ins);
                        }
                        break;
                }
            }
            break;
        case 0x9:
            {
                uint8_t regX = ins.nibble2;
                uint8_t regY = ins.nibble1;
                callbacks->ifNotEqualsReg(callbacks->opaque, regX, regY);
            }
            break;
        case 0xA:
            {
                callbacks->setAddress(callbacks->opaque, ins.instruction & 0xFFF);
            }
            break;
        case 0xB:
            {
                callbacks->jumpOffset(callbacks->opaque, ins.instruction & 0xFFF);
            }
            break;
        case 0xC:
            {
                uint8_t reg = ins.nibble2;
                uint8_t mask = 0xFF & ins.instruction;
                callbacks->rand(callbacks->opaque, reg, mask);
            }
            break;
        case 0xD:
            {
                uint8_t regX = ins.nibble2;
                uint8_t regY = ins.nibble1;
                uint8_t height = ins.nibble0;
                callbacks->draw(callbacks->opaque, regX, regY, height);
            }
            break;
        case 0xE:
            {
                uint8_t regX = ins.nibble2;
                if (ins.instruction & 0xFF == 0x9E)
                {
                    callbacks->keyEquals(callbacks->opaque, regX);
                }
                else if (ins.instruction & 0xFF == 0xA1)
                {
                    callbacks->keyNotEquals(callbacks->opaque, regX);
                }
                else
                {
                    callbacks->illeagleInstruction(callbacks->opaque, ins);
                }
            }
            break;
        case 0xF:
            {
                uint8_t regX;
                switch (ins.instruction & 0xFF)
                {
                    case 0x07:
                        {
                            callbacks->getDelay(callbacks->opaque, regX);
                        }
                        break;
                    case 0x0A:
                        {
                            callbacks->getKey(callbacks->opaque, regX);                            
                        }
                        break;
                    case 0x15:
                        {
                            callbacks->setDelay(callbacks->opaque, regX);                            
                        }
                        break;
                    case 0x18:
                        {
                            callbacks->setSound(callbacks->opaque, regX);                            
                        }
                        break;
                    case 0x1E:
                        {
                            callbacks->incrementI(callbacks->opaque, regX);                            
                        }
                        break;
                    case 0x29:
                        {
                            callbacks->loadSprite(callbacks->opaque, regX);                            
                        }
                        break;
                    case 0x33:
                        {
                            callbacks->storeBCD(callbacks->opaque, regX);                            
                        }
                        break;
                    case 0x55:
                        {
                            callbacks->push(callbacks->opaque, regX);                            
                        }
                        break;
                    case 0x65:
                        {
                            callbacks->pop(callbacks->opaque, regX);                             
                        }
                        break;               
                    default:
                        {
                            callbacks->illeagleInstruction(callbacks->opaque, ins);
                        }
                        break;
                }
            }
            break;                                                                                                                        
    }
}

#endif