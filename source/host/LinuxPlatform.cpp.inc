#include <cstdint>
#include <unistd.h>
#include <sys/mman.h>

void *Platform::allocateCodeBuffer(size_t size)
{
    void *ret = mmap(0, size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS, -1, 0);
    if (ret == MAP_FAILED)
    {
        ret = nullptr;
    }
    return ret;
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
    munmap(buffer, size);   
}

bool Platform::lockCodeBuffer(void *buffer, size_t size)
{
    return mprotect(buffer, size, PROT_EXEC | PROT_READ) != -1;
}