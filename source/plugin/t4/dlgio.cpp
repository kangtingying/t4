#include "dlgio.h"
#include "ui_dlgio.h"

namespace mrx_t4 {

QString DlgIO::fmtDo( quint32 _do, int w )
{
    QString str;

    //! fmt
    str += "[0]";
    for ( int i = 0; i < w; i++ )
    {
        if ( i != 0 )
        { str += ","; }

        if ( _do & 0x01 )
        { str += "H"; }
        else
        { str += "L"; }

        _do >>= 1;
    }
    str += QString("[%1]").arg( w - 1);

    return str;
}
QString DlgIO::fmtDi( quint32 _di, int w )
{
    return DlgIO::fmtDi( _di, w );
}

DlgIO::DlgIO(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgIO)
{
    ui->setupUi(this);
}

DlgIO::~DlgIO()
{
    delete ui;
}

void DlgIO::setDo( quint32 _do )
{ mDo = _do; }
quint32 DlgIO::getDo()
{ return mDo; }

void DlgIO::setDi( quint32 _di )
{ mDi = _di; }
quint32 DlgIO::getDi()
{ return mDi; }

}
