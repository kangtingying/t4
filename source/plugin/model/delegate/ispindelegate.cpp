#include "ispindelegate.h"

iSpinDelegate::iSpinDelegate( QObject *parent ) : QStyledItemDelegate(parent)
{
    mMax = INT_MAX;
    mMin = INT_MIN;
}

QWidget *iSpinDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                      const QModelIndex &index) const
{
    QSpinBox *pSpinBox = new QSpinBox( parent );
    Q_ASSERT( NULL != pSpinBox );

    pSpinBox->setFrame( false );
    pSpinBox->setMaximum( mMax );
    pSpinBox->setMinimum( mMin );

    return pSpinBox;
}

void iSpinDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if ( index.isValid() )
    {}
    else
    { return; }

    double value = index.model()->data(index, Qt::EditRole).toDouble();

    QDoubleSpinBox *pSpin = static_cast<QDoubleSpinBox*>(editor);

    pSpin->setValue( value );
}

void iSpinDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                  const QModelIndex &index) const
{
    QDoubleSpinBox *pCon = static_cast<QDoubleSpinBox*>(editor);

    model->setData(index, pCon->value(), Qt::EditRole);
}

void iSpinDelegate::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}

void iSpinDelegate::setMax( int v )
{ mMax = v; }
void iSpinDelegate::setMin( int v )
{ mMin = v; }

