#ifndef HOST_H
#define HOST_H

#include <QObject>
#include <QSharedPointer>

#include "Encoder.h"
#include "Platform.h"
#include "CodeBlock.h"
#include "global_defs.h"
#include "Environ.h"

class Host: public QObject
{
    Q_OBJECT

public:



    Host(QObject *parent = nullptr);
    ~Host();
    void destroyCode(QSharedPointer<CodeBlock> &block);
    Endian endianness();
    Platform &platform() { return this->_platform; }
    void runCode(QSharedPointer<CodeBlock> &block);

public signals:

    void blockExecutionStarting();
    void blockExecutionStopping();

private:

    void cleanUp();
    void init();

    Platform _platform;
    QSharedPointer<Encoder> _encoder;
};

#endif