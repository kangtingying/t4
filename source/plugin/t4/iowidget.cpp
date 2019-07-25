#include "iowidget.h"
#include "ui_iowidget.h"

#define cat_widgets( wig )      mBits.append( wig ); mSpanWidgets.append( wig );

IOWidget::IOWidget(int w, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::IOWidget)
{
    ui->setupUi(this);

    mWidth = w;

    //! adapt to the width
    cat_widgets( ui->widget );
    cat_widgets( ui->widget_2 );
    cat_widgets( ui->widget_3 );
    cat_widgets( ui->widget_4 ); mSpanWidgets.append( ui->label );

    cat_widgets( ui->widget_5 );
    cat_widgets( ui->widget_6 );
    cat_widgets( ui->widget_7 );
    cat_widgets( ui->widget_8 );

    //! adapt the widgets
    int i = 0;
    for ( i = 0; i < w; i++ )
    {
        mBits[i]->setVisible( true );
        mBits[i]->setName(QString("Y%1").arg(i+1));
    }

    //! find the w
    int last;
    last = mSpanWidgets.indexOf( mBits.at(w-1) );
    for ( last = last + 1 ; last < mSpanWidgets.size(); last++ )
    {
        mSpanWidgets[ last ]->setVisible( false );
    }
}

IOWidget::~IOWidget()
{
    delete ui;
}

void IOWidget::setData( quint32 dat )
{
    for ( int i = 0; i < mWidth; i++ )
    {
        //mBits.at(i)->setCheckState( Qt::CheckState( dat & 0x03) );
        quint32 t = dat & 0x03;
        if(t == 0x03){
            mBits.at(i)->setValue( HLX::e_hlx_h );
        }
        if(t == 0x00){
            mBits.at(i)->setValue( HLX::e_hlx_x );
        }
        if(t == 0x02){
            mBits.at(i)->setValue( HLX::e_hlx_l );
        }

        dat >>= 2;
    }
}

quint32 IOWidget::getData( )
{
    quint32 dat = 0;
    HLX::hlxValue iVal;
    for ( int i = 0; i < mWidth; i++ )
    {
        iVal = mBits.at(i)->value();
        switch ( iVal )
        {
            case HLX::e_hlx_l:
                dat |= 0x02 << (i*2);
                continue;
            case HLX::e_hlx_x:
                dat |= 0x00 << (i*2);
                continue;
            case HLX::e_hlx_h:
                dat |= 0x03 << (i*2);
                continue;
            default:
                break;
        }
    }

    return dat;
}
