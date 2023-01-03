#ifndef AARCH64ENCODER_H
#define AARCH64ENCODER_H

#include <QObject>

#include "Encoder.h"


class AARCH64Encoder: public Encoder
{
    Q_OBJECT

public:

    AARCH64Encoder(QObject *parent = nullptr);
    ~AARCH64Encoder();
    virtual size_t encodeBlock(void *buffer, QSharedPointer<CodeBlock> &code);
    virtual size_t preamble(void *buffer, QSharedPointer<CodeBlock> &code);
    virtual size_t postamble(void *buffer, QSharedPointer<CodeBlock> &code);
};

#endif