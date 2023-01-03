#include "CodeBlock.h"
#include "Environ.h"

CodeBlock::CodeBlock(Context &context, QObject *parent):
    QObject(parent),
    _cache(nullptr),
    _cacheSize(0),
    _context(context),
    _size(0)
{

}

CodeBlock::~CodeBlock()
{

}

size_t CodeBlock::addInstruction(TranslationInstruction &instruction)
{
    return 0;
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

// TranslationInstruction &CodeBlock::operator[](size_t offset)
// {
//     return nullptr;
// }

void CodeBlock::setCache(void *cache, size_t cacheSize)
{
    this->_cache = cache;
    this->_cacheSize = cacheSize; 
}