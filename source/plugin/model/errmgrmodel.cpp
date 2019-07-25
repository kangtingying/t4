#include <QFile>
#include "../../../include/mystd.h"
#include "errmgrmodel.h"
#include "../../sys/sysapi.h"   //! lang

QString ErrorMgrItem::encode( bool b )
{ return b ? "11" : "10"; }
bool ErrorMgrItem::decode( const QString &str )
{
    if ( str=="10" )
    { return false; }
    else
    { return true; }
}

ErrorMgrItem::ErrorMgrItem()
{ }

void ErrorMgrItem::setOutput( bool b )
{ mOutput=encode(b); }
bool ErrorMgrItem::getOutput()
{ return decode(mOutput); }
bool ErrorMgrItem::isOutputAble()
{ return mOutput != "00"; }

void ErrorMgrItem::setLed( bool b )
{ mLed=encode(b); }
bool ErrorMgrItem::getLed()
{ return decode(mLed); }
bool ErrorMgrItem::isLedAble()
{ return mLed != "00" ; }

ErrorMgrModel::ErrorMgrModel() : MegaTableModel()
{
    mHeaderList<<QObject::tr("No.")
               <<QObject::tr("Brief")
               <<QObject::tr("Type")
               <<QObject::tr("Reason")
               <<QObject::tr("Action")
               <<QObject::tr("Output")
               <<QObject::tr("Blink");
}

ErrorMgrModel::~ErrorMgrModel()
{
    qDeleteAll( mItems );
}

int ErrorMgrModel::rowCount(const QModelIndex &parent) const
{ return mItems.count(); }
int ErrorMgrModel::columnCount(const QModelIndex &parent) const
{ return mHeaderList.size(); }

QVariant ErrorMgrModel::data(const QModelIndex &index, int role) const
{
    if ( !index.isValid() )
    { return QVariant(); }

    //! index
    int col = index.column();
    int row = index.row();

    //! visible
    if ( role == Qt::UserRole )
    {
        if ( col == 5 )
        { return mItems.at(row)->isOutputAble(); }
        else if ( col == 6 )
        { return mItems.at(row)->isLedAble(); }
        else
        { return QVariant(); }
    }

    if ( role == Qt::DisplayRole )
    {
        int langIndex;

        langIndex = sysLangIndex() == e_lang_cn ? 0 : 1;

        if ( col == 0 )
        { return mItems.at( row )->mCode; }
        else if ( col == 1 )
        { return mItems.at(row)->mBrief[langIndex]; }
        else if ( col == 2 )
        { return mItems.at(row)->mType; }
        else if ( col == 3 )
        { return mItems.at(row)->mReason[langIndex]; }
        else if ( col == 4 )
        { return mItems.at(row)->mAction[langIndex]; }
        else
        { }
    }

    if ( role == Qt::DisplayRole
         || role == Qt::EditRole )
    {
        if ( col == 5 )
        {
            if ( mItems.at(row)->isOutputAble() )
            {}
            else
            { return QVariant(); }

            return mItems.at( row )->decode( mItems.at(row)->mOutput );
        }
        else if ( col == 6 )
        {
            if ( mItems.at(row)->isLedAble() )
            {}
            else
            { return QVariant(); }

            return mItems.at( row )->decode( mItems.at(row)->mLed );
        }
        else
        {}
    }

    return QVariant();
}

bool ErrorMgrModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    int col = index.column();
    int row = index.row();

    if (index.isValid() && role == Qt::EditRole)
    {
        if ( col == 5 )
        { mItems[row]->setOutput( value.toBool() );}
        else if ( col == 6 )
        { mItems[row]->setLed( value.toBool() );}
        else
        { return false; }

        emit dataChanged(index, index);

        return true;
    }
    else
    {
        return false;
    }
}

#define enable_flags( b )   b ? ( Qt::ItemIsSelectable \
                                  | Qt::ItemIsEditable \
                                  | Qt::ItemIsEnabled \
                                    ) : ( Qt::NoItemFlags )
Qt::ItemFlags ErrorMgrModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
          return Qt::ItemIsEnabled;

    if ( index.isValid() )
    {}
    else
    { return Qt::NoItemFlags; }

    int col = index.column();
    int row = index.row();
    if ( col == 1
         || col == 3
         || col == 4 )
    {
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
    }
    else if ( col == 5 )
    {
        return enable_flags ( mItems.at(row)->isOutputAble() );
    }
    else if ( col == 6 )
    {
        return enable_flags ( mItems.at(row)->isLedAble() );
    }
    else
    {}

    return Qt::NoItemFlags;
}

QVariant ErrorMgrModel::headerData(int section, Qt::Orientation orientation, int role ) const
{
    if ( role != Qt::DisplayRole )
    { return QVariant(); }

    if ( orientation == Qt::Horizontal )
    { return QVariant( mHeaderList.at(section) ); }
    else
    { return QVariant(section);}
}

