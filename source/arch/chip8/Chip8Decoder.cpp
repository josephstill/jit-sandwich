#include "Chip8Decoder.h"
#include "Environ.h"
#include "TranslationInstruction.h"
#include "TranslationRegister.h"
#include "guest_defs.h"

class DecodeState 
{
public:
    DecodeState(QSharedPointer<CodeBlock> &_code, QSharedPointer<Context> &_context):
        code(_code),
        context(_context)
    {

    }

    QSharedPointer<CodeBlock> &code;
    QSharedPointer<Context>   &context;
};

static void chip8_decode_callMachineCode(void *opaque, uint16_t address) 
{
    (void) opaque;
    (void) address;
}

static void chip8_decode_clearDisplay(void *opaque) 
{
    QSharedPointer<CodeBlock>   &block   = ((DecodeState *) opaque)->code;
    QSharedPointer<Context>     &context = ((DecodeState *) opaque)->context;
    
    GuestAddress screen_address = SCREEN_MEM; 
    uint8_t *buffer = nullptr;
    size_t buffer_size = END_MEM - SCREEN_MEM; 
    context->addressSpace().getBuffer(screen_address, &buffer, &buffer_size);

    for (uint32_t idx = 0; idx < buffer_size; idx += HOST_GP_REGISTER_SIZE)
    {
        QSharedPointer<TranslationRegister> temp(new TranslationRegister(HOST_GP_REGISTER_SIZE));
        block->addInstruction(TranslationInstruction::build(TranslationInstruction::MOVI, temp,(uint64_t) buffer + idx));
        block->addInstruction(TranslationInstruction::build(TranslationInstruction::STI,  temp, 0));
    }
}

static void chip8_decode_doReturn(void *opaque) 
{
    QSharedPointer<CodeBlock>   &block   = ((DecodeState *) opaque)->code;
    QSharedPointer<Context>     &context = ((DecodeState *) opaque)->context;
    
    QSharedPointer<TranslationRegister> pc = context->getTranslationRegister(QVariant("pc"));
    QSharedPointer<TranslationRegister> sp = context->getTranslationRegister(QVariant("sp"));
    block->addInstruction(TranslationInstruction::build(TranslationInstruction::LD, pc, sp));
    block->addInstruction(TranslationInstruction::build(TranslationInstruction::ADDI, sp, sp, pc->size()));

    context->setState(Context::DEVIATION);    
}

static void chip8_decode_doGoto(void *opaque, uint16_t address) 
{
    QSharedPointer<CodeBlock>   &block   = ((DecodeState *) opaque)->code;
    QSharedPointer<Context>     &context = ((DecodeState *) opaque)->context;
    
    QSharedPointer<TranslationRegister> pc = context->getTranslationRegister(QVariant("pc"));
    block->addInstruction(TranslationInstruction::build(TranslationInstruction::MOVI, pc, address));

    context->setState(Context::DEVIATION);       
}

static void chip8_decode_call(void *opaque, uint16_t address) 
{
    QSharedPointer<CodeBlock>   &block   = ((DecodeState *) opaque)->code;
    QSharedPointer<Context>     &context = ((DecodeState *) opaque)->context;
    
    QSharedPointer<TranslationRegister> pc = context->getTranslationRegister(QVariant("pc"));
    QSharedPointer<TranslationRegister> sp = context->getTranslationRegister(QVariant("sp"));

    block->addInstruction(TranslationInstruction::build(TranslationInstruction::SUBI, sp, sp, pc->size()));
    block->addInstruction(TranslationInstruction::build(TranslationInstruction::ST, sp, pc));
    block->addInstruction(TranslationInstruction::build(TranslationInstruction::MOVI, pc, address));
    
    context->setState(Context::DEVIATION);
}

