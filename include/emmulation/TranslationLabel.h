#ifndef TRANSLATIONLABEL_H
#define TRANSLATIONLABEL_H

#include <QObject>
#include "guest_defs.h"
#include "Environ.h"

class TranslationLabel: public QObject
{
    Q_OBJECT

public:
    
    TranslationLabel(QObject *parent = nullptr);
    ~TranslationLabel();
};

#endif