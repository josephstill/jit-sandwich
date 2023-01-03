#ifndef ENVIRON_H
#define ENVIRON_H

#include <QtGlobal>

#include <cstdint>

enum Endian 
{
    BIG,
    LITTLE
};

#ifdef Q_PROCESSOR_X86_64
#define MAX_HOST_INSTRUCTION_SIZE 16

typedef int HostAddress;
typedef int  HostAddressOffset;

#else
#error "Unknown host processor type"
#endif

#endif