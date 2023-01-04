#ifndef CONTEXT_H
#define CONTEXT_H

#include <QObject>
#include <QVariant>
#include <QMap>
#include <QString>
#include <QString>
#include <QSharedPointer>

#include "AddressSpace.h"
#include "TranslationRegister.h"
#include "TranslationLabel.h"
#include "guest_defs.h"


class Context: public QObject 
{
    Q_OBJECT

public:

    enum ContextState 
    {
        FALLTHROUGH,
        DEVIATION,
        TRAP,
        INTERNAL_ERROR
    };

    Context(AddressSpace &space, QObject *parent = nullptr);
    ~Context();
    AddressSpace &addressSpace() { return this->_addressSpace; }
    void addTranslationRegister(QSharedPointer<TranslationRegister> &reg, QVariant registerId);
    GuestAddress currentPc() const { return this->_currentPc; }
    QSharedPointer<TranslationRegister> getTranslationRegister(QVariant registerId);
    uint64_t instructionLimit() const { return this->_instructionLimit; }
    GuestAddress nextPc() const { return this->_nextPc; }
    void setCurrentPc(GuestAddress cPc) { this->_currentPc = cPc; }
    void setInstructionLimit(uint64_t newInstructionLimit) { this->_instructionLimit = newInstructionLimit; }
    void setNextPc(GuestAddress newNextPc) { this->_nextPc = newNextPc; }
    void setState(ContextState newState) { this->_state = newState; }
    ContextState state() const { return this->_state; }

private:

    AddressSpace                        &_addressSpace;
    GuestAddress                       _currentPc;
    GuestAddress                       _nextPc;
    uint64_t                           _instructionLimit;
    ContextState                       _state;

    QMap<QString, QSharedPointer<TranslationRegister>> _registers;
};

#endif