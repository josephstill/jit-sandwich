#include "Context.h"
#include "Environ.h"

Context::Context(AddressSpace &space, QObject *parent): 
    QObject(parent),
    _addressSpace(space),
    _currentPc(0),
    _nextPc(0),
    _instructionLimit(0),
    _state(Context::FALLTHROUGH)
{

}

Context::~Context() 
{

}

void Context::addTranslationRegister(QSharedPointer<TranslationRegister> &reg, QVariant registerId) 
{
    this->_registers.insert(registerId.toString(), reg);
}

QSharedPointer<TranslationRegister> Context::getTranslationRegister(QVariant registerId) 
{
    QSharedPointer<TranslationRegister> ret;
    if (this->_registers.contains(registerId.toString()))
    {
        ret = this->_registers[registerId.toString()];      
    }
    return ret;
}

QSharedPointer<TranslationRegister> Context::getTempTranslationRegister(uint8_t size)
{
    QSharedPointer<TranslationRegister> ret(new TranslationRegister(size));
    return ret;
}

