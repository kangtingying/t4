#ifndef ISPINDELEGATE_H
#define ISPINDELEGATE_H

#include <QStyledItemDelegate>
#include <QtWidgets>

class iSpinDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    iSpinDelegate(QObject *parent = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override;

    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const override;

    void updateEditorGeometry(QWidget *editor,
        const QStyleOptionViewItem &option, const QModelIndex &index) const override;

public:
    void setMax( int v );
    void setMin( int v );
//    void setDecimal( int d );

protected:
//    int mDecimal;
    int mMax, mMin;
};

#endif // ISPINDELEGATE_H
