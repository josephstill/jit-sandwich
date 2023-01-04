#include "CodeBlock.h"
#include "Environ.h"

CodeBlock::CodeBlock(QObject *parent):
    QObject(parent),
    _cache(nullptr),
    _cacheSize(0),
    _size(0)
{

}

CodeBlock::~CodeBlock()
{

}

void CodeBlock::addInstruction(QSharedPointer<TranslationInstruction> instruction)
{
    this->_instructions.push_back(instruction);
}

void CodeBlock::deadCodeOptimization() 
{

}

void CodeBlock::finalize() 
{
    this->livenessOptimization();
    this->deadCodeOptimization();
}

void CodeBlock::livenessOptimization() 
{

}

void CodeBlock::setCache(void *cache, size_t cacheSize)
{
    this->_cache = cache;
    this->_cacheSize = cacheSize; 
}