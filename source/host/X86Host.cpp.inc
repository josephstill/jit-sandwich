#include "X86Encoder.h"

void Host::cleanUp()
{

}

Endian Host::endianness()
{
    return Endian::LITTLE;
}

void Host::init()
{
    this->_encoder = QSharedPointer<Encoder>(new X86Encoder());
}