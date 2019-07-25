#ifndef DIAGNOSISTABLE_H
#define DIAGNOSISTABLE_H

#include "model.h"

class DiagnosisElement
{
public:
    enum DiagnosisType
    {
        diag_info,
        diag_warning,
        diag_error
    };
public:
    int mNumber;
    DiagnosisType mType;
    QString mTs;
    QString mInfo;
    int mCounter;
    QString mMessage;

public:
    static QString toString( DiagnosisType type );

public:
    DiagnosisElement();
};

class DiagnosisTable : public MegaTableModel
{
    Q_OBJECT
public:
    DiagnosisTable();
    virtual ~DiagnosisTable();

public:
    virtual int rowCount(const QModelIndex &parent) const;
    virtual int columnCount(const QModelIndex &parent) const;

    virtual QVariant data(const QModelIndex &index, int role) const;

    virtual bool setData(const QModelIndex &index, const QVariant &value, int role);
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;

    virtual bool insertRows(int position, int rows, const QModelIndex &parent);
    virtual bool removeRows(int position, int rows, const QModelIndex &parent);

    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

protected:
    virtual int serialOut( QXmlStreamWriter &writer );
    virtual int serialIn( QXmlStreamReader &reader );

    int _serialIn( QXmlStreamReader &reader, QList< DiagnosisElement *> &items );


public:
    int append( int code, int cnt, const QString &info );
    int append( int code, DiagnosisElement::DiagnosisType type, QString stmp, QString info, int cnt, QString msg );

    void createDebug();

public:
    QList< DiagnosisElement *> mItems;

};

#endif // DIAGNOSISTABLE_H
