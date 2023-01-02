#ifndef MEMORY_MEMORY_H
#define MEMORY_MEMORY_H

#include <QObject>

#include "guest_defs.h"


class Memory: public QObject
{
    Q_OBJECT

public:
 
    Memory(QObject *parent = nullptr);
    virtual ~Memory();
    virtual void init() = 0;
    virtual bool isExecute() const = 0;
    virtual bool isRead() const = 0;
    virtual bool isWrite() const = 0;
    virtual bool performRead(GuestAddress offset, uint8_t *data, size_t size) const = 0;
    virtual bool performWrite(GuestAddress offset, const uint8_t *data, size_t size) = 0;
    virtual void reset() = 0;
    virtual size_t size() const = 0;
    virtual uint8_t *toBuffer(GuestAddress offset, size_t size) = 0;
};

#endif