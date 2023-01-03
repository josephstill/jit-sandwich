#include "Chip8Decoder.h"

static void chip8_decode_callMachineCode(void *opaque, uint16_t address) 
{
    // Intentionally left blank
}

static void chip8_decode_clearDisplay(void *opaque) 
{
    CodeBlock *block = (CodeBlock *) opaque;
}

static void chip8_decode_doReturn(void *opaque) 
{
    CodeBlock *block = (CodeBlock *) opaque;
}

static void chip8_decode_doGoto(void *opaque, uint16_t address) 
{
    CodeBlock *block = (CodeBlock *) opaque;
}

static void chip8_decode_call(void *opaque, uint16_t address) 
{
    CodeBlock *block = (CodeBlock *) opaque;
}

static void chip8_decode_ifEqualsImm(void *opaque, uint8_t reg, uint8_t imm) 
{
    CodeBlock *block = (CodeBlock *) opaque;
}

static void chip8_decode_ifNotEqualsImm(void *opaque, uint8_t reg, uint8_t imm) 
{
    CodeBlock *block = (CodeBlock *) opaque;
}

static void chip8_decode_ifEqualsReg(void *opaque, uint8_t regX, uint8_t regY) 
{
    CodeBlock *block = (CodeBlock *) opaque;
}

static void chip8_decode_setRegisterImm(void *opaque, uint8_t reg, uint8_t imm) 
{
    CodeBlock *block = (CodeBlock *) opaque;
}

static void chip8_decode_incrementImm(void *opaque, uint8_t reg, uint8_t imm) 
{
    CodeBlock *block = (CodeBlock *) opaque;
}

static void chip8_decode_setRegister(void *opaque, uint8_t regX, uint8_t regY) 
{
    CodeBlock *block = (CodeBlock *) opaque;
}

static void chip8_decode_orEquals(void *opaque, uint8_t regX, uint8_t regY) 
{
    CodeBlock *block = (CodeBlock *) opaque;
}

static void chip8_decode_andEquals(void *opaque, uint8_t regX, uint8_t regY) 
{
    CodeBlock *block = (CodeBlock *) opaque;
}

static void chip8_decode_xorEquals(void *opaque, uint8_t regX, uint8_t regY) 
{
    CodeBlock *block = (CodeBlock *) opaque;
}

static void chip8_decode_incrementReg(void *opaque, uint8_t regX, uint8_t regY) 
{
    CodeBlock *block = (CodeBlock *) opaque;
}

static void chip8_decode_decrementReg(void *opaque, uint8_t regX, uint8_t regY) 
{
    CodeBlock *block = (CodeBlock *) opaque;
}

static void chip8_decode_shiftRighImm(void *opaque, uint8_t regX, uint8_t regY) 
{
    CodeBlock *block = (CodeBlock *) opaque;
}

static void chip8_decode_reverseSubtract(void *opaque, uint8_t regX, uint8_t regY) 
{
    CodeBlock *block = (CodeBlock *) opaque;
}

static void chip8_decode_shiftLeftImm(void *opaque, uint8_t regX, uint8_t regY) 
{
    CodeBlock *block = (CodeBlock *) opaque;
}

static void chip8_decode_ifNotEqualsReg(void *opaque, uint8_t regX, uint8_t regY) 
{
    CodeBlock *block = (CodeBlock *) opaque;
}

static void chip8_decode_setAddress(void *opaque, uint16_t address) 
{
    CodeBlock *block = (CodeBlock *) opaque;
}

static void chip8_decode_jumpOffset(void *opaque, uint16_t offset) 
{
    CodeBlock *block = (CodeBlock *) opaque;
}

static void chip8_decode_rand(void *opaque, uint8_t reg, uint8_t mask) 
{
    CodeBlock *block = (CodeBlock *) opaque;
}

static void chip8_decode_draw(void *opaque, uint8_t regX, uint8_t regY, uint8_t height) 
{
    CodeBlock *block = (CodeBlock *) opaque;
}

static void chip8_decode_keyEquals(void *opaque, uint8_t regX) 
{
    CodeBlock *block = (CodeBlock *) opaque;
}

static void chip8_decode_keyNotEquals(void *opaque, uint8_t regX) 
{
    CodeBlock *block = (CodeBlock *) opaque;
}

static void chip8_decode_getDelay(void *opaque, uint8_t regX) 
{
    CodeBlock *block = (CodeBlock *) opaque;
}

static void chip8_decode_getKey(void *opaque, uint8_t regX) 
{
    CodeBlock *block = (CodeBlock *) opaque;
}

static void chip8_decode_setDelay(void *opaque, uint8_t regX) 
{
    CodeBlock *block = (CodeBlock *) opaque;
}

