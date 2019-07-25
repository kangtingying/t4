#include "errmgritem.h"
#include "mystd.h"

const static char *_headers[] =
{
    QT_TRANSLATE_NOOP("HEADER", "No."),
    QT_TRANSLATE_NOOP("HEADER", "Error Text"),
    QT_TRANSLATE_NOOP("HEADER", "Error"),
    QT_TRANSLATE_NOOP("HEADER", "Warning"),
    QT_TRANSLATE_NOOP("HEADER", "Information"),
    QT_TRANSLATE_NOOP("HEADER", "Reaction on error"),
    QT_TRANSLATE_NOOP("HEADER", "Output stage on"),
    QT_TRANSLATE_NOOP("HEADER", "Save diagnosis"),
};

int ErrMgrItem::columns()
{
    return sizeof_array( _headers );
}
QString ErrMgrItem::header( int col )
{
    Q_ASSERT( col >= 0 && col <= sizeof_array(_headers) );

    return qApp->translate("HEADER", _headers[col]);
}

ErrMgrItem::ErrMgrItem()
{
    mNr = 0;
    mEventType = e_error;
    mAction = "";
    mbErrorAble = true;
    mbWarnAble = true;
    mbInfoAble = true;

    mbOutput = false;
    mbOutputAble = false;

    mbSaveDiagnosis = false;
    mbSaveAble = true;

    mbActionAble = true;
}

eColumnAttr ErrMgrItem::columnAttr( int col )
{
    return column_unk;
}

void ErrMgrItem::setEventType( e_event_type evt, bool b )
{
    //! \todo according the event attr
    if ( evt == e_error )
    {
        if ( b )
        { mEventType = e_error; }
        else
        { mEventType = e_warning; }
    }
    else if ( evt == e_warning )
    {
        if ( b )
        { mEventType = e_warning; }
        else
        { mEventType = e_info; }
    }
    else if ( evt == e_info )
    {
        if ( b )
        { mEventType = e_info; }
        else
        { mEventType = e_error; }
    }
    else
    { Q_ASSERT(false); }
}
