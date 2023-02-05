#include "TranslationInstruction.h"   
    
    
TranslationInstruction::TranslationInstruction(OpCode opcode, QObject *parent):
    QObject(parent),    
    _canOptimize(false),
    _destinationLabel(),   
    _guest_asm(),
    _host_asm(),
    _op(opcode),
    _sourceLabel(),
    _translation_asm()
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
                                                                     QSharedPointer<TranslationArgument> &immediate)
{
    QSharedPointer<TranslationInstruction> ret(new TranslationInstruction(opcode));
    return ret;
}

QSharedPointer<TranslationInstruction> TranslationInstruction::build(OpCode opcode, 
                                                                     QSharedPointer<TranslationRegister> &dest, 
                                                                     QSharedPointer<TranslationRegister> &src,  
                                                                     QSharedPointer<TranslationArgument> &immediate) 
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
                                                                     QSharedPointer<TranslationArgument> &immediate, 
                                                                     QSharedPointer<TranslationLabel> &label) 
{
    QSharedPointer<TranslationInstruction> ret(new TranslationInstruction(opcode));
    return ret;
}
