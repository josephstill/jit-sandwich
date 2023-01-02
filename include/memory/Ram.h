#ifndef MEMORY_RAM_H
#define MEMORY_RAM_H

#include <QObject>

#include "guest_defs.h"
#include "PhysicalMemory.h"


class Ram: public PhysicalMemory
{
    Q_OBJECT

public:
 
    Ram(size_t size, QObject *parent = nullptr);
    ~Ram();
    void disableExecute();
    void disableRead();
    void disableWrite();
    void enableExecute();
    void enableRead();
    void enableWrite();
    virtual bool performRead(GuestAddress offset, uint8_t *data, size_t size) const;
    virtual bool performWrite(GuestAddress offset, const uint8_t *data, size_t size);
};

#endif