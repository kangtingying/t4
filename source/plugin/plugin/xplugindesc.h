#ifndef XPLUGINDESC_H
#define XPLUGINDESC_H

#include <QtCore>

class XPluginDesc
{
public:
    XPluginDesc();

public:
    void setModel( const QString &model );
    QString getModel();

    void setAddr( const QString &addr );
    QString getAddr();

public:
    virtual int open();
    virtual void close();

    bool isOpened();

    int deviceVi();

protected:
    QString mModel, mAddr;
    int mVi;
};
#endif // XPLUGINDESC_H
