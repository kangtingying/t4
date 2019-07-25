#ifndef DBLSPINDELEGATE_H
#define DBLSPINDELEGATE_H

#include <QStyledItemDelegate>

class DoubleSpinBoxDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    DoubleSpinBoxDelegate( QObject *parent = 0);
    DoubleSpinBoxDelegate( int decimals = 2, double min = 0, double max = 100, QObject *parent = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override;

    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const override;

    void updateEditorGeometry(QWidget *editor,
        const QStyleOptionViewItem &option, const QModelIndex &index) const override;

public slots:
    void slotSetValueRange(int min, int max);

private:
    int m_decimals;
    double m_min;
    double m_max;
};

#endif // DBLSPINDELEGATE_H
