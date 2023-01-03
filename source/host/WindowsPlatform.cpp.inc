#include <Windows.h>

void *Platform::allocateCodeBuffer(size_t size)
{
    return 0;
}

void Platform::cleanUp() 
{
    return;
}

void Platform::init() 
{
    return;
}

void Platform::freeCodeBuffer(void *buffer, size_t size)
{
    return;
}

bool Platform::lockCodeBuffer(void *buffer, size_t size)
{
    return true;
}