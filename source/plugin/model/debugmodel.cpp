#include "debugmodel.h"

#include "debugmodel.h"
#include "../../../include/mystd.h"

DebugModel::DebugModel() : MegaTableModel()
{

}

DebugModel::~DebugModel()
{
    delete_all( mItems );
}

int DebugModel::rowCount(const QModelIndex &parent) const
{ return mItems.count(); }
int DebugModel::columnCount(const QModelIndex &parent) const
{ return DebugItem::columns(); }

QVariant DebugModel::data(const QModelIndex &index, int role) const
{
    if ( !index.isValid() )
    { return QVariant(); }

    //! index
    int col = index.column();
    int row = index.row();

    if ( role != Qt::DisplayRole && role != Qt::EditRole )
    { return QVariant(); }

    if ( col == 0 )
    { return QVariant( mItems[ row ]->mId ); }
    else if ( col == 1 )
    { return QVariant( ( mItems[ row ]->mDelay ) ); }

    else
    { return QVariant(); }
}

bool DebugModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole)
    {
        int col = index.column();
        int row = index.row();
        if ( col == 0 )
        { mItems[ row ]->mId = value.toInt(); }
        else if ( index.column() == 1 )
        { mItems[ row ]->mDelay = value.toDouble(); }

        else
        {}

        emit dataChanged(index, index);

        return true;
    }
    else
    {
        return false;
    }
}
Qt::ItemFlags DebugModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
          return Qt::ItemIsEnabled;

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

QMimeData *DebugModel::mimeData(const QModelIndexList &indexes) const
{
    QMimeData *pMime;

    pMime = new QMimeData();
    if ( NULL == pMime )
    { return NULL; }

    QByteArray ary;

    ary.setNum( indexes.at(0).row() );

    pMime->setData( "debug/row", ary );
logDbg()<<pMime;
    return pMime;
}

QStringList DebugModel::mimeTypes() const
{
    QStringList strList;

    strList<<"debug/row";
logDbg()<<strList;
    return strList;
}

bool DebugModel::dropMimeData(const QMimeData *data,
                              Qt::DropAction action,
                              int row,
                              int column,
                              const QModelIndex &parent )
{logDbg()<<data;
    if ( data->hasFormat("debug/row") )
    {}
    else
    { return false; }

    //! exchange the row
    QByteArray ary;
    ary = data->data( "debug/row" );
    int srcRow = ary.toInt();

    //! get src
    DebugItem proxyItem = *mItems.at( srcRow );

    removeRow( srcRow );

    insertRow( row );

    //! set data
    *mItems.at( row ) = proxyItem;

    return true;
}

bool DebugModel::insertRows(int position, int rows, const QModelIndex &parent)
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
bool DebugModel::removeRows(int position, int rows, const QModelIndex &parent)
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

QVariant DebugModel::headerData(int section, Qt::Orientation orientation, int role ) const
{
    if ( role != Qt::DisplayRole )
    { return QVariant(); }

    if ( orientation == Qt::Horizontal )
    {
        if ( role == Qt::TextAlignmentRole )
        { return Qt::AlignLeft; }
        else
        { return QVariant( DebugItem::header(section)); }
    }
    else
    { return QVariant(section);}
}

QList< DebugItem *> *DebugModel::items()
{
    return &mItems;
}

int DebugModel::save( const QString &fileName )
{
    QFile fileOut( fileName );

    if ( !fileOut.open( QIODevice::WriteOnly) )
    { return -1; }

    QXmlStreamWriter writer( &fileOut );

    int ret;

    writer.writeStartDocument();

    writer.writeStartElement("debug");

    ret = serialOut( writer );

    writer.writeEndElement();

    writer.writeEndDocument();

    fileOut.close();

    return ret;
}

int DebugModel::load( const QString &fileName )
{
    //! check ver
    QFile fileIn(fileName);
    if ( !fileIn.open( QIODevice::ReadOnly) )
    { return -1; }

    QXmlStreamReader reader( &fileIn );

    int ret = 0;
    while( reader.readNextStartElement() )
    {
        if ( reader.name() == "debug" )
        {
            ret = serialIn( reader );
        }
        else
        { reader.skipCurrentElement(); }
    }

    fileIn.close();

    return ret;
}

int DebugModel::serialOut( QXmlStreamWriter & writer )
{
    foreach( DebugItem *pAction, mItems )
    {
        Q_ASSERT( NULL != pAction );

        writer.writeStartElement( "item" );

        writer.writeTextElement( "mId", QString::number( pAction->mId ) );
        writer.writeTextElement( "mDelay", QString::number( pAction->mDelay ) );

        writer.writeEndElement();
    }

    return 0;

}
int DebugModel::serialIn( QXmlStreamReader & reader )
{
    //! item
    DebugItem *pItem;
    QList< DebugItem * > localItems;

    while( reader.readNextStartElement() )
    {
        if ( reader.name() == "item" )
        {
            pItem = new DebugItem();
            Q_ASSERT( NULL != pItem );

            while( reader.readNextStartElement() )
            {

                if ( reader.name() == "mId" )
                { pItem->mId = reader.readElementText().toInt(); }
                else if ( reader.name() == "mDelay" )
                { pItem->mDelay = reader.readElementText().toDouble(); }

                else
                { reader.skipCurrentElement(); }
            }

            localItems.append( pItem );
        }
        else
        { reader.skipCurrentElement(); }
    }

    //! assign
    delete_all( mItems );
    mItems = localItems;

    endResetModel();

    return 0;
}

void DebugModel::signal_dataChanged( const QModelIndex &tl, const QModelIndex &br )
{
    emit dataChanged( tl, br );
}


