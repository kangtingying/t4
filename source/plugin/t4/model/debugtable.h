#ifndef DEBUGTABLE_H
#define DEBUGTABLE_H

#include "../../model/model.h"

namespace mrx_t4{

class DebugItem
{
public:
    float mT;
    int mNumber;
    float mDelay;
public:
    DebugItem()
    {
        mT = 0;
        mDelay = 0;
        mNumber = 0;
    }
};

class DebugTable : public MegaTableModel
{
    Q_OBJECT

public:
    DebugTable( );
    virtual ~DebugTable();

public:
    virtual int rowCount(const QModelIndex &parent) const;
    virtual int columnCount(const QModelIndex &parent) const;

    virtual QVariant data(const QModelIndex &index, int role) const;

    virtual bool setData(const QModelIndex &index, const QVariant &value, int role);
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;

    QMimeData *mimeData(const QModelIndexList &indexes) const;
    QStringList mimeTypes() const;
    bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent);
//    Qt::DropActions supportedDropActions() const;

    virtual bool insertRows(int position, int rows, const QModelIndex &parent);
    virtual bool removeRows(int position, int rows, const QModelIndex &parent);

    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

public:
    void exchange( int row1, int row2 );

protected:
    virtual int serialOut( QXmlStreamWriter &writer );
    virtual int serialIn( QXmlStreamReader &reader );

    int _serialIn( QXmlStreamReader &reader, QList< DebugItem *> &items );

public:
    void createDebug();

public:
    QList< DebugItem *> mItems;
};

}

#endif // DEBUGTABLE_H
