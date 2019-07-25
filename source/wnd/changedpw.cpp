#include "changedpw.h"
#include "ui_changedpw.h"
#include <QPushButton>
ChangedPw::ChangedPw(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChangedPw)
{
    ui->setupUi(this);

    connect( ui->edtLast, SIGNAL(textChanged( const QString &)),
             this, SLOT(slot_updateControl()));
    connect( ui->edtPw1, SIGNAL(textChanged( const QString &)),
             this, SLOT(slot_updateControl()));
    connect( ui->edtPw2, SIGNAL(textChanged( const QString &)),
             this, SLOT(slot_updateControl()));

    //! update control
    slot_updateControl();
}

ChangedPw::~ChangedPw()
{
    delete ui;
}

QString ChangedPw::getOldPw()
{
    return ui->edtLast->text();
}
QString ChangedPw::getNewPw()
{
    return ui->edtPw1->text();
}

void ChangedPw::slot_updateControl()
{
    ui->buttonBox->button( QDialogButtonBox::Ok )->setEnabled( false );

    //! check the last
    if ( ui->edtLast->text().trimmed().length() < 1 )
    {
        return;
    }

    //! check equal
    if ( ui->edtPw1->text().trimmed().length() < 1 )
    {
        return;
    }

    if ( ui->edtPw1->text() != ui->edtPw2->text() )
    {
        return;
    }

    ui->buttonBox->button( QDialogButtonBox::Ok )->setEnabled( true );
}


