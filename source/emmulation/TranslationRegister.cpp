#include "TranslationRegister.h"

TranslationRegister::TranslationRegister(size_t size, bool temp, QObject *parent):
    QObject(parent),
    _size(size),
    _temp(temp) 
{

}

TranslationRegister::~TranslationRegister() 
{

}