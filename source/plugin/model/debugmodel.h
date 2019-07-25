#ifndef DEBUGMODEL_H
#define DEBUGMODEL_H

#include "model.h"
#include "debugitem.h"

class DebugModel : public MegaTableModel
{
public:
    DebugModel();
    ~DebugModel();

public:
    virtual int rowCount(const QModelIndex &parent) const;
    virtual int columnCount(const QModelIndex &parent) const;

    virtual QVariant data(const QModelIndex &index, int role) const;

    virtual bool setData(const QModelIndex &index, const QVariant &value, int role);

    virtual Qt::ItemFlags flags(const QModelIndex &index) const;

    QMimeData *mimeData(const QModelIndexList &indexes) const;
    QStringList mimeTypes() const;
    bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent);

    virtual bool insertRows(int position, int rows, const QModelIndex &parent);
    virtual bool removeRows(int position, int rows, const QModelIndex &parent);

    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

public:
    QList< DebugItem *> *items();

    int save( const QString &fileName );
    int load( const QString &fileName );

    int serialOut( QXmlStreamWriter & writer );
    int serialIn( QXmlStreamReader & reader );

    void signal_dataChanged( const QModelIndex &tl,
                             const QModelIndex &br );

public:
    QList< DebugItem *> mItems;
};

#endif // DEBUGMODEL_H