//! code -> brief
//! langIndex : 0 -- CN
//!             1 -- EN
QVariant ErrorMgrModel::errorBrief( int errCode,
                                    int langIndex )
{
    int code;
    bool bOk;
    for( int i = 0; i < mItems.size(); i++ )
    {
        code = mItems.at( i )->mCode.toInt( &bOk, 16 );
        if ( bOk )
        { }
        else
        { continue; }

        if ( errCode == code && langIndex < mItems.at(i)->mBrief.size() )
        { return mItems.at(i)->mBrief.at( langIndex ); }
        else
        { }
    }

    return QVariant();
}

QList< ErrorMgrItem *> *ErrorMgrModel::items()
{
    return &mItems;
}

//int ErrorMgrModel::save( const QString &fileName )
//{
//    QFile fileOut( fileName );

//    if ( !fileOut.open( QIODevice::WriteOnly) )
//    { return -1; }

//    QXmlStreamWriter writer( &fileOut );

//    int ret;

//    writer.writeStartDocument();

//    writer.writeStartElement("event");

//    ret = serialOut( writer );

//    writer.writeEndElement();

//    writer.writeEndDocument();

//    fileOut.close();

//    return ret;
//}

int ErrorMgrModel::load( const QString &fileName )
{
    //! check ver
    QFile fileIn(fileName);
    if ( !fileIn.open( QIODevice::ReadOnly) )
    { return -1; }

    QByteArray ary = fileIn.readAll();

    fileIn.close();

    return load( ary );
}

int ErrorMgrModel::load( QByteArray &ary )
{
    QTextStream stream( ary, QIODevice::ReadOnly );

    int ret = serialIn( stream );

    return ret;
}

#define output_item( item )    writer<<item##Key<<":"<<item<<endl;
#define output_items(  items  ) Q_ASSERT( items.size() == items##Key.size() );\
                                for ( int i = 0; i < items.size(); i++ )\
                                {  \
                                    writer<<items##Key.at(i)<<":"<<items.at(i)<<endl;\
                                }
int ErrorMgrModel::serialOut( QTextStream & writer )
{
    foreach( ErrorMgrItem *pItem, mItems )
    {
        writer<<"################"<<endl;
        output_item( pItem->mCode );

        output_items( pItem->mBrief );
        output_items( pItem->mReason );
        output_items( pItem->mAction );

        output_items( pItem->mDetail1 );
        output_items( pItem->mDetail2 );

        output_item( pItem->mType );
        output_item( pItem->mOutput );

        output_item( pItem->mLed );
    }

    return 0;
}

#define cat_items( tItem ) \
    kvlist = lineStr.split( ":", QString::SkipEmptyParts );\
if ( kvlist.size() == 2 )\
{\
    tItem##Key.append( kvlist.at(0) );\
    tItem.append( kvlist.at(1));\
}\
else if ( kvlist.size() == 1 )\
{\
    tItem##Key.append( kvlist.at(0) );\
    tItem.append("");\
}\
else\
{\
    tItem##Key.append( lineStr );\
    tItem.append("");\
}

#define assign_item( tItem ) \
    kvlist = lineStr.split( ":", QString::SkipEmptyParts );\
if ( kvlist.size() == 2 )\
{\
    tItem##Key = kvlist.at(0);\
    tItem = kvlist.at(1);\
}\
else\
{\
    tItem##Key = lineStr;\
}

int ErrorMgrModel::serialIn( QTextStream & reader )
{
    //! item
    ErrorMgrItem *pItem = NULL;
    QList< ErrorMgrItem * > localItems;

    QString lineStr;
    QStringList kvlist;

    while( !reader.atEnd() )
    {
        lineStr = reader.readLine();

        //! deparse the item
        if ( lineStr.startsWith("#") )
        { continue; }

        if ( lineStr.startsWith("[code]:") )
        {
            pItem = new ErrorMgrItem();
            //! add item
            if ( NULL == pItem )
            {
                delete_all( localItems );
                return -1;
            }
            localItems.append( pItem );

            assign_item( pItem->mCode );

        }
        else if ( lineStr.startsWith("[brief]") )
        {
            cat_items( pItem->mBrief );
        }
        else if ( lineStr.startsWith("[reason]") )
        {
            cat_items( pItem->mReason );
        }
        else if ( lineStr.startsWith("[handle]") )
        {
            cat_items( pItem->mAction );
        }
        else if ( lineStr.startsWith("[detail1]") )
        {
            cat_items( pItem->mDetail1 );
        }
        else if ( lineStr.startsWith("[detail2]") )
        {
            cat_items( pItem->mDetail2 );
        }
        else if ( lineStr.startsWith("[level]") )
        {
            cat_items( pItem->mType );
        }
        else if ( lineStr.startsWith("[response]") )
        {
            assign_item( pItem->mOutput );
        }
        else if ( lineStr.startsWith("[led]") )
        {
            assign_item( pItem->mLed );
        }
        else
        {}
    }

    //! assign
    delete_all( mItems );
    mItems = localItems;

    endResetModel();

    return 0;
}

//void ErrMgrModel::createDebug()
//{
//    ErrMgrItem *pItem;

//    pItem = new ErrMgrItem();
//    pItem->mNr = 1;
//    pItem->mErr = "hello";

//    pItem->mEventType = e_error;

//    pItem->mbOutput = true;
//    pItem->mbOutputAble = true;
//    pItem->mbSaveDiagnosis = true;

//    mItems.append( pItem );
//}
