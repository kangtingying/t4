#include "syspara.h"
#include <QFile>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>

SysPara::SysPara()
{
    init();

    reset();
}

void SysPara::init()
{
    mPw = qCompress("root");
}

void SysPara::reset()
{
    mbAutoExpand = true;
    mbAutoLoad = true;
    mbAutoSearch = false;
    mbAutoMax = true;
    mbAutoRpp = true;

    mIntfIndex = 0;
    mStyleIndex = 0;
    mRefreshIndex = 4;

//    mSysMode = 0;   //! operator
}

int SysPara::save( const QString &fileName )
{
    QFile file( fileName );
    if ( file.open( QIODevice::WriteOnly ) )
    {}
    else
    { return -1; }

    QXmlStreamWriter writer( &file );

    writer.writeStartDocument();

    writer.writeStartElement("pref");

    int ret = serialOut( writer );

    writer.writeEndElement();

    writer.writeEndDocument();

    file.close();

    return ret;
}
int SysPara::load( const QString &fileName )
{
    QFile file( fileName );
    if ( file.open( QIODevice::ReadOnly ) )
    {}
    else
    { return -1; }

    QXmlStreamReader reader( &file );

    int ret = 0;
    while( reader.readNextStartElement() )
    {
        if ( reader.name() == "pref" )
        {
            ret = serialIn( reader );
        }
        else
        { reader.skipCurrentElement(); }
    }

    return ret;
}

int SysPara::refreshIntervalMs()
{
    //! 0.1 0.2 0.5
    //! 1 2 5
    //! 10 20 50
    int interval[]={
        10000, 5000, 2000,
        1000, 500, 200,
        100, 50, 20
    };

    if ( mRefreshIndex < 0 || mRefreshIndex >= sizeof(interval)/sizeof(interval[0]) )
    { return interval[0]; }
    else
    { return interval[ mRefreshIndex ]; }
}

int SysPara::serialOut( QXmlStreamWriter &writer )
{
    writer.writeStartElement( "common" );
        writer.writeTextElement( "language", QString::number( mLangIndex ) );
        writer.writeTextElement( "style", QString::number( mStyleIndex ) );

        writer.writeTextElement( "auto_expand", QString::number( mbAutoExpand ) );
        writer.writeTextElement( "auto_load", QString::number( mbAutoLoad ) );
        writer.writeTextElement( "auto_search", QString::number( mbAutoSearch ) );
        writer.writeTextElement( "auto_max", QString::number( mbAutoMax ) );
        writer.writeTextElement( "auto_rpp", QString::number(mbAutoRpp) );

        writer.writeTextElement( "intf", QString::number(mIntfIndex) );
        writer.writeTextElement( "refresh",QString::number( mRefreshIndex ));

        writer.writeTextElement( "code", mPw.toBase64() );
    writer.writeEndElement();

    writer.writeStartElement( "plugins");
    foreach( const QString &str, mPlugins )
    {
        writer.writeTextElement( "plugin", str );
    }
    writer.writeEndElement();

    return 0;
}

int SysPara::serialIn( QXmlStreamReader &reader )
{
    while( reader.readNextStartElement() )
    {
        if ( reader.name() == "common" )
        {
            while( reader.readNextStartElement() )
            {
                if ( reader.name() == "language" )
                { mLangIndex = reader.readElementText().toInt(); }
                else if ( reader.name() == "style" )
                { mStyleIndex = reader.readElementText().toInt(); }
                else if ( reader.name() == "auto_expand" )
                { mbAutoExpand = reader.readElementText().toInt() > 0; }
                else if ( reader.name() == "auto_load" )
                { mbAutoLoad = reader.readElementText().toInt() > 0; }
                else if ( reader.name() == "auto_search" )
                { mbAutoSearch = reader.readElementText().toInt() > 0; }
                else if ( reader.name() == "auto_max" )
                { mbAutoMax = reader.readElementText().toInt() > 0; }
                else if ( reader.name() == "auto_rpp" )
                { mbAutoRpp = reader.readElementText().toInt() > 0; }
                else if ( reader.name() == "intf" )
                { mIntfIndex = reader.readElementText().toInt(); }
                else if ( reader.name() == "refresh" )
                { mRefreshIndex = reader.readElementText().toInt(); }
                else if ( reader.name() == "code" )
                { mPw = (QByteArray::fromBase64( reader.readElementText().toLatin1() )); }
                else
                { reader.skipCurrentElement(); }
            }
        }
        else if ( reader.name() =="plugins" )
        {
            while( reader.readNextStartElement() )
            {
                if ( reader.name() == "plugin" )
                { mPlugins.append( reader.readElementText()); qDebug()<<mPlugins.last(); }
                else
                { reader.skipCurrentElement(); }
            }
        }
        else
        { reader.skipCurrentElement(); }
    }

    return 0;
}
