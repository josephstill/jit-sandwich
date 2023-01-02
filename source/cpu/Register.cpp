#include <cstring>

#include "Register.h"

Register::Register(size_t size, QObject *parent):
    QObject(parent),
    source(nullptr),
    data(new uint8_t(size)),
    _size(size)
{

}

Register::Register(uint8_t *data, size_t size, Register *source, QObject *parent):
    QObject(parent),
    source(source),
    data(data),
    _size(size)
{
    
}

Register::~Register() 
{
    if (!this->source)
    {
        for (QList<QSharedPointer<Register>>::iterator it = this->aliases.begin(); it != this->aliases.end(); ++it)
        {
            (*it)->detatch(nullptr);
        }
        delete this->data; 
    }
    else
    {
        while (!this->aliases.isEmpty())
        {
            this->source->takeAlias(this->aliases.front());
            this->aliases.pop_front();
        }      
    }
}

QSharedPointer<Register> Register::alias()
{
    return this->alias(this->_size, 0);
}

QSharedPointer<Register> Register::alias(size_t size, uint8_t offset)
{
    if (offset + size <= this->_size)
    {
        QSharedPointer<Register> reg = QSharedPointer<Register>(new Register(this->data + offset, size, this));
        this->aliases.push_back(reg);
        return reg;
    }  
        
    QSharedPointer<Register> dummy = QSharedPointer<Register>(new Register(size));
    return dummy;
}

void Register::detatch(uint8_t *data)
{
    uint8_t *newData = data;

    if (!newData)
    {
        newData = new uint8_t(this->_size);
        ::memcpy(newData, this->data, this->_size);
        this->source = nullptr;
    }

    for (QList<QSharedPointer<Register>>::iterator it = this->aliases.begin(); it != this->aliases.end(); ++it)
    {
        size_t  offset = (*it)->buffer() - this->data;
        uint8_t *aliasData = newData + offset;
        (*it)->detatch(aliasData);
    }    

    this->data = newData;
}

void Register::init()
{
    this->reset();
}

void Register::read(uint8_t *buffer, size_t size) 
{
    size_t readSize = size <= this->_size ? size : this->_size;
    ::memcpy(buffer, this->data, readSize);
}

void Register::reset()
{
    if (!this->source)
    {
        ::memset(this->data, 0, this->_size);
    }
}

void Register::takeAlias(QSharedPointer<Register> alias) 
{
    alias->takeSource(this);
    this->aliases.push_back(alias);
}

void Register::write(uint8_t *buffer, size_t size) 
{
    size_t writeSize = size <= this->_size ? size : this->_size;
    ::memset(this->data, 0, this->_size);
    ::memcpy(this->data, buffer, writeSize);
}