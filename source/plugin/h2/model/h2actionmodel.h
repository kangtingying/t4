#ifndef H2ACTIONMODEL_H
#define H2ACTIONMODEL_H

#include "megatablemodel.h"
#include "h2actionitem.h"
class H2ActionModel : public MegaTableModel
{
    Q_OBJECT
public:
    H2ActionModel();
    ~H2ActionModel();

public:
    virtual int rowCount(const QModelIndex &parent) const;
    virtual int columnCount(const QModelIndex &parent) const;

    virtual QVariant data(const QModelIndex &index, int role) const;

    virtual bool setData(const QModelIndex &index, const QVariant &value, int role);
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;

    virtual bool insertRows(int position, int rows, const QModelIndex &parent);
    virtual bool removeRows(int position, int rows, const QModelIndex &parent);

    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

public:
    QList< H2ActionItem *> *items();

    int save( const QString &fileName );
    int load( const QString &fileName );

    int serialOut( QXmlStreamWriter & writer );
    int serialIn( QXmlStreamReader & reader );

    int output( const QString &fileName );
    int input( const QString &fileName );

public:
    QList< H2ActionItem *> mItems;
};

#endif // H2ACTIONMODEL_H