static void chip8_decode_ifEqualsImm(void *opaque, uint8_t reg, uint8_t imm) 
{
    QSharedPointer<CodeBlock>   &block   = ((DecodeState *) opaque)->code;
    QSharedPointer<Context>     &context = ((DecodeState *) opaque)->context;
    
    QSharedPointer<TranslationLabel> branchNotTaken(new TranslationLabel());

    QSharedPointer<TranslationRegister> treg = context->getTranslationRegister(QVariant(reg));  
    QSharedPointer<TranslationRegister> pc   = context->getTranslationRegister(QVariant("pc"));
    QSharedPointer<TranslationRegister> temp(new TranslationRegister(sizeof(uint8_t)));
 
    block->addInstruction(TranslationInstruction::build(TranslationInstruction::XOR,  temp, temp, temp));   
    block->addInstruction(TranslationInstruction::build(TranslationInstruction::BNEI, treg, imm,  branchNotTaken));
    block->addInstruction(TranslationInstruction::build(TranslationInstruction::MOVI, temp, sizeof(Chip8Instruction)));

    QSharedPointer<TranslationInstruction> pcIncrement = TranslationInstruction::build(TranslationInstruction::ADD, pc, pc, temp);
    pcIncrement->setDestinationLabel(branchNotTaken);
    block->addInstruction(pcIncrement); 

    
    context->setState(Context::DEVIATION);
}

static void chip8_decode_ifNotEqualsImm(void *opaque, uint8_t reg, uint8_t imm) 
{
    QSharedPointer<CodeBlock>   &block   = ((DecodeState *) opaque)->code;
    QSharedPointer<Context>     &context = ((DecodeState *) opaque)->context;
    
    QSharedPointer<TranslationLabel> branchNotTaken(new TranslationLabel());

    QSharedPointer<TranslationRegister> treg = context->getTranslationRegister(QVariant(reg));  
    QSharedPointer<TranslationRegister> pc   = context->getTranslationRegister(QVariant("pc"));
    QSharedPointer<TranslationRegister> temp(new TranslationRegister(sizeof(uint8_t)));
 
    block->addInstruction(TranslationInstruction::build(TranslationInstruction::XOR,  temp, temp, temp));   
    block->addInstruction(TranslationInstruction::build(TranslationInstruction::BEQI, treg, imm,  branchNotTaken));
    block->addInstruction(TranslationInstruction::build(TranslationInstruction::MOVI, temp, sizeof(Chip8Instruction)));

    QSharedPointer<TranslationInstruction> pcIncrement = TranslationInstruction::build(TranslationInstruction::ADD, pc, pc, temp);
    pcIncrement->setDestinationLabel(branchNotTaken);
    block->addInstruction(pcIncrement);

    context->setState(Context::DEVIATION);
}

static void chip8_decode_ifEqualsReg(void *opaque, uint8_t regX, uint8_t regY) 
{
    QSharedPointer<CodeBlock>   &block   = ((DecodeState *) opaque)->code;
    QSharedPointer<Context>     &context = ((DecodeState *) opaque)->context;

    QSharedPointer<TranslationLabel> branchNotTaken(new TranslationLabel());

    QSharedPointer<TranslationRegister> tregX = context->getTranslationRegister(QVariant(regX));  
    QSharedPointer<TranslationRegister> tregY = context->getTranslationRegister(QVariant(regY)); 
    QSharedPointer<TranslationRegister> pc    = context->getTranslationRegister(QVariant("pc"));  
    QSharedPointer<TranslationRegister> temp(new TranslationRegister(sizeof(uint8_t)));

    block->addInstruction(TranslationInstruction::build(TranslationInstruction::XOR,  temp, temp, temp));    
    block->addInstruction(TranslationInstruction::build(TranslationInstruction::BNE,  tregX, tregY, branchNotTaken));
    block->addInstruction(TranslationInstruction::build(TranslationInstruction::MOVI, temp, sizeof(Chip8Instruction)));
    
    QSharedPointer<TranslationInstruction> pcIncrement = TranslationInstruction::build(TranslationInstruction::ADD, pc, pc, temp);
    pcIncrement->setDestinationLabel(branchNotTaken);
    block->addInstruction(pcIncrement);

    context->setState(Context::DEVIATION);            
}

static void chip8_decode_setRegisterImm(void *opaque, uint8_t reg, uint8_t imm) 
{
    QSharedPointer<CodeBlock>   &block   = ((DecodeState *) opaque)->code;
    QSharedPointer<Context>     &context = ((DecodeState *) opaque)->context;

    QSharedPointer<TranslationRegister> treg = context->getTranslationRegister(QVariant(reg));  

    block->addInstruction(TranslationInstruction::build(TranslationInstruction::MOVI, treg, imm));
}

