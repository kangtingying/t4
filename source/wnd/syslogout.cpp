#include "syslogout.h"
#include "ui_syslogout.h"

#include "../../include/mydebug.h"
SysLogout::SysLogout(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SysLogout)
{
    ui->setupUi(this);

    setFocusPolicy( Qt::WheelFocus );
}

SysLogout::~SysLogout()
{
    delete ui;
}

void SysLogout::attachLogModel( QAbstractListModel *pModel )
{
    Q_ASSERT( NULL != pModel );

    ui->listView->setModel( pModel );

}

bool SysLogout::event(QEvent *event)
{
    if ( event->type() == QEvent::FocusIn )
    {logDbg();
        focusInEvent( (QFocusEvent*)event );
    }
    else
    {}

    return QWidget::event( event );
}

void SysLogout::focusInEvent(QFocusEvent *event)
{
    emit signal_focus_in( "sys", "logout" );

    QWidget::focusInEvent( event );

    logDbg();
}
void SysLogout::reTranslate()
{
    ui->tabWidget->setTabText(0, tr("Logout"));
}

