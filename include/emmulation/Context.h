#ifndef CONTEXT_H
#define CONTEXT_H

#include <QObject>

#include "Host.h"
#include "guest_defs.h"

class Context: public QObject 
{
    Q_OBJECT

public:

    Context(QObject *parent = nullptr);
    ~Context();
    Host &host() { return this->_host; }
    GuestAddress nextPc() const {return 0;}

private:

    Host _host;
};

#endif