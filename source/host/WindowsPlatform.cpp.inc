#include <Windows.h>

void *Platform::allocateCodeBuffer()
{
    return 0;
}

void Platform::freeCodeBuffer(void *buffer)
{
    return;
}

bool Platform::lockCodeBuffer(void *buffer, size_t size)
{
    return true;
}