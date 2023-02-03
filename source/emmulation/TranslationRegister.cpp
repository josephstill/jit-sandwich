#include "TranslationRegister.h"

TranslationRegister::TranslationRegister(size_t size, QObject *parent):
    QObject(parent),
    _size(size)
{

}

TranslationRegister::~TranslationRegister() 
{

}