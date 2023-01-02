#ifndef DECODER_H
#define DECODER_H

#include <QObject>
#include <QSharedPointer>

#include "CodeBlock.h"

class Decoder: public QObject
{
    Q_OBJECT

public:

    enum FlowType
    {
        FALLTHROUGH,
        BRANCH,
        CALL,
        RETURN,
        TRAP,
        ERROR
    };

    Decoder(QObject *parent = nullptr);
    ~Decoder();
    bool decode(QSharedPointer<CodeBlock> &block);

public signals:

    void blockCompleted(FlowType reason);    

protected:

    virtual bool blockInit(QSharedPointer<CodeBlock> &block) = 0;
    virtual bool blockComplete(QSharedPointer<CodeBlock> &block) = 0;
    virtual bool instructionComplete(QSharedPointer<CodeBlock> &block) = 0;
    virtual size_t instructionDecode(QSharedPointer<CodeBlock> &block, uint8_t *fetch) = 0;
    virtual bool instructionInit(QSharedPointer<CodeBlock> &block) = 0;


};

#endif