#ifndef MEMORY_PHYSICAL_MEMORY_H
#define MEMORY_PHYSICAL_MEMORY_H

#include <QObject>
#include <QFile>
#include <QString>

#include "guest_defs.h"
#include "Memory.h"


class PhysicalMemory: public Memory
{
    Q_OBJECT

public:
 
    PhysicalMemory(size_t size, QObject *parent = nullptr);
    PhysicalMemory(QString &filePath, bool read, bool write, QObject *parent = nullptr);
    ~PhysicalMemory();
    virtual void init();
    virtual bool isExecute() const {return this->execute;}
    virtual bool isRead() const {return this->read;}
    virtual bool isWrite() const {return this->write;}
    virtual bool performRead(GuestAddress offset, uint8_t *data, size_t size) const;
    virtual bool performWrite(GuestAddress offset, const uint8_t *data, size_t size);
    virtual void reset();
    virtual size_t size() const {return this->_size;}
    virtual uint8_t *toBuffer(GuestAddress offset, size_t size);

protected:

    void setExecute(bool enable) {this->execute = enable;}
    void setRead(bool enable) {this->read = enable;}
    void setWrite(bool enable) {this->write = enable && !this->mediumReadOnly;}

private:

    QFile   *backingFile;
    uint8_t *buffer;
    bool    execute;
    bool    mediumReadOnly;
    bool    read;
    size_t  _size;
    bool    write;

};

#endif