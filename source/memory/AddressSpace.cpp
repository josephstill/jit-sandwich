#include "AddressSpace.h"


AddressSpace::AddressSpace(QObject *parent):
    QObject(parent) 
{

}

AddressSpace::~AddressSpace() 
{

}

void AddressSpace::getBuffer(GuestAddress offset, uint8_t **buffer, size_t *size)
{

}