static void chip8_decode_incrementImm(void *opaque, uint8_t reg, uint8_t imm) 
{
    QSharedPointer<CodeBlock>   &block   = ((DecodeState *) opaque)->code;
    QSharedPointer<Context>     &context = ((DecodeState *) opaque)->context;

    QSharedPointer<TranslationRegister> treg = context->getTranslationRegister(QVariant(reg));  

    block->addInstruction(TranslationInstruction::build(TranslationInstruction::ADDI, treg, treg, imm));    
}

static void chip8_decode_setRegister(void *opaque, uint8_t regX, uint8_t regY) 
{
    QSharedPointer<CodeBlock>   &block   = ((DecodeState *) opaque)->code;
    QSharedPointer<Context>     &context = ((DecodeState *) opaque)->context;

    QSharedPointer<TranslationRegister> tregX = context->getTranslationRegister(QVariant(regX));  
    QSharedPointer<TranslationRegister> tregY = context->getTranslationRegister(QVariant(regY)); 
    block->addInstruction(TranslationInstruction::build(TranslationInstruction::MOV, tregX, tregY));  
}

static void chip8_decode_orEquals(void *opaque, uint8_t regX, uint8_t regY) 
{
    QSharedPointer<CodeBlock>   &block   = ((DecodeState *) opaque)->code;
    QSharedPointer<Context>     &context = ((DecodeState *) opaque)->context;

    QSharedPointer<TranslationRegister> tregX = context->getTranslationRegister(QVariant(regX));  
    QSharedPointer<TranslationRegister> tregY = context->getTranslationRegister(QVariant(regY));     
    block->addInstruction(TranslationInstruction::build(TranslationInstruction::OR, tregX, tregX, tregY));    
}

static void chip8_decode_andEquals(void *opaque, uint8_t regX, uint8_t regY) 
{
    QSharedPointer<CodeBlock>   &block   = ((DecodeState *) opaque)->code;
    QSharedPointer<Context>     &context = ((DecodeState *) opaque)->context;

    QSharedPointer<TranslationRegister> tregX = context->getTranslationRegister(QVariant(regX));  
    QSharedPointer<TranslationRegister> tregY = context->getTranslationRegister(QVariant(regY)); 
    block->addInstruction(TranslationInstruction::build(TranslationInstruction::AND, tregX, tregX, tregY)); 
}

static void chip8_decode_xorEquals(void *opaque, uint8_t regX, uint8_t regY) 
{
    QSharedPointer<CodeBlock>   &block   = ((DecodeState *) opaque)->code;
    QSharedPointer<Context>     &context = ((DecodeState *) opaque)->context;

    QSharedPointer<TranslationRegister> tregX = context->getTranslationRegister(QVariant(regX));  
    QSharedPointer<TranslationRegister> tregY = context->getTranslationRegister(QVariant(regY)); 
    block->addInstruction(TranslationInstruction::build(TranslationInstruction::XOR, tregX, tregX, tregY)); 
}

static void chip8_decode_incrementReg(void *opaque, uint8_t regX, uint8_t regY) 
{
    QSharedPointer<CodeBlock>   &block   = ((DecodeState *) opaque)->code;
    QSharedPointer<Context>     &context = ((DecodeState *) opaque)->context;

    QSharedPointer<TranslationRegister> tregX = context->getTranslationRegister(QVariant(regX));  
    QSharedPointer<TranslationRegister> tregY = context->getTranslationRegister(QVariant(regY)); 
    block->addInstruction(TranslationInstruction::build(TranslationInstruction::ADD, tregX, tregX, tregY)); 
    // TODO VF
}

static void chip8_decode_decrementReg(void *opaque, uint8_t regX, uint8_t regY) 
{
    QSharedPointer<CodeBlock>   &block   = ((DecodeState *) opaque)->code;
    QSharedPointer<Context>     &context = ((DecodeState *) opaque)->context;

    QSharedPointer<TranslationRegister> tregX = context->getTranslationRegister(QVariant(regX));  
    QSharedPointer<TranslationRegister> tregY = context->getTranslationRegister(QVariant(regY)); 
    block->addInstruction(TranslationInstruction::build(TranslationInstruction::SUB, tregX, tregX, tregY)); 
    // TODO VF
}

