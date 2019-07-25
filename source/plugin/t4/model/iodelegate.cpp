#include "iodelegate.h"

#include "../iowidget.h"

QString IODelegate::toString( quint32 _do, int w )
{
    QString str;

    //! fmt
    str += "[0]";
    for ( int i = 0; i < w; i++ )
    {
        if ( i != 0 )
        { str += ","; }

        if ( (_do & 0x3)==0x00 )
        { str += "X"; }
        else if ( (_do & 0x3)==0x3 )
        { str += "H"; }
        else if ( (_do & 0x3)==0x2 )
        { str += "L"; }
        else
        {}

        _do >>= 2;
    }
    str += QString("[%1]").arg( w - 1);

    return str;
}

IODelegate::IODelegate( int w, QObject *parent ) : QStyledItemDelegate(parent)
{
    mWidth = w;
}

QWidget *IODelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                      const QModelIndex &index) const
{
    IOWidget *pWig = new IOWidget( mWidth, parent );
    Q_ASSERT( NULL != pWig );

//    pWig->setWindowFlag( Qt::Popup, true );

    return pWig;
}

void IODelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if ( index.isValid() )
    {}
    else
    { return; }

    quint32 value = index.model()->data(index, Qt::EditRole).toUInt();

    IOWidget *pWig = static_cast<IOWidget*>(editor);

    pWig->setData( value );
}

void IODelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                  const QModelIndex &index) const
{
    IOWidget *pWig = static_cast<IOWidget*>(editor);
    quint32 value = pWig->getData();

    model->setData(index, value, Qt::EditRole);
}

void IODelegate::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QRect selfRect;
    selfRect = editor->geometry();

    editor->setGeometry( option.rect.left(), option.rect.top(),
                         selfRect.width(), selfRect.height() );
//     editor->setGeometry( option.rect );
}

