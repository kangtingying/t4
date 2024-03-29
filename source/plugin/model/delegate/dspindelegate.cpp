#include "dspindelegate.h"

dSpinDelegate::dSpinDelegate( QObject *parent ) : QStyledItemDelegate(parent)
{
    mDecimal = 2;
    mMax = INT_MAX;
    mMin = INT_MIN;

    mPrefix = "";
    mSuffix = "";
}

dSpinDelegate::dSpinDelegate(double mi, double ma, QObject *parent ): QStyledItemDelegate(parent)
{
    mDecimal = 2;
    mMin = mi;
    mMax = ma;

    mPrefix = "";
    mSuffix = "";
}

QWidget *dSpinDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                      const QModelIndex &index) const
{
    QDoubleSpinBox *pSpinBox = new QDoubleSpinBox( parent );
    Q_ASSERT( NULL != pSpinBox );

    pSpinBox->setFrame( false );
    pSpinBox->setDecimals( mDecimal );
    pSpinBox->setMaximum( mMax );
    pSpinBox->setMinimum( mMin );

    pSpinBox->setPrefix( mPrefix );
    pSpinBox->setSuffix( mSuffix );

    //! \note no button style
    pSpinBox->setButtonSymbols( QAbstractSpinBox::NoButtons );

    return pSpinBox;
}

void dSpinDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if ( index.isValid() )
    {}
    else
    { return; }

    double value = index.model()->data(index, Qt::EditRole).toDouble();

    QDoubleSpinBox *pSpin = static_cast<QDoubleSpinBox*>(editor);

    pSpin->setValue( value );
}

void dSpinDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                  const QModelIndex &index) const
{
    QDoubleSpinBox *pCon = static_cast<QDoubleSpinBox*>(editor);

    model->setData(index, pCon->value(), Qt::EditRole);
}

void dSpinDelegate::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}

void dSpinDelegate::setMax( double v )
{ mMax = v; }
void dSpinDelegate::setMin( double v )
{ mMin = v; }
void dSpinDelegate::setDecimal( int d )
{ mDecimal = d; }

void dSpinDelegate::sePrefix( const QString &pre )
{ mPrefix = pre; }
void dSpinDelegate::setSuffix( const QString &sur )
{ mSuffix = sur; }

