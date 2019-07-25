#include "recordtable.h"
#include "iodelegate.h"

#include "../../../include/mydebug.h"

#include "../../mrp/mdataset.h"

namespace mrx_t4{

#define char_deg                        QChar(0x00B0)
#define char_square                     QChar(0x00B2)
RecordTable::RecordTable()
{
    mHeaderList
               <<QObject::tr("Type")
               <<QObject::tr("Para.")
               <<QObject::tr("X(mm)")
               <<QObject::tr("Y(mm)")
               <<QObject::tr("Z(mm)")
               <<QObject::tr("Pw(") + char_deg + ")"
               <<QObject::tr("H(") + char_deg + ")"
               <<QObject::tr("Velocity(mm/s)")
               <<QObject::tr("Acceleration(mm/s") + char_square + ")"
               <<QObject::tr("Comment");

    setUri( "record" );
}

RecordTable::~RecordTable()
{
//    removeRows( 0, mItems.size(), QModelIndex() );
    qDeleteAll( mItems );
}

int RecordTable::rowCount(const QModelIndex &parent) const
{ return mItems.size(); }
int RecordTable::columnCount(const QModelIndex &parent) const
{ return mHeaderList.size(); }

QVariant RecordTable::data(const QModelIndex &index, int role) const
{
    if ( !index.isValid() )
    { return QVariant(); }

    int col = index.column();
    int row = index.row();

    QVariant var;

    if ( decorationRole( role, row, col, var ) )
    { return var; }

    if ( displayRole( role, row, col, var ) )
    { return var; }

    //! bypass
    if ( role != Qt::DisplayRole && role != Qt::EditRole )
    { return QVariant(); }

    //! displayrole

    //! defualt proc
    int _col = 0;
    if ( col == _col++ )
    { return QVariant( mItems[row]->mType ); }
    if ( col == _col++ )
    { return QVariant( mItems[row]->mPara ); }
    if ( col == _col++ )
    { return QVariant( mItems[row]->mX ); }
    if ( col == _col++ )
    { return QVariant( mItems[row]->mY ); }
    if ( col == _col++ )
    { return QVariant( mItems[row]->mZ ); }
    if ( col == _col++ )
    { return QVariant( mItems[row]->mPw ); }
    if ( col == _col++ )
    { return QVariant( mItems[row]->mH ); }
    if ( col == _col++ )
    { return QVariant( mItems[row]->mVel ); }
    if ( col == _col++ )
    { return QVariant( mItems[row]->mAcc ); }
    if ( col == _col++ )
    { return QVariant( mItems[row]->mComment ); }

    return QVariant();
}

bool RecordTable::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if ( index.isValid() && role == Qt::EditRole )
    {}
    else
    { return false; }

    int col = index.column();
    int row = index.row();

    int _col = 0;
    if ( col == _col++ )
    { mItems[ row ]->mType = value.toString(); }
    else if ( col == _col++ )
    { mItems[ row]->mPara = value.toString(); }
    else if ( col == _col++ )
    { mItems[ row ]->mX = value.toDouble(); }
    else if ( col == _col++ )
    { mItems[ row ]->mY = value.toDouble(); }
    else if ( col == _col++ )
    { mItems[ row ]->mZ = value.toDouble(); }
    else if ( col == _col++ )
    { mItems[ row ]->mPw = value.toDouble(); }
    else if ( col == _col++ )
    { mItems[ row ]->mH = value.toDouble(); }
    else if ( col == _col++ )
    { mItems[ row ]->mVel = value.toDouble(); }
    else if ( col == _col++ )
    { mItems[ row ]->mAcc = value.toDouble(); }
    else if ( col == _col++ )
    { mItems[ row ]->mComment = value.toString(); }
    else
    {}

    emit dataChanged(index, index);

