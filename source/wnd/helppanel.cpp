#include "helppanel.h"
#include "ui_helppanel.h"
#include <QFile>

#include "mystd.h"

HelpPanel::HelpPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HelpPanel)
{
    ui->setupUi(this);

}

HelpPanel::~HelpPanel()
{
    delete ui;
}

void HelpPanel::setFile( const QString &path )
{
    if ( mPath != path )
    {
        mPath = path;

        slot_file_changed();
    }
}
QString HelpPanel::file( )
{ return mPath; }

void HelpPanel::slot_file_changed()
{
    QVariant var;
    var = ui->textEdit->loadResource( QTextDocument::HtmlResource,
                                QUrl::fromLocalFile(mPath) );

    QFile file( mPath );
    QByteArray ary;
    if ( !file.open( QIODevice::ReadOnly) )
    { return; }

    ary = file.readAll();
    file.close();
    if ( ary.length() > 0 )
    {
        ui->textEdit->setHtml( ary );
    }

    logDbg()<<mPath;
}
