#ifndef ENCODER_H
#define ENCODER_H

#include <QObject>

#include "CodeBlock.h"

class Encoder: public QObject
{
    Q_OBJECT

public:

    Encoder(QObject *parent = nullptr);
    ~Encoder();

    virtual size_t encodeBlock(void *buffer, QSharedPointer<CodeBlock> &code) = 0;
    virtual size_t preamble(void *buffer, QSharedPointer<CodeBlock> &code) = 0;
    virtual size_t postamble(void *buffer, QSharedPointer<CodeBlock> &code) = 0;
};

#endif