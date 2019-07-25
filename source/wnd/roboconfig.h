#ifndef ROBO_CONFIG_H
#define ROBO_CONFIG_H

#include <QtWidgets>
//#include "xrobo.h"
#include "megainterface.h"
#include "MegaGateway.h"

#include "syspref.h"

#include "../plugin/model/megaloglistmodel.h"
#include "../plugin/plugin/xplugin.h"

namespace Ui {
class RoboConfig;
}

class RoboConfig : public QWidget
{
    Q_OBJECT

public:
    static QString extractIp( const QString &ip );

public:
    explicit RoboConfig(QWidget *parent = 0);
    ~RoboConfig();

    void retranslateUi();
    void userRoleChanged();

signals:
    void signal_focus_in( const QString &model, const QString &help );

    void signal_plugins_changed();
    void signal_plugins_operable( bool b );
    void signal_plugins_online( bool b );

    void signal_request_sysOpPanel();
    void signal_save_sysPref();
protected:
    void setupUi();


public:
    void attachOpDock( QDockWidget *pDock );
    void attachLogModel( MegaLogListModel *pModel );
    void attachSysPref( SysPara *pPref );
    void attachConnectWidget( QAction *pAction );

    void postStartup();

    void cancelBgWorking();

    QTreeWidgetItem *rootItem();
    QStackedWidget *stackWidget();

    bool downloadVisible();
    bool resetVisible();

protected:
    void stackPageChange( QTreeWidgetItem *current, QTreeWidgetItem *previous );
    void panelPageChange( QTreeWidgetItem *current, QTreeWidgetItem *previous );

    int createRobot( const QStringList &strInfos );

    //! remove the plugin
    void removePlugin( XPlugin *plugin );
    void addPlugin( XPlugin *plugin );

    XPlugin* findPlugin( const QString &_addr,
                     const QString &_model,
                     const QString &_sn );

    int  setReset();

    bool pluginsConnectState();
    void pluginsOpen();
    void plginsClose();

    void pluginsStop();
    void pluginHome();

public slots:
    void slotDownload();
    void slotUpload();
    void slotStore();
    void slotSync();

    void slotSearch();

    void slotConnect();

    void slot_plugins_stop();
    void slot_plugin_home();

private slots:
    void on_treeWidget_currentItemChanged(QTreeWidgetItem *current,
                                          QTreeWidgetItem *previous);
    void on_buttonBox_clicked(QAbstractButton *button);

    void slotAddNewRobot( const QStringList &strDevInfo );

    void slotShowContextmenu(const QPoint &pos);

    void slotShowContextPrj( const QPoint &pos );
    void slotShowContextPlugin( const QPoint &pos );

    void slotActionDelAll();

    void slotActionOpen();
    void slotActionClose();

    void slotActionHome();

    void slotActionDelete();

//    void slotActionUpdate();

//    void slotStoreEnd(int val);

    void slot_plugins_changed();
    void slot_plugin_setting_changed( XSetting setting );

    void on_stackedWidget_currentChanged(int arg1);

private:
    Ui::RoboConfig *ui;

    MegaInterface *m_megaSerachWidget;
    QDockWidget *m_pOpDock;
    MegaLogListModel *m_pLogModel;
    SysPara *m_pPref;

    QAction *m_pConnAction;

    QMenu *m_pRoboContextMenu;
    QAction *m_pActionOpen, *m_pActionClose;
    QAction *m_pActionHome;
    QAction *m_pActionDelete;

    QMenu *m_pProjectContextMenu;
    QAction *m_pActionDelAll;

    //! info
    XPlugin *m_pCurPlugin;
    QTreeWidgetItem *m_pCurTreeItem;

    QTreeWidgetItem *m_pRootNode;
    QList<XPlugin*> mPluginList;
public:
    XPlugin* currentXPlugin() const { return m_pCurPlugin; }
};

#endif // H2CONFIG_H
