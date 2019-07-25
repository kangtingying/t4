#ifndef CONTROLLERSTATUS_H
#define CONTROLLERSTATUS_H

#include <QWidget>
#include "../../plugin/xpage.h"
#include "ioindicator.h"

namespace Ui {
class ControllerStatus;
}

class ControllerStatus : public XPage
{
    Q_OBJECT

enum SysOut
{
    sys_out_enable,
    sys_out_to,
    sys_out_ready,
    sys_out_mc,
    sys_out_warning,
    sys_out_error,
};

public:
    explicit ControllerStatus(QWidget *parent = 0);
    ~ControllerStatus();

public:
    virtual void spyEdited();

protected:
    void retranslateUi();
    void updateRole( int role );

public:
    void setOut( SysOut e, bool b );
    void setWorkingMode( const QString &mode );
//    void setWarning( const QString &warning );
//    void setError( const QString &error );

    void setRole( int role );
    void setWorkingStatus( const QString &status );
    void setRecordName( const QString &name );

    void setMctChecked( bool b );
    bool isMctChecked();

    void setDevicePower( bool b );
    bool getDevicePower();

    void setDevicePowerEnable( bool b );
    bool isDevicePowerEnable();

    void setDeviceStatCheck(int index, bool b );

private:
    Ui::ControllerStatus *ui;

    QList<IoIndicator *> IoIndicatorList;
    int mRole;

Q_SIGNALS:
    void signal_mct_checked( bool b );
    bool signal_device_power( bool b );
    void signal_ack_error();
};

#endif // CONTROLLERSTATUS_H
