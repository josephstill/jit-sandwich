#include "PhysicalMemory.h"

#include <QFlag>

#include <cstring>

PhysicalMemory::PhysicalMemory(size_t size, QObject *parent):
    Memory(parent),
    backingFile(nullptr),
    buffer(new uint8_t(size)),
    execute(true),
    mediumReadOnly(false),
    read(true),
    _size(size),
    write(true) 
{

}

PhysicalMemory::PhysicalMemory(QString &filePath, bool read, bool write, QObject *parent):
    Memory(parent),
    backingFile(nullptr),
    buffer(nullptr),
    execute(false),
    mediumReadOnly(true),
    read(false),
    _size(0),
    write(false)     
{
    this->backingFile = new QFile(filePath);
    if (this->backingFile->exists())
    { 
        QFlags<QIODevice::OpenModeFlag> openMode = QIODevice::NotOpen;
        if (read)
        {
            this->read = true;
            openMode |= QIODevice::ReadOnly;
        }

        if (write)
        {
            this->write = true;
            this->mediumReadOnly = false;
            openMode |= QIODevice::WriteOnly;
        }

        this->backingFile->open(openMode);
        this->_size = this->backingFile->size();
        this->buffer = this->backingFile->map(0, this->_size);
    }

    if (!this->buffer)
    {
        QFileDevice::FileError err = this->backingFile->error();
        delete this->backingFile;
        throw err;
    }
}

PhysicalMemory::~PhysicalMemory() 
{
    if (this->backingFile)
    {
        this->backingFile->unmap(this->buffer);
        this->backingFile->close();
        delete this->backingFile;
    }
    else 
    {    
        delete this->buffer;
    }
}

void PhysicalMemory::init() 
{
    this->reset();
}

bool PhysicalMemory::performRead(GuestAddress offset, uint8_t *data, size_t size) const
{
    bool ret = false;
    if (this->read)
    {
        if (offset + size <= this->_size)
        {
            ::memcpy(data, this->buffer + offset, size);
            ret = true;
        }
    }
    return ret;
}

bool PhysicalMemory::performWrite(GuestAddress offset, const uint8_t *data, size_t size)
{
    bool ret = false;
    if (this->write)
    {
        if (offset + size <= this->_size)
        {
            ::memcpy(this->buffer + offset, data, size);
            ret = true;
        }
    }
    return ret;
}

void PhysicalMemory::reset() 
{
    if (!this->mediumReadOnly)
    {
        ::memset(this->buffer, 0, this->_size);
    }
}

uint8_t *PhysicalMemory::toBuffer(GuestAddress offset, size_t size)
{
    
}