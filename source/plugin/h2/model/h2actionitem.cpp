#include "h2actionitem.h"
#include "mystd.h"

const static char *_headers[] =
{
    QT_TRANSLATE_NOOP("HEADER", "Type"),
    QT_TRANSLATE_NOOP("HEADER", "Target X"),
    QT_TRANSLATE_NOOP("HEADER", "Target Y"),
    QT_TRANSLATE_NOOP("HEADER", "Velocity"),
    QT_TRANSLATE_NOOP("HEADER", "Acceleration"),
    QT_TRANSLATE_NOOP("HEADER", "Comment"),
};

int H2ActionItem::columns()
{
    return sizeof_array( _headers );
}
QString H2ActionItem::header( int col )
{
    Q_ASSERT( col >= 0 && col <= sizeof_array(_headers) );

    return qApp->translate("HEADER", _headers[col]);
}

H2ActionItem::H2ActionItem()
{
    mType = "PA";
    mX = 0;
    mY = 0;

    mAcc = 1;
    mVel = 1;
}

