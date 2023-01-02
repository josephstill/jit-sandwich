#include <QObject>

#include "Encoder.h"


class X86Encoder: public Encoder
{
    Q_OBJECT

public:

    X86Encoder(QObject *parent = nullptr);
    ~X86Encoder();
    virtual size_t preamble(void *buffer);
    virtual size_t postamble(void *buffer);
};