#include "Platform.h"

Platform::Platform(QObject *parent):
    QObject(parent)
{
    this->init();
}

Platform::~Platform()
{
    this->cleanUp();
}

#ifdef Q_OS_LINUX
#include "LinuxPlatform.cpp.inc"
#elif defined(Q_OS_WINDOWS)
#include "WindowsPlatform.cpp.inc"
#else
#error "Undefined target platform"
#endif