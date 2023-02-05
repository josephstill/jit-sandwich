#ifndef TRANSLATION_REGISTER_H
#define TRANSLATION_REGISTER_H

#include <QObject>
#include <QSharedPointer>

#include "TranslationArgument.h"

class TranslationRegister: public TranslationArgument
{
    Q_OBJECT

public:

    TranslationRegister(size_t size, QObject *parent = nullptr);
    ~TranslationRegister();
    size_t size() const { return this->_size; }

private:

    size_t _size;
};

#endif