#ifndef TRANSLATION_INSTRUCTION_H
#define TRANSLATION_INSTRUCTION_H

#include <QObject>
#include <iostream>

#include "TranslationRegister.h"
#include "TranslationLabel.h"

class TranslationInstruction: public QObject
{
    Q_OBJECT

public:

    enum OpCode
    {
        MOV, MOVI, 
        LD, LDI, ST, STI, 
        AND, ANDI, OR, ORI, XOR, XORI, SHL, SHLI, SHR, SHRI,
        NOT,
        ADD, ADDI, SUB, SUBI, MULT, MULTI, DIV, DIVI,
        LT, LTI, GT, GTI, 
        LTE, LTEI, GTE, GTEI, EQ, EQI, NE, NEI,
        BLT, BLTI, BGT, BGTI, 
        BLTE, BLTEI, BGTE, BGTEI, BEQ, BEQI, BNE, BNEI,          
        RET
    };
    
    TranslationInstruction(OpCode opcode, QObject *parent = nullptr);
    TranslationInstruction(OpCode opcode, TranslationRegister &dest, TranslationRegister &src,  QObject *parent = nullptr);
    TranslationInstruction(OpCode opcode, TranslationRegister &dest, TranslationRegister &src,  uint64_t immediate, QObject *parent = nullptr);
    TranslationInstruction(OpCode opcode, TranslationRegister &dest, TranslationRegister &src1, TranslationRegister &src2, QObject *parent = nullptr);
    TranslationInstruction(const TranslationInstruction &instruction);
    ~TranslationInstruction();
    void addDestLabel(QSharedPointer<TranslationLabel> &destLabel) { this->_destLabel = destLabel; }
    void addSourceLabel(QSharedPointer<TranslationLabel> &srcLabel) { this->_sourceLabel = srcLabel; }
    bool valid() const { return this->_valid; }

    friend ostream& operator<<(ostream& os, const TranslationInstruction &inst);

private:

    QSharedPointer<TranslationLabel> _destLabel;
    uint64_t                         _immediate; 
    OpCode                           _op;
    QSharedPointer<TranslationLabel> _sourceLabel;
    bool                             _valid;




};

#endif