static void chip8_decode_setSound(void *opaque, uint8_t regX) 
{
    CodeBlock *block = (CodeBlock *) opaque;
}

static void chip8_decode_incrementI(void *opaque, uint8_t regX) 
{
    CodeBlock *block = (CodeBlock *) opaque;
}

static void chip8_decode_loadSprite(void *opaque, uint8_t regX) 
{
    CodeBlock *block = (CodeBlock *) opaque;
}

static void chip8_decode_storeBCD(void *opaque, uint8_t regX) 
{
    CodeBlock *block = (CodeBlock *) opaque;
}

static void chip8_decode_push(void *opaque, uint8_t regX) 
{
    CodeBlock *block = (CodeBlock *) opaque;
}

static void chip8_decode_pop(void *opaque, uint8_t regX) 
{
    CodeBlock *block = (CodeBlock *) opaque;
}

static void chip8_decode_illeagleInstruction(void *opaque, Chip8Instruction instruction) 
{
    CodeBlock *block = (CodeBlock *) opaque;
}

Chip8Decoder::Chip8Decoder(QObject *parent):
    Decoder(parent)
{
    this->callbacks.callMachineCode = chip8_decode_callMachineCode;
    this->callbacks.clearDisplay = chip8_decode_clearDisplay;
    this->callbacks.doReturn = chip8_decode_doReturn;
    this->callbacks.doGoto = chip8_decode_doGoto;
    this->callbacks.call = chip8_decode_call;
    this->callbacks.ifEqualsImm = chip8_decode_ifEqualsImm;
    this->callbacks.ifNotEqualsImm = chip8_decode_ifNotEqualsImm;
    this->callbacks.ifEqualsReg = chip8_decode_ifEqualsReg;
    this->callbacks.setRegisterImm = chip8_decode_setRegisterImm;
    this->callbacks.incrementImm = chip8_decode_incrementImm;
    this->callbacks.setRegister = chip8_decode_setRegister;
    this->callbacks.orEquals = chip8_decode_orEquals;
    this->callbacks.andEquals = chip8_decode_andEquals;
    this->callbacks.xorEquals = chip8_decode_xorEquals;
    this->callbacks.incrementReg = chip8_decode_incrementReg;
    this->callbacks.decrementReg = chip8_decode_decrementReg;
    this->callbacks.shiftRighImm = chip8_decode_shiftRighImm;
    this->callbacks.reverseSubtract = chip8_decode_reverseSubtract;
    this->callbacks.shiftLeftImm = chip8_decode_shiftLeftImm;
    this->callbacks.ifNotEqualsReg = chip8_decode_ifNotEqualsReg;
    this->callbacks.setAddress = chip8_decode_setAddress;
    this->callbacks.jumpOffset = chip8_decode_jumpOffset;
    this->callbacks.rand = chip8_decode_rand;
    this->callbacks.draw = chip8_decode_draw;
    this->callbacks.keyEquals = chip8_decode_keyEquals;
    this->callbacks.keyNotEquals = chip8_decode_keyNotEquals;
    this->callbacks.getDelay = chip8_decode_getDelay;
    this->callbacks.getKey = chip8_decode_getKey;
    this->callbacks.setDelay = chip8_decode_setDelay;
    this->callbacks.setSound = chip8_decode_setSound;
    this->callbacks.incrementI = chip8_decode_incrementI;
    this->callbacks.loadSprite = chip8_decode_loadSprite;
    this->callbacks.storeBCD = chip8_decode_storeBCD;
    this->callbacks.push = chip8_decode_push;
    this->callbacks.pop = chip8_decode_pop;
    this->callbacks.illeagleInstruction = chip8_decode_illeagleInstruction;
}

Chip8Decoder::~Chip8Decoder()
{

}

bool Chip8Decoder::blockInit(QSharedPointer<CodeBlock> &block) 
{
    return false;
}

bool Chip8Decoder::blockComplete(QSharedPointer<CodeBlock> &block) 
{
    return false;
}

bool Chip8Decoder::instructionComplete(QSharedPointer<CodeBlock> &block) 
{
    return false;
}

size_t Chip8Decoder::instructionDecode(QSharedPointer<CodeBlock> &block, uint8_t *fetch) 
{
    Chip8Instruction instriction = *((Chip8Instruction *) fetch);
    this->callbacks.opaque = block.data();
    decodeChip8(instriction, &this->callbacks);
    this->callbacks.opaque = nullptr;
    return sizeof(uint16_t);
}

bool Chip8Decoder::instructionInit(QSharedPointer<CodeBlock> &block) 
{
    return false;
}