    return true;
}
Qt::ItemFlags RecordTable::flags(const QModelIndex &index) const
{
    if (!index.isValid())
    { return Qt::ItemIsEnabled; }

    //! enabled
    if ( isItemParaAble( index.row() ) )
    {}
    else
    {
        if ( index.column() == 0 || index.column() == 8 )
        {}
        else
        {
            return QAbstractItemModel::flags(index);
        }
    }

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

bool RecordTable::insertRows(int position, int rows, const QModelIndex &parent)
{
    if ( position < 0 || rows < 0 )
    { return false; }

    beginInsertRows(QModelIndex(), position, position+rows-1);

    RecordItem *pItem;
    for (int row = 0; row < rows; ++row)
    {
        pItem = new RecordItem();
        mItems.insert( position+row, pItem );
    }

    endInsertRows();
    return true;
}
bool RecordTable::removeRows(int position, int rows, const QModelIndex &parent)
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

QVariant RecordTable::headerData(int section, Qt::Orientation orientation, int role ) const
{
    if ( role != Qt::DisplayRole )
    { return QVariant(); }

    if ( orientation == Qt::Horizontal )
    { return QVariant( mHeaderList.at(section) ); }
    //! \note from 1 to n
    if ( orientation == Qt::Vertical )
    { return QVariant( QString::number( section + 1 ) ); }
    else
    { return QVariant(section);}
}
//<<QObject::tr("Type")
//<<QObject::tr("Para.")
//<<QObject::tr("X(mm)")
//<<QObject::tr("Y(mm)")
//<<QObject::tr("Z(mm)")
//<<QObject::tr("Pw(") + char_deg + ")"
//<<QObject::tr("H(") + char_deg + ")"
//<<QObject::tr("Velocity(mm/s)")
//<<QObject::tr("Acceleration(mm/s") + char_square + ")"
//<<QObject::tr("Comment");
//! save mrp
int RecordTable::exportOut( const QString &fileName )
{
    MDataSet dataSet;
    dataSet.setModel( "MRX-T4" );
    QStringList headers;
    headers<<"type"<<"coord"<<"para"<<"di"<<"do"
           <<"x"<<"y"<<"z"<<"w"<<"h"<<"v"<<"a"<<"comment";
    dataSet.setHeaders( headers );

    MDataSection *pSection;
    pSection = dataSet.addSection();
    if ( NULL == pSection )
    { return -1; }

    bool bRet;
    QString strRow;
    for ( int i = 0; i < mItems.size(); i++ )
    {
        strRow = QString("%1,%2,%3,%4,%5,%6,%7,%8,%9,%10,%11,%12,%13").arg( mItems.at(i)->mType )
                                                          .arg( mItems.at(i)->mCoord )
                                                          .arg( mItems.at(i)->mPara )
                                                          .arg( mItems.at(i)->mDi )
                                                          .arg( mItems.at(i)->mDo )
                                                          .arg( mItems.at(i)->mX )
                                                          .arg( mItems.at(i)->mY )
                                                          .arg( mItems.at(i)->mZ )
                                                          .arg( mItems.at(i)->mPw )
                                                          .arg( mItems.at(i)->mH )
                                                          .arg( mItems.at(i)->mVel )
                                                          .arg( mItems.at(i)->mAcc )
                                                          .arg( mItems.at(i)->mComment );

        bRet = pSection->addRow(strRow );
        if ( bRet )
        {}
        else
        { return -1; }
    }

    return dataSet.save( fileName );
}

//! load mrp
int RecordTable::loadIn( const QString &fileName )
{
    MDataSet dataSet;

    int ret;
    ret = dataSet.load( fileName );
    if ( ret != 0 )
    { logDbg();return ret; }

    //! extract the data
    int secCnt = dataSet.sections();
    if ( secCnt < 1 )
    { logDbg();return -1; }

    //! get the first one
    MDataSection *pSection = dataSet.section( 0 );

    QList<RecordItem*> localItems;
    ret = _loadIn( pSection, localItems );

    if ( ret != 0 )
    {logDbg();
        qDeleteAll( localItems );
        return ret;
    }
    else
    {
        qDeleteAll( mItems );

        beginResetModel();
            mItems = localItems;
        endResetModel();
    }

    return 0;
}

#define get_v( out, row, def, local )   bRet = pSection->cellValue( row, col++, local, def );\
                                        if ( !bRet ){ logDbg(); return -1; }\
                                        out = local;

#define get_str( out, row, def )   get_v( out, row, def, strV )
#define get_float( out, row, def )   get_v( out, row, def, fV )
#define get_double( out, row, def )   get_v( out, row, def, dV )
#define get_int( out, row, def )   get_v( out, row, def, iV )

#define get_bool( out, row, def )   get_v( out, row, def, bV )

int RecordTable::_loadIn( MDataSection *pSection,
                          QList< RecordItem *> &items )
{
    RecordItem *pItem;
    bool bRet;
    int col;

    QString strV;
    int iV;
    double dV;
    float fV;
    bool bV;
    for ( int i = 0; i < pSection->rows(); i++ )
    {
        pItem = new RecordItem();
        if ( NULL == pItem )
        { return -1; }

        //! col
        col = 0;

        get_str( pItem->mType, i, "PA" );
        get_str( pItem->mCoord, i, "" );
        get_str( pItem->mPara, i, "" );
        get_int( pItem->mDi, i, 0 );
        get_int( pItem->mDo, i, 0 );

        get_double( pItem->mX, i, 0 );
        get_double( pItem->mY, i, 0 );
        get_double( pItem->mZ, i, 0 );
        get_double( pItem->mPw, i, 0 );
        get_double( pItem->mH, i, 0 );

        get_double( pItem->mVel, i, 0 );
        get_double( pItem->mAcc, i, 0 );

        //! append
        items.append( pItem );
    }

    return 0;
}


bool RecordTable::decorationRole( int role, int row, int col, QVariant &var ) const
{
    if ( role == Qt::BackgroundColorRole )
    {}
    else
    { return false; }

    if ( isItemParaAble( row) )
    {
    }
    else
    {
        //! enabled
        if ( col == 0 || col == 8 )
        {}
        //! disabled
        else
        {
            var.setValue( QColor(Qt::gray) );
            return true;
        }
    }

    return false;
}
bool RecordTable::displayRole( int role, int row,int col, QVariant &var ) const
{
    if ( role == Qt::DisplayRole )
    {}
    else
    { return false; }

    if ( isItemParaAble( row) )
    {
    }
    else
    {
        //! enabled
        if ( col == 0 || col == 8 )
        {}
        //! disabled
        else
        {
            var.clear();
            return true;
        }
    }

    return false;
}

bool RecordTable::isItemParaAble( int row ) const
{
    if ( mItems.at( row)->mType == "PA" || mItems.at( row)->mType == "RA"  )
    { return true; }
    else
    { return false; }
}

void RecordTable::createDebug()
{
    mItems.append( new RecordItem() );
    mItems.append( new RecordItem() );
    mItems.append( new RecordItem() );
}

//QString mType;
//QString mPara;
//quint32 mDi, mDo;
//float mX, mY, mZ, mH, mPw;
//float mVel, mAcc;
//QString mComment;
int RecordTable::serialOut( QXmlStreamWriter &writer )
{
    foreach( RecordItem *pItem, mItems )
    {
        Q_ASSERT( NULL != pItem );

        writer.writeStartElement("item");

            writer.writeTextElement("type", pItem->mType );
            writer.writeTextElement( "coord", pItem->mCoord );
            writer.writeTextElement( "para", pItem->mPara );

            writer.writeTextElement( "di", QString::number( pItem->mDi, 16 ) );
            writer.writeTextElement( "do", QString::number( pItem->mDo, 16 ) );

            writer.writeTextElement( "x", QString::number( pItem->mX ) );
            writer.writeTextElement( "y", QString::number( pItem->mY ) );
            writer.writeTextElement( "z", QString::number( pItem->mZ ) );
            writer.writeTextElement( "h", QString::number( pItem->mH ) );
            writer.writeTextElement( "w", QString::number( pItem->mPw ) );

            writer.writeTextElement( "v", QString::number( pItem->mVel ) );
            writer.writeTextElement( "a", QString::number( pItem->mAcc ) );

            writer.writeTextElement( "comment", pItem->mComment );

        writer.writeEndElement();
    }

    return 0;
}
int RecordTable::serialIn( QXmlStreamReader &reader )
{
    int ret;

    QList< RecordItem *> localItems;

    while( reader.readNextStartElement() )
    {
        if ( reader.name() == "item" )
        {
            RecordItem *pItem;

            pItem = new RecordItem();

            ret = _serialIn( reader, pItem );
            if ( ret == 0 )
            { localItems.append( pItem ); }
            else
            {
                qDeleteAll( localItems );
                return -1;
            }
        }
        else
        { reader.skipCurrentElement(); }
    }

    //! clean
    qDeleteAll( mItems );
    mItems.clear();

    //! shift the memory
    beginResetModel();
        mItems = localItems;
        logDbg()<<mItems.size()<<localItems.size();
    endResetModel();

    return 0;
}

int RecordTable::_serialIn( QXmlStreamReader &reader, RecordItem *pItem )
{
    Q_ASSERT( NULL != pItem );

    while( reader.readNextStartElement() )
    {
        if ( reader.name() == "type" )
        { pItem->mType = reader.readElementText(); }
        else if ( reader.name() == "coord" )
        { pItem->mCoord = reader.readElementText(); }
        else if ( reader.name() == "para" )
        { pItem->mPara = reader.readElementText(); }

        else if ( reader.name() == "di" )
        { pItem->mDi = reader.readElementText().toInt( Q_NULLPTR, 16 ); }
        else if ( reader.name() == "do" )
        { pItem->mDo = reader.readElementText().toInt( Q_NULLPTR, 16 ); }

        else if ( reader.name() == "x" )
        { pItem->mX = reader.readElementText().toDouble(); }
        else if ( reader.name() == "y" )
        { pItem->mY = reader.readElementText().toDouble(); }
        else if ( reader.name() == "z" )
        { pItem->mZ = reader.readElementText().toDouble(); }
        else if ( reader.name() == "h" )
        { pItem->mH = reader.readElementText().toDouble(); }
        else if ( reader.name() == "w" )
        { pItem->mPw = reader.readElementText().toDouble(); }

        else if ( reader.name() == "v" )
        { pItem->mVel = reader.readElementText().toDouble(); }
        else if ( reader.name() == "a" )
        { pItem->mAcc = reader.readElementText().toDouble(); }

        else if ( reader.name() == "comment" )
        { pItem->mComment = reader.readElementText(); }

        else
        { reader.skipCurrentElement(); }
    }

    return 0;
}

}
