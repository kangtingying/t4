#include "megaloglistmodel.h"
#include <QIcon>
#include "../../../include/mystd.h"

QString LogStr::toString( LogStr::eLogLevel lev )
{
    if ( lev == LogStr::log_info )
    { return "Info"; }
    else if ( lev == LogStr::log_warning )
    { return "Warn"; }
    else if ( lev == LogStr::log_error )
    { return "Error"; }
    else
    { return ""; }
}

MegaLogListModel::MegaLogListModel(QObject *parent)
    : QAbstractListModel(parent)
{
    mMaxCount = 256;
}
MegaLogListModel::~MegaLogListModel()
{
//    clear();
    delete_all( mItemList );
}

int MegaLogListModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;

    // FIXME: Implement me!
    return mItemList.size();
}

bool MegaLogListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if ( !index.isValid() )
    { return false; }

    if ( role != Qt::EditRole )
    { return false; }

    QVariantList varList = value.toList();
    if ( varList.size() != 2 )
    { return false; }

    mItemList[ index.row() ]->mLevel  = (LogStr::eLogLevel)varList[0].toInt();
    mItemList[ index.row() ]->mStr  = varList[1].toString();
    return true;
}

QVariant MegaLogListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if ( role == Qt::DisplayRole )
    { return QVariant( mItemList.at(index.row())->mStr ); }

    else if ( role == Qt::DecorationRole )
    {
        LogStr::eLogLevel lev = mItemList.at(index.row())->mLevel;

        if ( lev == LogStr::log_info )
        { return QVariant( QIcon(":/res/image/icon/xiaoxi.png") ); }
        else if ( lev == LogStr::log_warning )
        { return QVariant( QIcon(":/res/image/icon/gantanhaozhong.png") ); }
        else if ( lev == LogStr::log_error )
        { return QVariant( QIcon(":/res/image/icon/shibai2.png") ); }
        else
        { return QVariant( QIcon(":/res/image/icon/xiaoxi.png") ); }
    }
    else
    { return QVariant(); }
}

bool MegaLogListModel::insertRows(int row, int count, const QModelIndex &parent)
{
    LogStr * pItem = new LogStr();
    if ( NULL == pItem )
    { return false ; }

    beginInsertRows( parent, row, row + count - 1 );
        mItemList.append( pItem );
    endInsertRows();

    return true;
}

bool MegaLogListModel::removeRows(int row, int count, const QModelIndex &parent)
{
    if ( row < 0 || count < 1 )
    { return false; }

    beginRemoveRows(QModelIndex(), row, row+count-1);

    for ( int i = 0; i < count; i++ )
    {
        delete mItemList[ row ];
//        logDbg()<<mItemList.size();
        mItemList.removeAt( row );
//        logDbg()<<mItemList.size();
    }

    endRemoveRows();
    return true;
}

void MegaLogListModel::append( const QString &content,
                               LogStr::eLogLevel logLev )
{
//    beginInsertRows(  createIndex( mItemList.size(), 0),
//                      mItemList.size() - 1, mItemList.size()
//                      );

//    beginResetModel();
//    return;
    LogStr *pItem;

    //! overlap
    if ( mItemList.size() >= mMaxCount )
    {
        removeRow( 0 );

//        beginResetModel();

//            pItem = mItemList.takeAt( 0 );
//            pItem->mLevel = logLev;
//            pItem->mStr = content;
//            mItemList.append( pItem );

//            emit dataChanged( index( 0,0), index( mItemList.size()-1,0 ) );
//        endResetModel();

//        return;
//        beginInsertRows(QModelIndex(), mItemList.size(), mItemList.size() );
    }
//    else if ( mItemList.size() < 1 )
//    {
//        pItem = new LogStr();
//        if ( NULL == pItem )
//        { return; }

//        beginInsertRows(QModelIndex(), 0, 0 );
//    }
//    else
    {
//        pItem = new LogStr();
//        if ( NULL == pItem )
//        { return; }

        if ( insertRow( mItemList.size() ) )
        {}
        else
        { return; }
//        beginInsertRows(QModelIndex(), mItemList.size(), mItemList.size() );
    }

    //! set content
//    pItem->mLevel = logLev;
//    pItem->mStr = content;

//    mItemList[ mItemList.size()-1]->mLevel = logLev;
//    mItemList[ mItemList.size()-1]->mStr = content;

//    data( index( mItemList.size() -1, 0),)
//    setData( index( mItemList.size() -1, 0) , logLev );

    QVariantList varList;
    varList<<(int)logLev<<content;
    setData( index( mItemList.size() - 1, 0), varList );

    //! last
    emit signal_current_changed( mItemList.size() - 1 );

    //! append
//    mItemList.append( pItem );

//    endInsertRows();
}
void MegaLogListModel::clear()
{
    beginResetModel();
    delete_all( mItemList );
    endResetModel();
}
