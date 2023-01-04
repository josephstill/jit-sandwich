#ifndef DECODER_H
#define DECODER_H

#include <QObject>
#include <QSharedPointer>

#include "CodeBlock.h"
#include "Context.h"

class Decoder: public QObject
{
    Q_OBJECT

public:

    Decoder(QObject *parent = nullptr);
    ~Decoder();
    bool decode(QSharedPointer<CodeBlock> &block, QSharedPointer<Context> &context);

protected:

    virtual bool blockDecodeComplete(QSharedPointer<CodeBlock> &block, QSharedPointer<Context> &context) = 0;
    virtual bool blockDecodeInit(QSharedPointer<CodeBlock> &block, QSharedPointer<Context> &context) = 0;
    virtual size_t instructionDecode(QSharedPointer<CodeBlock> &block, QSharedPointer<Context> &context, uint8_t *fetch) = 0;
    virtual bool instructionDecodeComplete(QSharedPointer<CodeBlock> &block, QSharedPointer<Context> &context) = 0;
    virtual bool instructionDecodeInit(QSharedPointer<CodeBlock> &block, QSharedPointer<Context> &context) = 0;


};

#endif