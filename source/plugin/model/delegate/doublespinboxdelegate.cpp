#include "doublespinboxdelegate.h"
#include <QDoubleSpinBox>

DoubleSpinBoxDelegate::DoubleSpinBoxDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
    m_decimals = 2;
    m_min = 0;
    m_max = 100;
}

DoubleSpinBoxDelegate::DoubleSpinBoxDelegate(int decimals, double min, double max, QObject *parent)
    : QStyledItemDelegate(parent)
{
    m_decimals = decimals;
    m_min = min;
    m_max = max;
}

QWidget *DoubleSpinBoxDelegate::createEditor(QWidget *parent,
                                       const QStyleOptionViewItem &/* option */,
                                       const QModelIndex &/* index */) const
{
    QDoubleSpinBox *editor = new QDoubleSpinBox(parent);
    editor->setFrame(false);

    editor->setDecimals(m_decimals);
    editor->setMinimum(m_min);
    editor->setMaximum(m_max);

    editor->setToolTip(QString("%1 <-> %2").arg(m_min).arg(m_max));

    return editor;
}

void DoubleSpinBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    double value = index.model()->data(index, Qt::EditRole).toDouble();

    QDoubleSpinBox *spinBox = static_cast<QDoubleSpinBox*>(editor);
    spinBox->setValue(value);
}

void DoubleSpinBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                   const QModelIndex &index) const
{
    QDoubleSpinBox *spinBox = static_cast<QDoubleSpinBox*>(editor);
    model->setData(index, spinBox->value(), Qt::EditRole);
}

void DoubleSpinBoxDelegate::updateEditorGeometry(QWidget *editor,
                                           const QStyleOptionViewItem &option,
                                           const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}

void DoubleSpinBoxDelegate::slotSetValueRange(int min, int max)
{
    this->m_max = max;
    this->m_min = min;
}

//void dblSpinDelegate::setRange(double min, double max)
//{
//    m_editor->setMinimum(min);
//    m_editor->setMaximum(max);
//}

//void dblSpinDelegate::setDecimals(int val)
//{
//    m_editor->setDecimals(val);
//}
