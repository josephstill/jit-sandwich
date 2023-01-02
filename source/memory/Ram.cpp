#include "Ram.h"


Ram::Ram(size_t size, QObject *parent): 
    PhysicalMemory(size, parent)
{

}

Ram::~Ram() 
{

}

void Ram::disableExecute() 
{
    this->setExecute(false);
}

void Ram::disableRead() 
{
    this->setRead(false);
}

void Ram::disableWrite() 
{
    this->setWrite(false);
}

void Ram::enableExecute() 
{
    this->setExecute(true);
}

void Ram::enableRead() 
{
    this->setRead(true);
}

void Ram::enableWrite() 
{
    this->setWrite(true);
}

bool Ram::performRead(GuestAddress offset, uint8_t *data, size_t size) const 
{
    return PhysicalMemory::performRead(offset, data, size);
}

bool Ram::performWrite(GuestAddress offset, const uint8_t *data, size_t size) 
{
    return PhysicalMemory::performWrite(offset, data, size);
}