#include "comboxdelegate.h"
#include <QtWidgets>

ComboxDelegate::ComboxDelegate( QObject *parent ) : QStyledItemDelegate(parent)
{
    m_type = 0;
}

ComboxDelegate::ComboxDelegate(int type, QObject *parent)
{
    m_type = type;
}

QWidget *ComboxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                      const QModelIndex &index) const
{
    QComboBox *pCombox = new QComboBox( parent );
    Q_ASSERT( NULL != pCombox );

    pCombox->setFrame( false );
    pCombox->setInsertPolicy( QComboBox::NoInsert );
    pCombox->setEditable( false );

    if(m_type == 0)
    {
        pCombox->addItems( mItems );
    }
    else
    {
        QStringList itemData = index.model()->data(index, Qt::UserRole+1).toStringList();
        pCombox->addItems( itemData );
    }


    return pCombox;
}

void ComboxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if ( index.isValid() )
    {}
    else
    { return; }

    QString value = index.model()->data(index, Qt::EditRole).toString();

    QComboBox *pCombox = static_cast<QComboBox*>(editor);

    //! not empty
    if ( value.length() > 0 )
    {}
    else
    {
        pCombox->setCurrentIndex( 0 );
        return;
    }

    if ( mTables.find(value) != mTables.end() )
    {
        pCombox->setCurrentText( value );
    }
    else
    {
        pCombox->setCurrentIndex( 0 );
    }
}

void ComboxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                  const QModelIndex &index) const
{
    QComboBox *pCombox = static_cast<QComboBox*>(editor);
    QString value = pCombox->currentText();

    model->setData(index, value, Qt::EditRole);
}

void ComboxDelegate::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}

void ComboxDelegate::setItems( const QStringList &strList,
               const QList<int> &vals )
{
    Q_ASSERT( vals.size() == strList.size() );
    mTables.clear();
    for( int i = 0; i < strList.size(); i++ )
    {
        mTables.insert( strList[i], vals[i] );
    }

    mItems.clear();
    mItems = strList;
}
void ComboxDelegate::setItems( const QStringList &strList,
               const int base )
{
    mTables.clear();
    for( int i = 0; i < strList.size(); i++ )
    {
        mTables.insert( strList[i], base + i );
    }

    mItems.clear();
    mItems = strList;
}

int ComboxDelegate::value( const QString &name,
                           int *pVal )
{
    foreach( QString str, mTables.keys() )
    {
        if ( name.compare( str, Qt::CaseInsensitive) == 0 )
        {
            *pVal = mTables[str];
            return 0;
        }
    }

    return 0;
}

QString ComboxDelegate::toString( int id )
{
    QMapIterator< QString, int > iter( mTables );

    while (iter.hasNext())
    {
          iter.next();

          if ( iter.value() == id )
          { return iter.key(); }
    }

    return mTables.firstKey();
}

