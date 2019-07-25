#ifndef JOINT_H
#define JOINT_H

#include <QWidget>

namespace Ui {
class Joint;
}

class Joint : public QWidget
{
public:
    enum ViewMode
    {
        view_unk,
        view_angle,
        view_distance,
    };

    Q_OBJECT

public:
    explicit Joint(QWidget *parent = 0);
    ~Joint();

Q_SIGNALS:
    void signal_zero_clicked();

    void signal_single_add_clicked();
    void signal_single_sub_clicked();

    void signal_single_add_pressed();
    void signal_single_add_released();

    void signal_single_sub_pressed();
    void signal_single_sub_released();

public:
    void setViewMode( Joint::ViewMode v );
    Joint::ViewMode viewMode();

    void setJointName( const QString &name );

    void setAngleVisible( bool bAbs, bool bDelta );

    void setAngle( double angle );
    double getAngle();

    void setdAngle( double v );
    double getdAngle();

    void setDistance( double d );
    double getDistance();

protected:
    ViewMode mViewMode;
    bool mAbsVisible;
    bool mbDeltaVisible;

private:
    Ui::Joint *ui;
};

#endif // JOINT_H
