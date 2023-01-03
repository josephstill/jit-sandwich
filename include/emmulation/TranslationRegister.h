#ifndef TRANSLATION_REGISTER_H
#define TRANSLATION_REGISTER_H

#include <QObject>
#include <QSharedPointer>

class TranslationRegister: public QObject
{
    Q_OBJECT

public:

    TranslationRegister(QObject *parent = nullptr);
    ~TranslationRegister();
};

#endif