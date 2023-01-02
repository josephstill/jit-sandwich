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

    virtual bool blockInit(QSharedPointer<CodeBlock> &block);
    virtual bool blockComplete(QSharedPointer<CodeBlock> &block);
    virtual bool instructionComplete(QSharedPointer<CodeBlock> &block);
    virtual size_t instructionDecode(QSharedPointer<CodeBlock> &block, uint8_t *decode);
    virtual bool instructionInit(QSharedPointer<CodeBlock> &block);

private:

    DecoderCallbacks callbacks;

};

#endif