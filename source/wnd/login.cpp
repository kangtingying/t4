#include "login.h"
#include "ui_login.h"
#include <QPushButton>
#include "../include/mydebug.h"
LogIn::LogIn(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LogIn)
{
    ui->setupUi(this);

    setWindowFlags( (Qt::WindowFlags)(~Qt::WindowCloseButtonHint) & windowFlags() );

    //! post setting
//    on_comboBox_currentIndexChanged( ui->comboBox->currentIndex() );
}

LogIn::~LogIn()
{
    delete ui;
}

QString LogIn::getPw()
{ return ui->edtPw->text(); }

int LogIn::getUserRole()
{
    return ui->comboBox->currentIndex();
}

void LogIn::on_edtPw_textChanged(const QString &arg1)
{
    ui->buttonBox->button( QDialogButtonBox::Ok )->setEnabled( arg1.length() > 0 );
}
