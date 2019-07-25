#include "diagnosistable.h"

QString DiagnosisElement::toString( DiagnosisElement::DiagnosisType type )
{
    if ( DiagnosisElement::diag_info == type )
    { return QObject::tr("Info."); }
    else if ( DiagnosisElement::diag_warning == type )
    { return QObject::tr("Warning"); }
    else if ( DiagnosisElement::diag_error == type )
    { return QObject::tr("Error"); }
    else
    { return QObject::tr("Info."); }
}

DiagnosisElement::DiagnosisElement()
{
    mNumber = 0;
    mType = diag_info;
    mCounter = 0;
}

DiagnosisTable::DiagnosisTable( ): MegaTableModel( )
{
    mHeaderList<<QObject::tr("No.")
               <<QObject::tr("Type")
               <<QObject::tr("Timestamp")
               <<QObject::tr("Additional info")
               <<QObject::tr("Counter")
               <<QObject::tr("Message");
}

DiagnosisTable::~DiagnosisTable()
{
//    removeRows( 0, mItems.size(), QModelIndex() );
    qDeleteAll( mItems );
}

int DiagnosisTable::rowCount(const QModelIndex &parent) const
{ return mItems.size(); }
int DiagnosisTable::columnCount(const QModelIndex &parent) const
{ return mHeaderList.size(); }

QVariant DiagnosisTable::data(const QModelIndex &index, int role) const
{
    if ( !index.isValid() )
    { return QVariant(); }

    int col = index.column();
    int row = index.row();

    //! \note can not be edited
    if ( role != Qt::DisplayRole || role == Qt::EditRole )
    { return QVariant(); }

    if ( col == 0 )
    { return QVariant( QString("0x%1").arg( mItems[row]->mNumber,4,16,QChar('0') ) ); }
    if ( col == 1 )
    { return QVariant( DiagnosisElement::toString( mItems[row]->mType ) ); }
    if ( col == 2 )
    { return QVariant( mItems[row]->mTs ); }
    if ( col == 3 )
    { return QVariant( mItems[row]->mInfo ); }
    if ( col == 4 )
    { return QVariant( mItems[row]->mCounter ); }
    if ( col == 5 )
    { return QVariant( mItems[row]->mMessage ); }

    return QVariant();
}

