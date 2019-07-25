#include <QFile>
#include "model.h"

MegaTableModel::MegaTableModel( QObject *parent  ) : QAbstractTableModel( parent )
{
    mUri = "data";

    connect( this, SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)),
             this, SIGNAL(signal_data_changed()));

    connect( this, SIGNAL(rowsInserted(QModelIndex,int,int)),
             this, SIGNAL(signal_data_changed()));

    connect( this, SIGNAL(rowsRemoved(QModelIndex,int,int)),
             this, SIGNAL(signal_data_changed()));

    connect( this, SIGNAL(modelReset()),
             this, SIGNAL(signal_data_changed()));

    connect( this, SIGNAL(rowsMoved(QModelIndex,int,int,QModelIndex,int)),
             this, SIGNAL(signal_data_changed()));
}

QString MegaTableModel::fmtString( const QStringList &list )
{
    QStringList strList;
    for ( int i = 1; i <= list.size(); i++ )
    {
        strList<<QString("\%%1").arg( i );
    }

    return strList.join('/');
}

void MegaTableModel::setUri( const QString &uri )
{ mUri = uri; }
QString MegaTableModel::Uri()
{ return mUri; }

int MegaTableModel::save( const QString &fileName )
{
    QFile file( fileName );

    if ( file.open( QIODevice::WriteOnly ) )
    {}
    else
    { return -1; }

    int ret;
    QXmlStreamWriter writer( &file );

    writer.writeStartDocument();

    writer.writeStartElement( mUri );

        ret = serialOut( writer );

    writer.writeEndElement();

    writer.writeEndDocument();

    file.close();

    return ret;
}

int MegaTableModel::save( QByteArray &ary )
{
    int ret;
    QXmlStreamWriter writer( &ary );

    writer.writeStartDocument();

    writer.writeStartElement( mUri );

        ret = serialOut( writer );

    writer.writeEndElement();

    writer.writeEndDocument();

    return ret;
}

int MegaTableModel::load( const QString &fileName )
{
    QFile file( fileName );

    if ( file.open( QIODevice::ReadOnly ) )
    {}
    else
    { return -1; }

    int ret;
    QXmlStreamReader reader( &file );

    while( reader.readNextStartElement() )
    {
        if ( reader.name()==mUri )
        {
            ret = serialIn( reader );
        }
        else
        { reader.skipCurrentElement(); }
    }

    file.close();

    return ret;
}

int MegaTableModel::load( QByteArray &ary )
{
    int ret;
    QXmlStreamReader reader( ary );

    while( reader.readNextStartElement() )
    {
        if ( reader.name()==mUri )
        {
            ret = serialIn( reader );
        }
        else
        { reader.skipCurrentElement(); }
    }

    return ret;
}

int MegaTableModel::serialOut( QXmlStreamWriter &writer )
{ return -1; }
int MegaTableModel::serialIn( QXmlStreamReader &reader )
{ return -1; }
