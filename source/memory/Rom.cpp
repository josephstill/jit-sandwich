#include "Rom.h"

Rom::Rom(QString filePath, QObject *parent):
    PhysicalMemory(filePath, true, false, parent) 
{
    
}

Rom::~Rom() 
{

}

void Rom::disableExecute()
{
    this->setExecute(false);
}

void Rom::enableExecute()
{
    this->setExecute(true);
}

uint8_t *PhysicalMemory::toBuffer(GuestAddress offset, size_t size)
{
        
}