#include "X86Encoder.h"

X86Encoder::X86Encoder(QObject *parent):
    Encoder(parent)
{

}

X86Encoder::~X86Encoder() 
{

}

size_t X86Encoder::encodeBlock(void *buffer, QSharedPointer<CodeBlock> &code) 
{
    return 0;
}

size_t X86Encoder::preamble(void *buffer, QSharedPointer<CodeBlock> &code) 
{
    return 0;
}

size_t X86Encoder::postamble(void *buffer, QSharedPointer<CodeBlock> &code) 
{
    return 0;
}