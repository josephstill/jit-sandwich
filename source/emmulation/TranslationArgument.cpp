#include "TranslationArgument.h"

TranslationArgument::TranslationArgument(uint64_t value, QObject *parent) :
    QObject(parent),
    _value(value)
{

}

TranslationArgument::TranslationArgument(QObject *parent) :
    QObject(parent),
    _value(0)
{

}   

TranslationArgument::~TranslationArgument() 
{

}
    

