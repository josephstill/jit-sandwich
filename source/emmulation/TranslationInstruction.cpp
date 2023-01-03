#include "TranslationInstruction.h"   
    
    
TranslationInstruction::TranslationInstruction(OpCode opcode, QObject *parent):
    QObject(parent),    
    _destLabel(nullptr),
    _immediate(0),
    _op(opcode),
    _sourceLabel(nullptr),
    _valid(false) 
{

}

TranslationInstruction::TranslationInstruction(OpCode opcode, TranslationRegister &dest, TranslationRegister &src,  QObject *parent):
    QObject(parent),    
    _destLabel(nullptr),
    _immediate(0),
    _op(opcode),
    _sourceLabel(nullptr),
    _valid(false)
{

}

TranslationInstruction::TranslationInstruction(OpCode opcode, TranslationRegister &dest, TranslationRegister &src,  uint64_t immediate, QObject *parent): 
    QObject(parent),    
    _destLabel(nullptr),
    _immediate(0),
    _op(opcode),
    _sourceLabel(nullptr),
    _valid(false)
{

}

TranslationInstruction::TranslationInstruction(OpCode opcode, TranslationRegister &dest, TranslationRegister &src1, TranslationRegister &src2, QObject *parent): 
    QObject(parent),    
    _destLabel(nullptr),
    _immediate(0),
    _op(opcode),
    _sourceLabel(nullptr),
    _valid(false)
{

}

TranslationInstruction::TranslationInstruction(const TranslationInstruction &instruction):
    QObject(instruction.parent()) 
{

}

TranslationInstruction::~TranslationInstruction() 
{

}
 
std::ostream& operator<<(std::ostream& os, const TranslationInstruction &inst)
{
    return os;
}