#ifndef TRANSLATION_REGISTER_H
#define TRANSLATION_REGISTER_H

#include <QObject>
#include <QSharedPointer>

class TranslationRegister: public QObject
{
    Q_OBJECT

public:

    TranslationRegister(size_t size, bool temp, QObject *parent = nullptr);
    ~TranslationRegister();
    size_t size() const { return this->_size; }
    bool temp() const { return this->_temp; }

private:

    size_t _size;
    bool   _temp;
};

#endif