#ifndef CPU_REGISTERFILE_H
#define CPU_REGISTERFILE_H

#include <QObject>
#include <QMap>
#include <QString>
#include <QSharedPointer>

#include "Register.h"

class RegisterFile: public QObject
{
    Q_OBJECT

public:

    RegisterFile(QObject *parent = nullptr);
    ~RegisterFile();
    QSharedPointer<Register> addRegister(QString name, size_t size);
    QSharedPointer<Register> addRegister(QString name, QString aliasOf, size_t size, size_t offset);
    QSharedPointer<Register> getRegister(QString name);
    void init();
    void reset();

private:

    QMap<QString, QSharedPointer<Register>> registers;

};

#endif