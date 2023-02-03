#ifndef TRANSLATIONARGUMENT_H
#define TRANSLATIONARGUMENT_H

#include <QObject>

class TranslationArgument: public QObject 
{
    Q_OBJECT

public:

    TranslationArgument(uint64_t value, QObject *parent = nullptr);
    ~TranslationArgument();
    virtual uint64_t value() const { return this->_value; }
    
protected:

    TranslationArgument(QObject *parent = nullptr);    
     
private:

    uint64_t _value;
};

#endif