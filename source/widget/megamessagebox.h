#ifndef MEGAMESSAGEBOX_H
#define MEGAMESSAGEBOX_H

#include <QMessageBox>
#include "sysapi.h"

class MegaMessageBox : public QMessageBox
{
    Q_OBJECT
public:
    static void setZeroAffirm( bool b);
    bool zeroAffirm();

protected:
    static bool _bZeroAffirm;

public:
    MegaMessageBox( QWidget *parent = Q_NULLPTR );

    MegaMessageBox( const QString &info,
                    QWidget *parent = Q_NULLPTR );

};

//class MegaZeroAffirmMessageBox : public MegaMessageBox
//{

//public:
//    MegaZeroAffirmMessageBox( QWidget *parent = Q_NULLPTR );

//public:
//    virtual int exec();
//};

class MegaDeleteAffirmMessageBox : public MegaMessageBox
{
    Q_OBJECT
public:
    MegaDeleteAffirmMessageBox( QWidget *parent = Q_NULLPTR );

};

#endif // MEGAMESSAGEBOX_H
