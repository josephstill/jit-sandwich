#ifndef CODE_BLOCK_H
#define CODE_BLOCK_H

#include <QObject>
#include <QList>

#include "TranslationInstruction.h"

class CodeBlock: public QObject
{
    Q_OBJECT

public:

    CodeBlock(QObject *parent = nullptr);
    ~CodeBlock();
    void addInstruction(QSharedPointer<TranslationInstruction> instruction);
    void *cache() { return this->_cache; }
    size_t cacheSize() const { return this->_cacheSize; }
    void finalize();
    void setCache(void *cache, size_t cacheSize);
    size_t size() const { return this->_size; }

private:

    void deadCodeOptimization();
    void livenessOptimization();

    void    *_cache;
    size_t  _cacheSize;
    size_t  _size;  

    QList<QSharedPointer<TranslationInstruction>> _instructions;
};

#endif