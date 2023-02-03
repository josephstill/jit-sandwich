#include "TranslationInstruction.h"   
    
    
TranslationInstruction::TranslationInstruction(OpCode opcode, QObject *parent):
    QObject(parent),    
    _canOptimize(false),
    _destinationLabel(),   
    _op(opcode),
    _sourceLabel()
{

}

TranslationInstruction::~TranslationInstruction() 
{

}
 

QSharedPointer<TranslationInstruction> TranslationInstruction::build(OpCode opcode) 
{
    QSharedPointer<TranslationInstruction> ret(new TranslationInstruction(opcode));
    return ret;
}

QSharedPointer<TranslationInstruction> TranslationInstruction::build(OpCode opcode, 
                                                                     QSharedPointer<TranslationRegister> &dest, 
                                                                     QSharedPointer<TranslationRegister> &src) 
{
    QSharedPointer<TranslationInstruction> ret(new TranslationInstruction(opcode));
    return ret;
}

QSharedPointer<TranslationInstruction> TranslationInstruction::build(OpCode opcode, 
                                                                     QSharedPointer<TranslationRegister> &dest, 
                                                                     uint64_t immediate)
{
    QSharedPointer<TranslationInstruction> ret(new TranslationInstruction(opcode));
    return ret;
}

QSharedPointer<TranslationInstruction> TranslationInstruction::build(OpCode opcode, 
                                                                     QSharedPointer<TranslationRegister> &dest, 
                                                                     QSharedPointer<TranslationRegister> &src,  
                                                                     uint64_t immediate) 
{
    QSharedPointer<TranslationInstruction> ret(new TranslationInstruction(opcode));
    return ret;
}


QSharedPointer<TranslationInstruction> TranslationInstruction::build(OpCode opcode, 
                                                                     QSharedPointer<TranslationRegister> &dest, 
                                                                     QSharedPointer<TranslationRegister> &src1, 
                                                                     QSharedPointer<TranslationRegister> &src2) 
{
    QSharedPointer<TranslationInstruction> ret(new TranslationInstruction(opcode));
    return ret;
}


QSharedPointer<TranslationInstruction> TranslationInstruction::build(OpCode opcode, 
                                                                     QSharedPointer<TranslationRegister> &src1, 
                                                                     QSharedPointer<TranslationRegister> &src2, 
                                                                     QSharedPointer<TranslationLabel> &label) 
{
    QSharedPointer<TranslationInstruction> ret(new TranslationInstruction(opcode));
    return ret;
}


QSharedPointer<TranslationInstruction> TranslationInstruction::build(OpCode opcode, 
                                                                     QSharedPointer<TranslationRegister> &src,  
                                                                     uint64_t immediate, 
                                                                     QSharedPointer<TranslationLabel> &label) 
{
    QSharedPointer<TranslationInstruction> ret(new TranslationInstruction(opcode));
    return ret;
}
