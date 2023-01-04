#ifndef ADDRESSSPACE_H
#define ADDRESSSPACE_H

#include <QObject>
#include <QSharedPointer>
#include <QMap>

#include "Memory.h"
#include "guest_defs.h"

class AddressSpace: public QObject 
{
    Q_OBJECT

public:
    
    AddressSpace(QObject *parent = nullptr);
    ~AddressSpace();
    void getBuffer(GuestAddress offset, uint8_t **buffer, size_t *size);

private:

    QMap<GuestAddress, QSharedPointer<Memory>> _address_space;
};

#endif
