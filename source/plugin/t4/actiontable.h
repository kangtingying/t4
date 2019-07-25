#ifndef ACTIONTABLE_H
#define ACTIONTABLE_H

#include <QWidget>
#include <QAbstractTableModel>
#include <QMenu>
#include "../plugin/xpage.h"

#include "./model/iodelegate.h"
#include "../model/delegate/comboxdelegate.h"

#include "../../model/delegate/dspindelegate.h"
#include "../../model/delegate/checkdelegate.h"

#include "filemanager.h"

namespace Ui {
class ActionTable;
}

namespace mrx_t4 {

class ActionTable : public XPage
{
    Q_OBJECT

public:
    enum eActionEvent
    {
        e_event_post_load = Qt::UserRole + 1
    };

public:
    explicit ActionTable(QWidget *parent = 0);
    ~ActionTable();

protected:
    virtual void keyReleaseEvent(QKeyEvent *event);
    virtual bool event(QEvent *event);

protected:
    virtual void setOperAble( bool b );
    virtual void setOpened( bool b );

    virtual void retranslateUi();

    void buildConnection();

public:
    void setModel( QAbstractItemModel *pModel );

    virtual void onSetting(XSetting setting);

    virtual void rst();

    virtual int upload();
    virtual int download();
    virtual int diff();    

    //! load the file
    int requestLoad( const QString &path, const QString &name );

protected:
    virtual void enterMission();
    virtual void exitMission();

public:
    int currentIndex();

protected:
    void addRecord( XSetting setting );
    void editRecord( XSetting setting );

    void doSave();
    void doLoad();

    void postDoLoad();

    int _doSave( const QString &fileName );
    int _doLoad( const QString &fileName );

    int postSaveProc( void *pContext );

protected:
    void updateControl();

    int onToHere( QVariant var );
    int onHoming( QVariant var );

    int terminalAction( QList<QVariant> &vars );
    int relToHere( QList<QVariant> &vars );
    int absToHere( QList<QVariant> &vars );
    int rellabsToHere( QList<QVariant> &vars );

private:
    Ui::ActionTable *ui;

    QAbstractItemView::EditTriggers mRawEditTriggers;

    IODelegate *m_pODelegate;
    ComboxDelegate *m_pTypeDelegate;
    dSpinDelegate *m_pDelegateXYZ;
    dSpinDelegate *m_pDelegatePw;
    dSpinDelegate *m_pDelegateH;
    dSpinDelegate *m_pDelegateV;

    CheckDelegate *m_pCheckDelegate;
    dSpinDelegate *m_pDelegateTime;

    QMenu *m_pContextMenu;
    QAction *m_pActionToHere;
    QAction *m_pActionAddBefore, *m_pActionAddBelow;
    QAction *m_pActionDelete;

    QAction *m_pActionUp, *m_pActionDown;

    QAction *m_pActionExpandAll, *m_pActionCollapseAll;

    QAction *m_pActionHoming;

    QAction *m_pActionResize;

    QTimer mPostSaveTimer;
    QString mPostSaveFileName;
Q_SIGNALS:


protected Q_SLOTS:
    void slot_request_save();
    void slot_request_load();

    void slot_data_changed();

    void slot_toHere();
    void slot_add_before();
    void slot_add_below();

    void slot_delete();

    void slot_up();
    void slot_down();

    void slot_expandAll();
    void slot_collapseAll();

    void slot_homing();

    void slot_resize();

    void slot_post_save_timeout();

    void slot_customContextMenuRequested(const QPoint &pos);
private slots:
    void on_toolExport_clicked();
    void on_toolImport_clicked();

    void on_toolOpen_clicked();
    void on_toolSaveAs_clicked();

    void on_toolUp_clicked();
    void on_toolDown_clicked();
    void on_toolDel_clicked();
    void on_toolClr_clicked();
    void on_toolInsert_clicked();
    void on_view_clicked(const QModelIndex &index);

};

}

#endif // ACTIONTABLE_H
