#ifndef TRANSLATION_INSTRUCTION_H
#define TRANSLATION_INSTRUCTION_H

#include <QObject>
#include <QString>

#include "TranslationRegister.h"
#include "TranslationArgument.h"
#include "TranslationLabel.h"

/**
 * This class defines the IL used when translating instructions. Instructions
 * can take registers, immediate values, and labels. Each instruction has specific syntax
 * which shall be documented here.
 * 
 * 
 * TODO: This should be a table in the readme
 * Note: Reverse instructions are provided so that all instructions can have a similar format, 
 * explicitly the immediate value comes last in the argument list. Not all instructions will 
 * need a reverse.
 * 
 * Reverse arithmetic is implemented only when the following conditions apply:
 *     - the arithmetic operation does not have the commutative property
 *     - the operation involves an immediate value
 * 
 * Reverse comparisons are not necessary because of the relationship between logical operators. 
 * Equalities and inequalities can be rewritten such that the immediate always comes last. 
 *     - Example: the statement x > y is the same as the statements y < x and !(y >= x)
 * 
 * Instructions:
 * ADD   <dr> <sr1> <sr2>
 *       Add
 *       stores the sum of <sr1> and <sr2> into <dr>
 *       $dr = $sr1 + $sr2
 * ADDI  <dr> <sr> <immediate>
 *       Add Immediate
 *       stores the sum of <sr> and the immediate value into <dr>
 *       $dr = $sr1 + #immediate
 * BEQ   <src1> <src2> <label>
 *       Branch If Equals
 *       Branches to <label> if the contents of <src1> and <src2> are equal 
 *       if ($src1 == $src2) goto &label
 * BEQI  <src> <immediate> <label>
 *       Branch If Equals Immediate
 *       Branches to <label> if the contents of <src> and the immediate value are equal 
 *       if ($src1 == #immediate) goto &label 
 * LD    <dr> <ar>
 *       Load
 *       Stores the value from the address contained in <ar> into <dr> 
 *       $dr = *$ar
 * MOV   <dr> <sr>
 *       Move
 *       stores the value of <sr> in <dr> 
 *       $dr = $sr
 * MOVI  <dr> <immediate>
 *       Move Immediate
 *       stores the immediate value in <dr>
 *       $dr = #immediate
 * NOOP  
 *       No Operation
 *       Does nothing
 * RSUBI <dr> <sr> <immediate>
 *       Reverse Subtract Immediate
 *       stores the difference of the immediate value and <sr> into <dr>
 *       $dr = #immediate - $sr
 * ST    <ar> <vr>
 *       Store  
 *       stores the value in <vr> in the address contained in <ar>
 *       *$ar = $vr  
 * STI   <ar> <immediate>
 *       Store Immediate  
 *       stores the immediate value in the address contained in <ar>
 *       *$ar = #immediate
 * SUB   <dr> <sr1> <sr2>
 *       Subtract
 *       stores the difference of <sr1> and <sr2> into <dr>
 *       $dr = $sr1 - $sr2
 * SUBI  <dr> <sr> <immediate>
 *       Subtract Immediate
 *       stores the difference of <sr1> and the immediate value into <dr>
 *       $dr = $sr - #immediate
 */
class TranslationInstruction: public QObject
{
    Q_OBJECT

public:

    enum OpCode
    {
        NOOP,
        MOV, MOVI, 
        LD, ST, STI, 
        AND, ANDI, OR, ORI, XOR, XORI, SHL, SHLI, SHR, SHRI,
        NOT,
        ADD, ADDI, SUB, SUBI, RSUBI, MULT, MULTI, DIV, DIVI, RDIVI,
        LT, LTI, GT, GTI, 
        LTE, LTEI, GTE, GTEI, EQ, EQI, NE, NEI,
        BLT, BLTI, BGT, BGTI,
        BEQ, BEQI, BNE, BNEI,
        CF, OF
    };
    
    TranslationInstruction(OpCode opcode, QObject *parent = nullptr);
    ~TranslationInstruction();
    void allowOptimization() {this->_canOptimize = true; }
    bool canOptimize() const { return this->_canOptimize; }
    QSharedPointer<TranslationLabel> &destinationLabel() { return this->_destinationLabel; }
    OpCode opCode() const { return this->_op; }
    void setDestinationLabel(QSharedPointer<TranslationLabel> &label) { this->_destinationLabel = label; }
    void setGuestAsm(QString &newGuestAsm) { this->_guest_asm = newGuestAsm; }
    void setHostAsm(QString &newHostAsm) { this->_host_asm = newHostAsm; }
    void setSourceLabel(QSharedPointer<TranslationLabel> &label) { this->_sourceLabel = label; }
    void setTranslationAsm(QString &newTranslationAsm) { this->_translation_asm = newTranslationAsm; }
    QSharedPointer<TranslationLabel> &sourceLabel() { return this->_sourceLabel; }

    static QSharedPointer<TranslationInstruction> build(OpCode opcode);
    static QSharedPointer<TranslationInstruction> build(OpCode opcode, QSharedPointer<TranslationRegister> &dest, QSharedPointer<TranslationRegister> &src);
    static QSharedPointer<TranslationInstruction> build(OpCode opcode, QSharedPointer<TranslationRegister> &dest, QSharedPointer<TranslationArgument> &immediate);
    static QSharedPointer<TranslationInstruction> build(OpCode opcode, QSharedPointer<TranslationRegister> &dest, QSharedPointer<TranslationRegister> &src,  QSharedPointer<TranslationArgument> &immediate);
    static QSharedPointer<TranslationInstruction> build(OpCode opcode, QSharedPointer<TranslationRegister> &dest, QSharedPointer<TranslationRegister> &src1, QSharedPointer<TranslationRegister> &src2);
    static QSharedPointer<TranslationInstruction> build(OpCode opcode, QSharedPointer<TranslationRegister> &src1, QSharedPointer<TranslationRegister> &src2, QSharedPointer<TranslationLabel> &label);
    static QSharedPointer<TranslationInstruction> build(OpCode opcode, QSharedPointer<TranslationRegister> &src,  QSharedPointer<TranslationArgument> &immediate, QSharedPointer<TranslationLabel> &label);

private:

    bool                             _canOptimize;
    QSharedPointer<TranslationLabel> _destinationLabel;
    QString                          _guest_asm;
    QString                          _host_asm;
    OpCode                           _op;
    QSharedPointer<TranslationLabel> _sourceLabel;
    QString                          _translation_asm;
};

#endif