static void chip8_decode_shiftRighImm(void *opaque, uint8_t regX, uint8_t regY) 
{
    QSharedPointer<CodeBlock>   &block   = ((DecodeState *) opaque)->code;
    QSharedPointer<Context>     &context = ((DecodeState *) opaque)->context;

    QSharedPointer<TranslationRegister> tregX = context->getTranslationRegister(QVariant(regX));  
    QSharedPointer<TranslationRegister> tregF = context->getTranslationRegister(QVariant(0xf)); 
    // REG_F = REG_X & 0x1
}

static void chip8_decode_reverseSubtract(void *opaque, uint8_t regX, uint8_t regY) 
{
    QSharedPointer<CodeBlock>   &block   = ((DecodeState *) opaque)->code;
    QSharedPointer<Context>     &context = ((DecodeState *) opaque)->context;

    QSharedPointer<TranslationRegister> tregX = context->getTranslationRegister(QVariant(regX));  
    QSharedPointer<TranslationRegister> tregY = context->getTranslationRegister(QVariant(regY)); 
    block->addInstruction(TranslationInstruction::build(TranslationInstruction::SUB, tregX, tregY, tregX)); 
    // TODO VF
}

static void chip8_decode_shiftLeftImm(void *opaque, uint8_t regX, uint8_t regY) 
{
    QSharedPointer<CodeBlock>   &block   = ((DecodeState *) opaque)->code;
    QSharedPointer<Context>     &context = ((DecodeState *) opaque)->context;

    QSharedPointer<TranslationRegister> tregX = context->getTranslationRegister(QVariant(regX));  
    QSharedPointer<TranslationRegister> tregF = context->getTranslationRegister(QVariant(0xf)); 
    // TODO look me up
}

static void chip8_decode_ifNotEqualsReg(void *opaque, uint8_t regX, uint8_t regY) 
{
    QSharedPointer<CodeBlock>   &block   = ((DecodeState *) opaque)->code;
    QSharedPointer<Context>     &context = ((DecodeState *) opaque)->context;

    QSharedPointer<TranslationLabel> branchNotTaken(new TranslationLabel());

    QSharedPointer<TranslationRegister> tregX = context->getTranslationRegister(QVariant(regX));  
    QSharedPointer<TranslationRegister> tregY = context->getTranslationRegister(QVariant(regY)); 
    QSharedPointer<TranslationRegister> pc   = context->getTranslationRegister(QVariant("pc"));  
    QSharedPointer<TranslationRegister> temp(new TranslationRegister(sizeof(uint8_t)));

    block->addInstruction(TranslationInstruction::build(TranslationInstruction::XOR,  temp, temp, temp));    
    block->addInstruction(TranslationInstruction::build(TranslationInstruction::BEQ,  tregX, tregY, branchNotTaken));
    block->addInstruction(TranslationInstruction::build(TranslationInstruction::MOVI, temp, sizeof(Chip8Instruction)));
    
    QSharedPointer<TranslationInstruction> pcIncrement = TranslationInstruction::build(TranslationInstruction::ADD, pc, pc, temp);
    pcIncrement->setDestinationLabel(branchNotTaken);
    block->addInstruction(pcIncrement);

    context->setState(Context::DEVIATION);         
}

static void chip8_decode_setAddress(void *opaque, uint16_t address) 
{
    QSharedPointer<CodeBlock>   &block   = ((DecodeState *) opaque)->code;
    QSharedPointer<Context>     &context = ((DecodeState *) opaque)->context;
}

static void chip8_decode_jumpOffset(void *opaque, uint16_t offset) 
{
    QSharedPointer<CodeBlock>   &block   = ((DecodeState *) opaque)->code;
    QSharedPointer<Context>     &context = ((DecodeState *) opaque)->context;
}

static void chip8_decode_rand(void *opaque, uint8_t reg, uint8_t mask) 
{
    QSharedPointer<CodeBlock>   &block   = ((DecodeState *) opaque)->code;
    QSharedPointer<Context>     &context = ((DecodeState *) opaque)->context;
}

static void chip8_decode_draw(void *opaque, uint8_t regX, uint8_t regY, uint8_t height) 
{
    QSharedPointer<CodeBlock>   &block   = ((DecodeState *) opaque)->code;
    QSharedPointer<Context>     &context = ((DecodeState *) opaque)->context;
}

