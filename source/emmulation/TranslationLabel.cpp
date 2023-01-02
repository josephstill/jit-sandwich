#include "TranslationLabel.h"

TranslationLabel::TranslationLabel(QObject *parent):
    QObject(parent),
    _landing(0),
    _offset(0),
    _source(0)
{

}

TranslationLabel::~TranslationLabel() 
{

}

void TranslationLabel::setDest(HostAddress dest) 
{
    if (this->_landing != dest)
    {
        if (this->_offset != 0)
        {
            this->_offset = 0;
        }

        this->_landing = dest;

        if (this->_source > 0)
        {
            this->_offset = this->_landing - this->_source;
        }
    }
}

void TranslationLabel::setSource(HostAddress source) 
{
    if (this->_source != source)
    {
        if (this->_offset != 0)
        {
            this->_offset = 0;
        }     

        this->_source = source;

        if (this->_landing > 0)
        {
            this->_offset = this->_landing - this->_source;
        }        
    }
}