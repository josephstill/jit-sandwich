#include "AARCH64Encoder.h"

AARCH64Encoder::AARCH64Encoder(QObject *parent):
    Encoder(parent) 
{

}

AARCH64Encoder::~AARCH64Encoder() 
{

}

size_t AARCH64Encoder::encodeBlock(void *buffer, QSharedPointer<CodeBlock> &code) 
{
    return 0;
}

size_t AARCH64Encoder::preamble(void *buffer, QSharedPointer<CodeBlock> &code) 
{
    return 0;
}

size_t AARCH64Encoder::postamble(void *buffer, QSharedPointer<CodeBlock> &code) 
{
    return 0;
}