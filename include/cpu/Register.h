#ifndef CPU_REGISTER_H
#define CPU_REGISTER_H

#include <QObject>
#include <QSharedPointer>
#include <QList>

class Register: public QObject
{
    Q_OBJECT

public:

    Register(size_t size, QObject *parent = nullptr);
    ~Register();
    virtual QSharedPointer<Register> alias();
    virtual QSharedPointer<Register> alias(size_t size, uint8_t offset);
    virtual void init();
    virtual uint8_t *buffer() {return this->data; } 
    virtual void read(uint8_t *buffer, size_t size);
    virtual void reset();
    size_t size() const { return this->_size; }
    virtual void write(uint8_t *buffer, size_t size);
    
private:

    Register(uint8_t *data, size_t size, Register *source, QObject *parent = nullptr);
    void detatch(uint8_t *data);
    void takeAlias(QSharedPointer<Register> alias);
    void takeSource(Register *source) { this->source = source; }

    uint8_t                          *data;
    size_t                           _size;
    Register                         *source;
    QList<QSharedPointer<Register>>  aliases;
 
};

#endif