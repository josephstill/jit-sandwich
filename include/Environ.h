#include <QtGlobal>

#ifdef Q_PROCESSOR_X86_64
#define MAX_HOST_INSTRUCTION_SIZE 16

typedef uint64_t HostAddress;
typedef int32_t  HostAddressOffset;

#else
#error "Unknown host processor type"
#endif