bool DiagnosisTable::setData(const QModelIndex &index, const QVariant &value, int role)
{
    return false;
//    if ( index.isValid() && role == Qt::EditRole )
//    {}
//    else
//    { return false; }

//    int col = index.column();
//    int row = index.row();

//    if ( col == 0 )
//    { mItems[ row ]->mT = value.toDouble(); }
//    else if ( index.column() == 1 )
//    { mItems[ row ]->mNumber = value.toInt(); }
//    else if ( index.column() == 2 )
//    { mItems[ row ]->mDelay = value.toDouble(); }

//    else
//    {}

//    emit dataChanged(index, index);

//    return true;
}
Qt::ItemFlags DiagnosisTable::flags(const QModelIndex &index) const
{
    if (!index.isValid())
    { return Qt::ItemIsEnabled; }

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

bool DiagnosisTable::insertRows(int position, int rows, const QModelIndex &parent)
{
    if ( position < 0 || rows < 0 )
    { return false; }

    beginInsertRows(QModelIndex(), position, position+rows-1);

    DiagnosisElement *pItem;
    for (int row = 0; row < rows; ++row)
    {
        pItem = new DiagnosisElement();
        mItems.insert( position+row, pItem );
    }

    endInsertRows();
    return true;
}
bool DiagnosisTable::removeRows(int position, int rows, const QModelIndex &parent)
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

QVariant DiagnosisTable::headerData(int section, Qt::Orientation orientation, int role ) const
{
    static const char *header_strings[] = {
          QT_TR_NOOP("No."),
          QT_TR_NOOP("Type"),
          QT_TR_NOOP("Timestamp"),
          QT_TR_NOOP("Additional info"),
          QT_TR_NOOP("Counter"),
          QT_TR_NOOP("Message")
      };

    if ( role != Qt::DisplayRole )
    { return QVariant(); }

    if ( orientation == Qt::Horizontal
         && section < sizeof(header_strings)/sizeof(header_strings[0]) )
    {
        return QVariant( QObject::tr( header_strings[section] ) );
    }
    else
    { return QVariant(section);}
}
//int mNumber;
//DiagnosisType mType;
//QString mTs;
//QString mInfo;
//int mCounter;
//QString mMessage;
int DiagnosisTable::serialOut( QXmlStreamWriter &writer )
{
    writer.writeStartElement( "data" );

    for ( int i = 0; i < mItems.size(); i++ )
    {
        writer.writeStartElement("item");

        writer.writeTextElement( "number", QString::number( mItems.at(i)->mNumber) );
        writer.writeTextElement( "type", QString::number( (int)mItems.at(i)->mType) );
        writer.writeTextElement( "ts", mItems.at(i)->mTs );
        writer.writeTextElement( "info", mItems.at(i)->mInfo );

        writer.writeTextElement( "counter", QString::number( mItems.at(i)->mCounter ) );
        writer.writeTextElement( "message", ( mItems.at(i)->mMessage ) );

        writer.writeEndElement();
    }

    writer.writeEndElement();

    return 0;
}

int DiagnosisTable::serialIn( QXmlStreamReader &reader )
{
    QList< DiagnosisElement *> localItems;

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

int DiagnosisTable::_serialIn( QXmlStreamReader &reader, QList< DiagnosisElement *> &items )
{
    DiagnosisElement *pItem = NULL;
    while( reader.readNextStartElement() )
    {
        if ( reader.name() == "data" )
        {
            while( reader.readNextStartElement() )
            {
                if ( reader.name() == "item" )
                {
                    //! new one
                    pItem = new DiagnosisElement;
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
                        { pItem->mNumber = reader.readElementText().toDouble(); }
                        else if ( reader.name() == "type" )
                        { pItem->mType = (DiagnosisElement::DiagnosisType)reader.readElementText().toInt(); }
                        else if ( reader.name() == "ts" )
                        { pItem->mTs = reader.readElementText(); }
                        else if ( reader.name() == "info" )
                        { pItem->mInfo = reader.readElementText(); }

                        else if ( reader.name() == "counter" )
                        { pItem->mCounter = reader.readElementText().toInt(); }
                        else if ( reader.name() == "message" )
                        { pItem->mMessage = reader.readElementText(); }

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

int DiagnosisTable::append( int code, int cnt, const QString &info )
{
    DiagnosisElement *pEle;

    pEle = new DiagnosisElement();
    if ( NULL == pEle )
    { return -1; }

    pEle->mNumber = code;
    pEle->mCounter = cnt;
    pEle->mInfo = info;

    beginInsertRows( QModelIndex(), mItems.size() + 1, mItems.size() + 1 );
    mItems.append( pEle );
    endInsertRows();

    return 0;
}

int DiagnosisTable::append( int code, DiagnosisElement::DiagnosisType type, QString stmp, QString info, int cnt, QString msg )
{
    DiagnosisElement *pEle;

    pEle = new DiagnosisElement();
    if ( NULL == pEle )
    { return -1; }

    pEle->mNumber = code;
    pEle->mType = type;
    pEle->mTs = stmp;
    pEle->mInfo = info;
    pEle->mCounter = cnt;
    pEle->mMessage = msg;

    beginInsertRows( QModelIndex(), mItems.size() + 1, mItems.size() + 1 );
    mItems.append( pEle );
    endInsertRows();

    return 0;
}

void DiagnosisTable::createDebug()
{
    DiagnosisElement *pEle;

    pEle = new DiagnosisElement();
    mItems.append( pEle );
}
