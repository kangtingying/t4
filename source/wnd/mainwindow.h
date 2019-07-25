#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <QDockWidget>

#include "syspara.h"

#include "helppanel.h"
#include "aboutdlg.h"
#include "roboconfig.h"
#include "stopwidget.h"

#include "syslogout.h"
#include "prompt.h"

#include "../plugin/model/megaloglistmodel.h"

#include "../sys/sysapi.h"

namespace Ui {
class MainWindow;
}

enum {
    LANG_EN = e_lang_en,
    LANG_CN = e_lang_cn,
    LANG_TR_CN = e_lang_tcn,
};

enum {
    STYLE_MEGAROBO,
    STYLE_CLASSIC,
    STYLE_SYSTEM,
};

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    static MainWindow *_pBackendProxy;
    static void requestLogout( const QString &str, LogStr::eLogLevel lev, int oHandle );
    static void requestProgress( const QString &info, bool b, int now, int mi=0, int ma = 100 );
    static void requestPrompt( const QString &info, int lev=2 );

    static void showStatus(const QString str);
    static int langIndex();
protected:
    virtual void changeEvent( QEvent *event );
    virtual void resizeEvent(QResizeEvent *event);

public:
    void emit_logout( const QString &info, int level, int oHandle );
    void emit_status( const QString &str );
    void emit_progress( const QString &info, bool b, int now, int mi, int ma );
    void emit_prompt( const QString &info, int );

public:
    int languageIndex();

protected:
    void retranslateUi();

    void adaptToUserRole();

    void savePref();
    void loadPref();

    void setupWorkArea();
    void setupMenu();
    void setupToolBar();
    void setupStatusBar();
    void buildConnection();
    void loadConfig();

    void changeLanguage();
    void changeStyle();
    void setUiStyle(const QString &styleFile);
    QString languageSuffix();

    void explorerDocFile( const QString &fileName );

signals:
    void signal_pref_changed();
    void signal_logout( const QString &str, int lev, int outHandle );
    void signal_status( const QString &str );

    void signal_progress( const QString &info, bool b, int now, int mi, int ma );
    void signal_prompt( const QString &info, int lev );

private slots:
//    void slot_role_changed();
    void slot_plugin_operable( bool b );
    void slot_plugin_online( bool b );

    void slot_request_sysOpPanel();
    void slot_save_sysPref();

    void slot_post_startup();

    void slot_lang_changed();
    void slot_style_changed();

    void slot_emergency_stop();

    //! outHandle: 0, console
    //!            1, log file
    void slot_logout( const QString &str, int lev, int outHandle );

    void slot_status(const QString &str);

    void slot_progress( const QString &info, bool b, int now, int mi, int ma );
    void slot_progress_canceled();

    void slot_prompt( const QString &info, int  );

    void slot_focus_in( const QString &model,
                        const QString &name );

    void on_actionStop_triggered();

    void on_actionAbout_triggered();

    void on_actionPoweroff_triggered();

    void on_actionReboot_triggered();

    void on_actionPref_triggered();

    void on_actiontest_triggered();

    void on_actionFAQ_triggered();

    void on_actionRead_me_triggered();



private:
    Ui::MainWindow *ui;

    StopWidget *m_pStopWidget;

    //! lang/style action
    QAction *m_pChAction, *m_pEnAction, *m_pTrChAction;
    QAction *m_pMegaAction, *m_pSystemAction;

    QLabel *m_pLabStatus, *m_pLabMctVer, *m_pLabConVer;

    QDockWidget *m_pDockOps;

    QProgressDialog *m_pProgress;
    Prompt *m_pPrompt;

    QDockWidget *m_pDockHelp;
    HelpPanel *m_pHelpPanel;
    QAction *m_pHelpAction;

    RoboConfig *m_roboConfig;
    SysLogout *m_pSysLogout;

    //! sys para
    SysPara mPref;

    //! common
    MegaLogListModel mLogModel;

    QTranslator mTranslator;

    QString mHelpModel, mHelpName;
};

#endif // MAINWINDOW_H