static void chip8_decode_keyEquals(void *opaque, uint8_t regX) 
{
    QSharedPointer<CodeBlock>   &block   = ((DecodeState *) opaque)->code;
    QSharedPointer<Context>     &context = ((DecodeState *) opaque)->context;
}

static void chip8_decode_keyNotEquals(void *opaque, uint8_t regX) 
{
    QSharedPointer<CodeBlock>   &block   = ((DecodeState *) opaque)->code;
    QSharedPointer<Context>     &context = ((DecodeState *) opaque)->context;
}

static void chip8_decode_getDelay(void *opaque, uint8_t regX) 
{
    QSharedPointer<CodeBlock>   &block   = ((DecodeState *) opaque)->code;
    QSharedPointer<Context>     &context = ((DecodeState *) opaque)->context;
}

static void chip8_decode_getKey(void *opaque, uint8_t regX) 
{
    QSharedPointer<CodeBlock>   &block   = ((DecodeState *) opaque)->code;
    QSharedPointer<Context>     &context = ((DecodeState *) opaque)->context;
}

static void chip8_decode_setDelay(void *opaque, uint8_t regX) 
{
    QSharedPointer<CodeBlock>   &block   = ((DecodeState *) opaque)->code;
    QSharedPointer<Context>     &context = ((DecodeState *) opaque)->context;
}

static void chip8_decode_setSound(void *opaque, uint8_t regX) 
{
    QSharedPointer<CodeBlock>   &block   = ((DecodeState *) opaque)->code;
    QSharedPointer<Context>     &context = ((DecodeState *) opaque)->context;
}

static void chip8_decode_incrementI(void *opaque, uint8_t regX) 
{
    QSharedPointer<CodeBlock>   &block   = ((DecodeState *) opaque)->code;
    QSharedPointer<Context>     &context = ((DecodeState *) opaque)->context;
}

static void chip8_decode_loadSprite(void *opaque, uint8_t regX) 
{
    QSharedPointer<CodeBlock>   &block   = ((DecodeState *) opaque)->code;
    QSharedPointer<Context>     &context = ((DecodeState *) opaque)->context;
}

static void chip8_decode_storeBCD(void *opaque, uint8_t regX) 
{
    QSharedPointer<CodeBlock>   &block   = ((DecodeState *) opaque)->code;
    QSharedPointer<Context>     &context = ((DecodeState *) opaque)->context;
}

static void chip8_decode_push(void *opaque, uint8_t regX) 
{
    QSharedPointer<CodeBlock>   &block   = ((DecodeState *) opaque)->code;
    QSharedPointer<Context>     &context = ((DecodeState *) opaque)->context;
}

static void chip8_decode_pop(void *opaque, uint8_t regX) 
{
    QSharedPointer<CodeBlock>   &block   = ((DecodeState *) opaque)->code;
    QSharedPointer<Context>     &context = ((DecodeState *) opaque)->context;
}

static void chip8_decode_illeagleInstruction(void *opaque, Chip8Instruction instruction) 
{
    QSharedPointer<CodeBlock>   &block   = ((DecodeState *) opaque)->code;
    QSharedPointer<Context>     &context = ((DecodeState *) opaque)->context;
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
bool Chip8Decoder::blockDecodeComplete(QSharedPointer<CodeBlock> &block, QSharedPointer<Context> &context) 
{
    return false;
}

bool Chip8Decoder::blockDecodeInit(QSharedPointer<CodeBlock> &block, QSharedPointer<Context> &context) 
{
    return false;
}

size_t Chip8Decoder::instructionDecode(QSharedPointer<CodeBlock> &block, QSharedPointer<Context> &context, uint8_t *fetch) 
{
    Chip8Instruction instriction = *((Chip8Instruction *) fetch);
    this->callbacks.opaque = (void *) new DecodeState(block, context);
    decodeChip8(instriction, &this->callbacks);
    delete (DecodeState *) this->callbacks.opaque;
    this->callbacks.opaque = nullptr;
    return sizeof(uint16_t);
}

bool Chip8Decoder::instructionDecodeComplete(QSharedPointer<CodeBlock> &block, QSharedPointer<Context> &context) 
{
    return false;
}

bool Chip8Decoder::instructionDecodeInit(QSharedPointer<CodeBlock> &block, QSharedPointer<Context> &context) 
{
    return false;
}