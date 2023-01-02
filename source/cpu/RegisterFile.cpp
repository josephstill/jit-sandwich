#include "RegisterFile.h"

RegisterFile::RegisterFile(QObject *parent):
    QObject(parent) 
{

}

RegisterFile::~RegisterFile() 
{

}

QSharedPointer<Register> RegisterFile::addRegister(QString name, size_t size) 
{
    if (!this->registers.contains(name))
    {
        QSharedPointer<Register> reg = QSharedPointer<Register>(new Register(size));
        this->registers[name] = reg;
    }
    return this->registers[name];
}

QSharedPointer<Register> RegisterFile::addRegister(QString name, QString aliasOf, size_t size, size_t offset)
{
    QSharedPointer<Register> nullReturn;
    if (!this->registers.contains(name))
    {
        if (this->registers.contains(aliasOf))
        {
            QSharedPointer<Register> reg = this->registers[aliasOf]->alias(size, offset);
            this->registers[name] = reg;
        }
        else
        {
            return nullReturn;
        }

    }
    return this->registers[name];
    
}

QSharedPointer<Register> RegisterFile::getRegister(QString name) 
{
    QSharedPointer<Register> nullReturn;
    if (this->registers.contains(name))
    {
        return this->registers[name];
    }
    return nullReturn;
}

void RegisterFile::init()
{
    this->reset();
}

void RegisterFile::reset()
{
    for (QMap<QString, QSharedPointer<Register>>::iterator it = this->registers.begin(); it != this->registers.end(); ++it)
    {
        it.value()->reset();
    }
}