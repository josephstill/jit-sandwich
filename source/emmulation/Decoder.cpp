#include "Decoder.h"

Decoder::Decoder(QObject *parent):
    QObject(parent)
{

}

Decoder::~Decoder()
{

}

bool Decoder::decode(QSharedPointer<CodeBlock> &block, QSharedPointer<Context> &context)
{
    return false;
}