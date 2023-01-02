#include <stdint>
#include <unistd.h>
#include <sys/mman.h>

void *Platform::allocateCodeBuffer()
{
    void *ret = mmap(0, size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS, -1, 0);
    if (ret == MAP_FAILED)
    {
        ret = nullptr;
    }
    return ret;
}

void Platform::freeCodeBuffer(void *buffer)
{
    munmap(buffer);   
}

bool Platform::lockCodeBuffer(void *buffer, size_t size)
{
    return mprotect(buffer, size, PROT_EXEC | PROT_READ) != -1;
}