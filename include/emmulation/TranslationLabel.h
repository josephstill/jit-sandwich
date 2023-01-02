#include <QObject>
#include "guest_defs.h"
#include "Environ.h"

class TranslationLabel: public QObject
{
    Q_OBJECT

public:
    
    TranslationLabel(QObject *parent = nullptr);
    ~TranslationLabel();
    HostAddressOffset offset() const { return this->_offset; }
    void setDest(HostAddress dest);
    void setSource(HostAddress source);
    bool valid() const { return this->_offset != 0; }

private:

    HostAddress       _landing;
    HostAddressOffset _offset;
    HostAddress       _source;
};