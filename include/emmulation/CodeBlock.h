#ifndef CODE_BLOCK_H
#define CODE_BLOCK_H

#include <QObject>
#include "Context.h"
#include "TranslationInstruction.h"

class CodeBlock: public QObject
{
    Q_OBJECT

public:

    CodeBlock(Context &context, QObject *parent = nullptr);
    ~CodeBlock();
    size_t addInstruction(TranslationInstruction &instruction);
    void *cache() { return this->_cache; }
    size_t cacheSize() const { return this->_cacheSize; }
    Context &context() { return this->_context; }
    void finalize();
    //TranslationInstruction &operator[](size_t offset);
    void setCache(void *cache, size_t cacheSize);
    size_t size() const { return this->_size; }

private:

    void deadCodeOptimization();
    void livenessOptimization();

    void    *_cache;
    size_t  _cacheSize;
    Context &_context;
    size_t  _size;  
};

#endif