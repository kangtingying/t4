#include "prompt.h"
#include "ui_prompt.h"

Prompt::Prompt(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Prompt)
{
    ui->setupUi(this);
}

Prompt::~Prompt()
{
    delete ui;
}

void Prompt::addInfo( const QString &info, int lev )
{
    QListWidgetItem *pItem;

    if ( ui->listWidget->count() > 64 )
    {
        pItem = ui->listWidget->takeItem( 0 );
    }
    else
    {
        pItem = new QListWidgetItem();
        if ( NULL == pItem )
        { return; }
    }
    if ( lev == 0 )
    { pItem->setIcon( QIcon(":/res/image/icon/xiaoxi.png") );}
    else if ( lev == 1 )
    { pItem->setIcon( QIcon(":/res/image/icon/gantanhaozhong.png") ); }
    else if ( lev == 2 )
    { pItem->setIcon( QIcon(":/res/image/icon/shibai2.png") ); }
    else
    {}

    pItem->setText( info );
    ui->listWidget->addItem( pItem );
    ui->listWidget->scrollToBottom();
}

void Prompt::hideEvent(QHideEvent *event)
{
    ui->listWidget->clear();
    QDialog::hideEvent( event );
}
