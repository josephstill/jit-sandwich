#include <QSysInfo>
#include <stdexcept>

#include "AARCH64Encoder.h"

void Host::cleanUp()
{

}

Endian Host::endianness()
{
    // TODO: determine endianness? Probably little
    return Endian::LITTLE;
}

void Host::init()
{
    if (QSysInfo::currentCpuArchitecture == "arm64")
    {
        this->_encoder = QSharedPointer<Encoder>(new AARCH64Encoder());
    }
    throw std::runtime_error("32 Bit ARM hosts are not supported.\n");
}