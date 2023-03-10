#ifndef X86ENCODER_H
#define X86ENCODER_H

#include <QObject>

#include "Encoder.h"


class X86Encoder: public Encoder
{
    Q_OBJECT

public:

    X86Encoder(QObject *parent = nullptr);
    ~X86Encoder();
    virtual size_t encodeBlock(void *buffer, QSharedPointer<CodeBlock> &code);
    virtual size_t preamble(void *buffer, QSharedPointer<CodeBlock> &code);
    virtual size_t postamble(void *buffer, QSharedPointer<CodeBlock> &code);
};

#endif