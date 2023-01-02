#ifndef MEMORY_ROM_H
#define MEMORY_ROM_H

#include <QObject>

#include "guest_defs.h"
#include "PhysicalMemory.h"


class Rom: public PhysicalMemory
{
    Q_OBJECT

public:
 
    Rom(QString filePath, QObject *parent = nullptr);
    ~Rom();
    void disableExecute();
    void enableExecute();
    virtual uint8_t *toBuffer(GuestAddress offset, size_t size);
};

#endif