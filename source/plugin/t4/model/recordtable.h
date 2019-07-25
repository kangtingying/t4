#ifndef RECORDTABLE_H
#define RECORDTABLE_H

#include "../../model/megatablemodel.h"
class MDataSection;
namespace mrx_t4{

class RecordItem
{

public:
    QString mType;
    QString mCoord;
    QString mPara;
    quint32 mDi, mDo;
    float mX, mY, mZ, mH, mPw;
    float mVel, mAcc;
    QString mComment;
};


class RecordTable : public MegaTableModel
{
    Q_OBJECT
public:
    RecordTable();

    virtual ~RecordTable();

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
    int exportOut( const QString &fileName );
    int loadIn( const QString &fileName );
protected:
    int _loadIn( MDataSection *pSection, QList< RecordItem *> &items );

protected:
    bool decorationRole( int role, int row, int col, QVariant &var ) const;
    bool displayRole( int role, int row,int col, QVariant &var ) const;

    bool isItemParaAble( int row ) const;

protected:
    virtual int serialOut( QXmlStreamWriter &writer );
    virtual int serialIn( QXmlStreamReader &reader );
    int _serialIn( QXmlStreamReader &reader, RecordItem *pItem );
public:

    void createDebug();

public:
    QList< RecordItem *> mItems;
};

}
#endif // RECORDTABLE_H
