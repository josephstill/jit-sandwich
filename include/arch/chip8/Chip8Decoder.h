#ifndef CHIP8_DECODER_H
#define CHIP8_DECODER_H

#include <QObject>

#include "Decoder.h"
#include "Chip8DecoderUtils.h"

class Chip8Decoder: public Decoder 
{
    Q_OBJECT

public: 

    Chip8Decoder(QObject *parent = nullptr);
    ~Chip8Decoder();

protected:

    virtual bool blockDecodeComplete(QSharedPointer<CodeBlock> &block, QSharedPointer<Context> &context);
    virtual bool blockDecodeInit(QSharedPointer<CodeBlock> &block, QSharedPointer<Context> &context);
    virtual size_t instructionDecode(QSharedPointer<CodeBlock> &block, QSharedPointer<Context> &context, uint8_t *fetch);
    virtual bool instructionDecodeComplete(QSharedPointer<CodeBlock> &block, QSharedPointer<Context> &context);
    virtual bool instructionDecodeInit(QSharedPointer<CodeBlock> &block, QSharedPointer<Context> &context);

private:

    DecoderCallbacks callbacks;

};

#endif