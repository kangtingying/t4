#include "debugitem.h"


#include "../../../include/mystd.h"

const static char *_headers[] =
{
    QT_TR_NOOP( "No."),
    QT_TR_NOOP( "Delay(s)"),
};

int DebugItem::columns()
{
    return sizeof_array( _headers );
}
QString DebugItem::header( int col )
{
    Q_ASSERT( col >= 0 && col <= sizeof_array(_headers) );

//    return qApp->translate("HEADER", _headers[col]);
    return QObject::tr( _headers[col] );
}

DebugItem::DebugItem()
{
    mId = 0;
    mDelay = 1.0;
}
