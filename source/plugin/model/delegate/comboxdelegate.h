#ifndef COMBOXDELEGATE_H
#define COMBOXDELEGATE_H
#include <QStyledItemDelegate>
#include <QMap>

class ComboxDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    ComboxDelegate(QObject *parent = 0);
    ComboxDelegate(int type = 0, QObject *parent = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override;

    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const override;

    void updateEditorGeometry(QWidget *editor,
        const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    void setItems( const QStringList &strList,
                   const QList<int> &vals );
    void setItems( const QStringList &strList,
                   const int base = 0 );

    int value( const QString &name, int *pVal );
    QString toString( int id );


protected:
    QMap<QString, int> mTables;
    QStringList mItems;

    int m_type;
};

#endif // COMBOXDELEGATE_H
