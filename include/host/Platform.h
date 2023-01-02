#ifndef PLATFORM_H
#define PLATFORM_H

#include <QObject>

class Platform: public QObject
{
    Q_OBJECT

public:

    Platform(QObject *parent = nullptr);
    ~Platform();
    void *allocateCodeBuffer();
    void freeCodeBuffer(void *buffer);
    bool lockCodeBuffer(void *buffer, size_t size);    

private:

    void cleanUp();
    void init();
};

#endif