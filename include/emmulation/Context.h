#ifndef CONTEXT_H
#define CONTEXT_H

#include <QObject>
#include "guest_defs.h"

class Context: public QObject 
{
    Q_OBJECT

public:

    Context(QObject *parent = nullptr);
    ~Context();
    GuestAddress nextPc() const {return 0;}

};

#endif