#ifndef _THE_MEGAROBOMODEL_H_
#define _THE_MEGAROBOMODEL_H_

#include <QAbstractTableModel>
#include <QAbstractItemView>

#include <QXmlStreamWriter>
#include <QXmlStreamReader>

class MegaTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    MegaTableModel( QObject *parent=Q_NULLPTR );
    virtual ~MegaTableModel()
    {}

    QString fmtString( const QStringList &list );

    void setUri( const QString &uri );
    QString Uri();

public:
    int save( const QString &fileName );
    int save( QByteArray &ary );

    int load( const QString &fileName );
    int load( QByteArray &ary );
protected:
    virtual int serialOut( QXmlStreamWriter &writer );
    virtual int serialIn( QXmlStreamReader &reader );

Q_SIGNALS:
    void signal_data_changed();
    void signal_current_changed( int row );

protected:
    QStringList mHeaderList;
    QString mUri;
};

#endif // MEGATABLEMODEL_H
