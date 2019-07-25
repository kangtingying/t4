#include "debugtable.h"
#include <QMimeData>
#include "../../include/mydebug.h"
namespace mrx_t4{

DebugTable::DebugTable( ): MegaTableModel( )
{
    mHeaderList
               <<QObject::tr("No.")
               <<QObject::tr("Delay");
}

DebugTable::~DebugTable()
{
//    removeRows( 0, mItems.size(), QModelIndex() );
    qDeleteAll( mItems );
}

int DebugTable::rowCount(const QModelIndex &parent) const
{ return mItems.size(); }
int DebugTable::columnCount(const QModelIndex &parent) const
{ return mHeaderList.size(); }

QVariant DebugTable::data(const QModelIndex &index, int role) const
{
    if ( !index.isValid() )
    { return QVariant(); }

    int col = index.column();
    int row = index.row();

    if ( role != Qt::DisplayRole && role != Qt::EditRole )
    { return QVariant(); }

    if ( col == 0 )
    { return QVariant( mItems[row]->mNumber ); }
    if ( col == 1 )
    { return QVariant( mItems[row]->mDelay ); }

    return QVariant();
}

bool DebugTable::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if ( index.isValid() && role == Qt::EditRole )
    {}
    else
    { return false; }

    int col = index.column();
    int row = index.row();

    if ( index.column() == 0 )
    { mItems[ row ]->mNumber = value.toInt(); }
    else if ( index.column() == 1 )
    { mItems[ row ]->mDelay = value.toDouble(); }

    else
    {}

    emit dataChanged(index, index);

    return true;
}
Qt::ItemFlags DebugTable::flags(const QModelIndex &index) const
{
    if (!index.isValid())
    { return Qt::ItemIsEnabled; }

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;
}

QMimeData *DebugTable::mimeData(const QModelIndexList &indexes) const
{
    QMimeData *pMime;

    pMime = new QMimeData();
    if ( NULL == pMime )
    { return NULL; }

    QByteArray ary;

    ary.setNum( indexes.at(0).row() );

    pMime->setData( "debug/row", ary );

    return pMime;
}

QStringList DebugTable::mimeTypes() const
{
    QStringList strList;

    strList<<"debug/row";

    return strList;
}

bool DebugTable::dropMimeData(const QMimeData *data,
                              Qt::DropAction action,
                              int row,
                              int column,
                              const QModelIndex &parent )
{logDbg();
    if ( data->hasFormat("debug/row") )
    {}
    else
    { return false; }

    //! exchange the row
    QByteArray ary;
    ary = data->data( "debug/row" );
    int srcRow = ary.toInt();

    logDbg()<<srcRow<<row<<column<<action;
    if ( parent.isValid() )
    { row = parent.row(); }
    else
    { return false; }

    //! get src
    DebugItem proxyItem = *mItems.at( srcRow );

    removeRow( srcRow );

    insertRow( row );

    //! set data
    *mItems.at( row ) = proxyItem;

    emit signal_data_changed();

    emit signal_current_changed( row );

    return true;
}

//Qt::DropActions DebugTable::supportedDropActions() const
//{
//    return Qt::MoveAction;
//}

bool DebugTable::insertRows(int position, int rows, const QModelIndex &parent)
{
    if ( position < 0 || rows < 0 )
    { return false; }

    beginInsertRows(QModelIndex(), position, position+rows-1);

    DebugItem *pItem;
    for (int row = 0; row < rows; ++row)
    {
        pItem = new DebugItem();
        mItems.insert( position+row, pItem );
    }

    endInsertRows();
    return true;
}
bool DebugTable::removeRows(int position, int rows, const QModelIndex &parent)
{
    if ( position < 0 || rows < 1 )
    { return false; }

    beginRemoveRows(QModelIndex(), position, position+rows-1);

    for (int row = 0; row < rows; ++row)
    {
        delete mItems[position];
        mItems.removeAt(position);
    }

    endRemoveRows();
    return true;
}

QVariant DebugTable::headerData(int section, Qt::Orientation orientation, int role ) const
{
    if ( role != Qt::DisplayRole )
    { return QVariant(); }

    if ( orientation == Qt::Horizontal )
    { return QVariant( mHeaderList.at(section) ); }
    else
    { return QVariant(section);}
}

void DebugTable::exchange( int row1, int row2 )
{
    DebugItem itemTemp = *mItems.at( row1 );

    *mItems[ row1 ] = *mItems[row2];
    *mItems[ row2 ] = itemTemp;

    //! changed
    QModelIndex indexTL, indexRB;

    indexTL = createIndex( row1, 0 );
    indexRB = createIndex( row1, columnCount( QModelIndex() ) - 1 );
    emit dataChanged(indexTL, indexRB);

    indexTL = createIndex( row2, 0 );
    indexRB = createIndex( row2, columnCount( QModelIndex() ) - 1 );
    emit dataChanged(indexTL, indexRB);
}

int DebugTable::serialOut( QXmlStreamWriter &writer )
{
    writer.writeStartElement( "data" );

    for ( int i = 0; i < mItems.size(); i++ )
    {
        writer.writeStartElement("item");

        writer.writeTextElement( "number", QString::number( mItems.at(i)->mNumber) );
        writer.writeTextElement( "delay", QString::number( mItems.at(i)->mDelay) );

        writer.writeEndElement();
    }

    writer.writeEndElement();

    return 0;
}

int DebugTable::serialIn( QXmlStreamReader &reader )
{
    QList< DebugItem *> localItems;

    int ret;

    ret = _serialIn( reader, localItems );
    if( ret != 0 )
    {
        qDeleteAll( localItems );
        return ret;
    }

    beginResetModel();

    //! load success
    qDeleteAll( mItems );
    mItems.clear();

    mItems.append( localItems );

    endResetModel();

    return 0;
}

int DebugTable::_serialIn( QXmlStreamReader &reader, QList< DebugItem *> &items )
{
    DebugItem *pItem = NULL;
    while( reader.readNextStartElement() )
    {
        if ( reader.name() == "data" )
        {
            while( reader.readNextStartElement() )
            {
                if ( reader.name() == "item" )
                {
                    //! new one
                    pItem = new DebugItem;
                    if ( NULL == pItem )
                    {
                        return -1;
                    }
                    else
                    {
                        items.append( pItem );
                    }

                    while( reader.readNextStartElement() )
                    {
                        if ( reader.name() == "number" )
                        { pItem->mNumber = reader.readElementText().toInt(); }
                        else if ( reader.name() == "delay" )
                        { pItem->mDelay = reader.readElementText().toDouble(); }
                        else
                        { reader.skipCurrentElement(); }
                    }
                }
                else
                {
                    reader.skipCurrentElement();
                }
            }
        }
        else
        {
            reader.skipCurrentElement();
        }
    }

    return 0;
}

void DebugTable::createDebug()
{
//    DebugItem *pItem;

//    pItem = new DebugItem();
//    pItem->mNumber = 0;
//    pItem->mT = 1;
//    pItem->mDelay = 1;

//    mItems.append( pItem );

//    pItem = new DebugItem();
//    pItem->mNumber = 1;
//    pItem->mT = 1;
//    pItem->mDelay = 1.5;
//    mItems.append( pItem );
}

}
