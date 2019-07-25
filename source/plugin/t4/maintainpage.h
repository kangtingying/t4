#ifndef MAINTAIN_H
#define MAINTAIN_H

#include <QWidget>
#include <QMovie>
#include "../plugin/xpage.h"
#include "filemanager.h"
namespace Ui {
class MaintainPage;
}

namespace mrx_t4 {

class MaintainPage : public XPage
{
    Q_OBJECT

public:
    explicit MaintainPage(QWidget *parent = 0);
    ~MaintainPage();

protected:
    virtual void enterMission();
    virtual void exitMission();

    virtual void retranslateUi();
public:
    virtual void setOperAble( bool b );
    virtual void setOnLine( bool b );
    virtual void setOpened( bool b );

    virtual void updateRole();
    virtual void updateWorkingRole( int wRole );

    virtual void updateUi();
private slots:
    void on_cmbDemo_currentIndexChanged(int index);

    void on_btnDemo_clicked();

    void on_btnRestart_clicked();

    void on_btnShutDown_clicked();

    void on_btnReset_clicked();

    void on_btnUpdate_clicked();

    void on_btnHistory_clicked();

    void on_btnFold_clicked();

    void on_btnBackup_clicked();

    void on_btnClearBackup_clicked();

    void on_cmbUser_currentIndexChanged(int index);

    void on_btnChange_clicked();

    void on_btnResetPw_clicked();

    void on_chkAutoLogin_stateChanged(int arg1);

    void on_btnRestore_clicked();

    void on_btnExport_clicked();

    void on_btnBuild_clicked();

private:
    Ui::MaintainPage *ui;
    QMovie *m_pDemoMovie;

};

}

#endif // MAINTAIN_H
