#include "Host.h"

Host::Host(QObject *parent):
    QObject(parent),
    _platform(),
    _encoder(nullptr)
{
    this->init();
}

Host::~Host()
{
    this->cleanUp();
}

void Host::destroyCode(QSharedPointer<CodeBlock> &block)
{
    void *code = block->cache();
    size_t block_size = block->size();
    if (code)
    {
        this->_platform.freeCodeBuffer(code, block_size);
        block->setCache(nullptr, 0);
    }
}

void Host::runCode(QSharedPointer<CodeBlock> &block)
{
    typedef void (*execute)(void);

    void *code      = nullptr;
    size_t codeSize = 0;
    execute e       = nullptr;

    if (!block->cache())
    {
        // TODO: account for pre/post amble 
        code = this->_platform.allocateCodeBuffer(MAX_HOST_INSTRUCTION_SIZE * block->size());
    
        codeSize += this->_encoder->preamble(code, block);
        codeSize += this->_encoder->encodeBlock(code, block);
        codeSize += this->_encoder->postamble(code, block);

        this->_platform.lockCodeBuffer(code, codeSize);
        block->setCache(code, codeSize);
    }

    e = (execute) block->cache();
    emit blockExecutionStarting();
    e();
    emit blockExecutionStopping();
}

#ifdef Q_PROCESSOR_X86
#include "X86Host.cpp.inc"
#elif defined(Q_PROCESSOR_ARM)
#include "ARMHost.cpp.inc"
#else
#error "Undefined host processor type"
#endif
