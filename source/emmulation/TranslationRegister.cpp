#include "TranslationRegister.h"

TranslationRegister::TranslationRegister(size_t size, QObject *parent):
    TranslationArgument(parent),
    _size(size)
{

}

TranslationRegister::~TranslationRegister() 
{

}