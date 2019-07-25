#include <QFile>
#include <QAction>
#include <QKeySequence>
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "login.h"
#include "syspref.h"
#include "changedpw.h"
#include "spacewidget.h"

#undef QT_NO_DEBUG_OUTPUT

#define pref_file_path  QDir::homePath() + "/AppData/Roaming/mct"
#define pref_file_name  pref_file_path + "/mct_pref.xml"

//! + time
#define log_file_path   pref_file_path + "/log"
#define log_file_name   log_file_path + "/" + QDateTime::currentDateTime().toString("yyyy_M_d")+".dat"

MainWindow *MainWindow::_pBackendProxy = NULL;

void MainWindow::requestLogout( const QString &str, LogStr::eLogLevel lev, int oHandle )
{
    if( NULL == MainWindow::_pBackendProxy )
    { return; }

    MainWindow::_pBackendProxy->emit_logout( str, lev, oHandle );
}

void MainWindow::requestProgress( const QString &info, bool b, int now, int mi, int ma  )
{
    if( NULL == MainWindow::_pBackendProxy )
    { return; }

    MainWindow::_pBackendProxy->emit_progress( info, b, now, mi, ma );
}

void MainWindow::requestPrompt( const QString &info, int lev )
{
    if( NULL == MainWindow::_pBackendProxy )
    { return; }

    MainWindow::_pBackendProxy->emit_prompt( info, lev );
}

void MainWindow::showStatus( const QString str)
{
    if( NULL == MainWindow::_pBackendProxy )
    { return; }

    MainWindow::_pBackendProxy->emit_status(str);
}

int MainWindow::langIndex()
{
    if( NULL == MainWindow::_pBackendProxy )
    { return 0; }

    return MainWindow::_pBackendProxy->languageIndex();
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

//    mLogInRet = QDialog::Accepted;

    m_pLabStatus = NULL;
    m_pLabMctVer = NULL;
    m_pLabConVer = NULL;
    m_roboConfig = NULL;
    m_pDockOps   = NULL;

    m_pDockHelp  = NULL;
    m_pHelpPanel = NULL;
    m_pProgress  = NULL;
    m_pPrompt = NULL;

    m_pSysLogout = NULL;

    //! default
    setupWorkArea();

    setupMenu();

    setupToolBar();

    setupStatusBar();

    buildConnection();

    loadConfig();

    //! register the proxy
    MainWindow::_pBackendProxy = this;
}

MainWindow::~MainWindow()
{
    delete ui;

    if ( NULL != m_pPrompt )
    { m_pPrompt->close(); }

    if ( NULL != m_pHelpPanel )
    { delete m_pHelpPanel; }

    if ( NULL != m_pProgress )
    { delete m_pProgress; }
}

void MainWindow::setupWorkArea()
{
    //! pref
    m_roboConfig = new RoboConfig(this);
    ui->centralWidget->insertTab( 0, m_roboConfig, tr("Pref") );

    //! connect

    connect( m_roboConfig, SIGNAL(signal_plugins_operable(bool)),
             this, SLOT(slot_plugin_operable(bool)) );
    connect( m_roboConfig, SIGNAL(signal_plugins_online(bool)),
             this, SLOT(slot_plugin_online(bool)) );
    connect( m_roboConfig, SIGNAL(signal_request_sysOpPanel()),
             this, SLOT(slot_request_sysOpPanel()) );
    connect( m_roboConfig, SIGNAL(signal_save_sysPref()),
             this, SLOT(slot_save_sysPref()) );

    //! dock
    m_pDockOps = new QDockWidget( tr("Ops"), this );
    m_pDockOps->setAllowedAreas(  Qt::BottomDockWidgetArea );

//    m_pDockOps->setFeatures( QDockWidget::DockWidgetVerticalTitleBar | m_pDockOps->features() );
    addDockWidget( Qt::BottomDockWidgetArea, m_pDockOps );

    m_roboConfig->attachOpDock( m_pDockOps );
    m_roboConfig->attachLogModel( &mLogModel );
    m_roboConfig->attachSysPref( &mPref );
    m_roboConfig->attachConnectWidget( ui->actionConnect );

    m_pSysLogout = new SysLogout( this );
    m_pDockOps->setWidget( m_pSysLogout );

    m_pSysLogout->attachLogModel( &mLogModel );

    ui->menuView->addAction( m_pDockOps->toggleViewAction() );

    //! help
    m_pDockHelp = new QDockWidget( tr("Help"), this  );
    m_pDockHelp->setAllowedAreas(  Qt::RightDockWidgetArea );
    addDockWidget( Qt::RightDockWidgetArea, m_pDockHelp );

    m_pHelpPanel = new HelpPanel();
    m_pDockHelp->setWidget( m_pHelpPanel );    

    m_pHelpAction = m_pDockHelp->toggleViewAction();
    ui->menuHelp->addAction( m_pHelpAction );
    m_pHelpAction->setText( tr("Show Help") );
    m_pHelpAction->setShortcut( QKeySequence("Ctrl+H") );

    m_pDockHelp->hide();
}

void MainWindow::setupMenu()
{
    QActionGroup * langGroup = new QActionGroup( ui->menuView );
    m_pChAction = langGroup->addAction( tr("Chinese") );
    m_pChAction->setCheckable( true );
    m_pEnAction = langGroup->addAction( tr("English") );
    m_pEnAction->setCheckable( true );
    m_pTrChAction = langGroup->addAction( tr("Traditional Chinese") );
    m_pTrChAction->setCheckable( true );
    m_pTrChAction->setVisible(false);

    QActionGroup * styleGroup = new QActionGroup(ui->menuView);
    m_pMegaAction = styleGroup->addAction( tr("MEGAROBO") );
    m_pMegaAction->setCheckable( true );
//    m_pClasAction = styleGroup->addAction( tr("Classic") );
//    m_pClasAction->setCheckable( true );
    m_pSystemAction = styleGroup->addAction( tr("System") );
    m_pSystemAction->setCheckable( true );

    ui->menuView->addSection( tr("Language") );
    ui->menuView->addActions( langGroup->actions() );

    ui->menuView->addSection( tr("Style") );
    ui->menuView->addActions( styleGroup->actions() );
}

void MainWindow::setupToolBar()
{
    ui->mainToolBar->addAction( ui->actionDownload );
    ui->mainToolBar->addAction( ui->actionUpload );
//    ui->mainToolBar->addAction( ui->actionStore );
//    ui->mainToolBar->addAction( ui->actionSync );
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addAction( ui->actionSearch );
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addAction( ui->actionConnect );

//    QWidget *pWidget;
//    pWidget->setLayout();
//    ui->mainToolBar->addWidget( new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum)
//                                );

    SpaceWidget *pSpac = new SpaceWidget();
    ui->mainToolBar->addWidget( pSpac );
    ui->mainToolBar->addAction( ui->actionHome );

    ui->mainToolBar->addSeparator();
    m_pStopWidget = new StopWidget();
    ui->mainToolBar->addWidget( m_pStopWidget );

//    ui->mainToolBar->addAction( ui->actionStop );
//    ui->mainToolBar->addAction( ui->actionIP );

    //! \note view action
    ui->menuView->addSeparator();
    ui->menuView->addAction( ui->mainToolBar->toggleViewAction() );

#ifdef _WIN32
    //! windows禁用这些功能
    ui->actionReboot->setVisible(false);
    ui->actionPoweroff->setVisible(false);
    ui->actionWifi->setVisible(false);
#endif

}

void MainWindow::setupStatusBar()
{
    m_pLabStatus = new QLabel(tr("MEGAROBO Configuration Tool"));
    m_pLabMctVer = new QLabel( QString("Version: %1  ").arg( qApp->applicationVersion() ) );
    m_pLabConVer = new QLabel( );

    ui->statusBar->insertWidget( 0, m_pLabStatus, 1 );
    ui->statusBar->insertWidget( 1, m_pLabMctVer, 0 );
    ui->statusBar->insertWidget( 2, m_pLabConVer, 0 );
}

void MainWindow::buildConnection()
{
    connect( this, SIGNAL(signal_pref_changed()),
             this, SLOT(slot_save_sysPref()) );

    connect( m_roboConfig, SIGNAL(signal_focus_in( const QString &, const QString &)),
             this, SLOT(slot_focus_in(const QString &, const QString &)) );

    //! direct connect
    connect(ui->actionDownload,SIGNAL(triggered(bool)), m_roboConfig, SLOT(slotDownload()));
    connect(ui->actionUpload,SIGNAL(triggered(bool)), m_roboConfig, SLOT(slotUpload()));
    connect(ui->actionStore,SIGNAL(triggered(bool)), m_roboConfig, SLOT(slotStore()));
    connect(ui->actionSync,SIGNAL(triggered(bool)), m_roboConfig, SLOT(slotSync()));
    connect(ui->actionSearch,SIGNAL(triggered(bool)), m_roboConfig, SLOT(slotSearch()));
    connect(ui->actionConnect,SIGNAL(triggered(bool)), m_roboConfig, SLOT(slotConnect()));

    connect( ui->actionHome, SIGNAL(triggered(bool)),
             m_roboConfig, SLOT(slot_plugin_home()) );

    //! default disable
    ui->actionDownload->setEnabled(false);
    ui->actionUpload->setEnabled(false);

    connect( m_pStopWidget, SIGNAL(signal_stop_clicked(bool)),
             this, SLOT(slot_emergency_stop()));

    //! connect menu
    connect( m_pEnAction, SIGNAL(triggered(bool)), this, SLOT(slot_lang_changed()) );
    connect( m_pChAction, SIGNAL(triggered(bool)), this, SLOT(slot_lang_changed()) );
    connect( m_pTrChAction, SIGNAL(triggered(bool)), this, SLOT(slot_lang_changed()) );

    connect( m_pMegaAction, SIGNAL(triggered(bool)), this, SLOT(slot_style_changed()) );
//    connect( m_pClasAction, SIGNAL(triggered(bool)), this, SLOT(slot_style_changed()) );
    connect( m_pSystemAction, SIGNAL(triggered(bool)), this, SLOT(slot_style_changed()) );


    //! logout
    connect( this, SIGNAL(signal_logout(const QString &,int, int )),
             this, SLOT(slot_logout(const QString &, int, int )),
             Qt::QueuedConnection );
    connect( m_pSysLogout, SIGNAL(signal_focus_in(const QString &,const QString &)),
             this, SLOT(slot_focus_in(const QString &, const QString &)) );

    //! status
    connect( this, SIGNAL(signal_status(const QString &)),
             this, SLOT(slot_status(const QString&)),
             Qt::QueuedConnection );

    //! progress
    connect( this, SIGNAL(signal_progress(const QString &,bool,int,int,int)),
             this, SLOT(slot_progress(const QString &,bool,int,int,int)),
             Qt::QueuedConnection );

    //! prompt
    connect( this, SIGNAL(signal_prompt(const QString&, int )),
             this, SLOT(slot_prompt(const QString &, int )),
             Qt::QueuedConnection );

}

void MainWindow::loadConfig()
{
    //! load pref
    loadPref();

    changeLanguage();

    changeStyle();

    //! post startup
    QTimer::singleShot( 0, this, SLOT(slot_post_startup()) );
}

void MainWindow::changeLanguage()
{
    //! set lang
    do
    {
        QLocale::Language lang;
        QLocale::Country area;
        if ( mPref.mLangIndex == LANG_EN )
        {
            lang = QLocale::English;
            area = QLocale::AnyCountry;
//            qmFile = ":/res/ts/qt_EN.qm";
        }
        else if ( mPref.mLangIndex == LANG_CN )
        {
            lang = QLocale::Chinese;
            area = QLocale::China;
//            qmFile = ":/res/ts/qt_CN.qm";
        }
        else if ( mPref.mLangIndex == LANG_TR_CN )
        {
            lang = QLocale::Chinese;
            area = QLocale::Taiwan;
        }
        else
        {
            lang = QLocale::AnyLanguage;
            area = QLocale::AnyCountry;
            sysError( tr("Invalid language"));
            return;
        }

        if ( mTranslator.isEmpty() )
        {}
        else
        { qApp->removeTranslator( &mTranslator ); }

        //! local
        QLocale local( lang, area );
        local.setDefault( QLocale(QLocale::English) );

        logDbg()<<local.uiLanguages();

        if ( mTranslator.load( local,
                              QLatin1String("mct"),
                              QLatin1String("_"),
                              qApp->applicationDirPath() + "/translate",
                              ".qm")
             && qApp->installTranslator(&mTranslator) )
        {
            m_roboConfig->retranslateUi();
        }
        else
        {
            QMessageBox::information( NULL,
                                    QObject::tr("Info"),
                                    QObject::tr("language loss"));
        }
    }while( 0 );
}

void MainWindow::changeStyle()
{
    if ( mPref.mStyleIndex == STYLE_MEGAROBO )
    {
        setUiStyle( qApp->applicationDirPath() + "/style/mega.qss" );
    }
    else if ( mPref.mStyleIndex == STYLE_CLASSIC )
    {
        setUiStyle( qApp->applicationDirPath() + "/style/classic.qss" );
    }
    else
    {
        qApp->setStyleSheet("");
    }
}

void MainWindow::setUiStyle(const QString &styleFile)
{
    if( ! QFile::exists(styleFile) )
    {
        qDebug() << "setStyleSheet file not exists!";
        sysError("setStyleSheet file not exists!" + styleFile);
        return;
    }

    QFile qss(styleFile);
    if( qss.open(QFile::ReadOnly) )
    {
        qApp->setStyleSheet(qss.readAll());
        qss.close();
    }
    else
    { sysError( tr("Style apply fail") );}
}

QString MainWindow::languageSuffix()
{
    //! english
    if ( mPref.mLangIndex == 0 )
    {}
    else if ( mPref.mLangIndex == 1 )
    { return "_ch"; }
    else
    {}

    return "";
}

void MainWindow::explorerDocFile( const QString &fileName )
{
    QStringList args;
    QString str;
    str = QCoreApplication::applicationDirPath()
            + QDir::separator()
            + QStringLiteral("doc")
            + QDir::separator()
            + fileName;
    str.replace("/","\\");
    args<<str;

    //! \todo linux
    QProcess::execute( "explorer.exe", args );
}

//void MainWindow::slot_role_changed()
//{
//    //! sys mode
//    if ( sysMode() == 0 )
//    { m_pLabConVer->setText( tr("Operator") ); }
//    else
//    { m_pLabConVer->setText( tr("Administrator") ); }
//}

void MainWindow::slot_plugin_operable( bool b )
{
    ui->actionDownload->setEnabled( b && m_roboConfig->downloadVisible() );
    ui->actionUpload->setEnabled( b );

    ui->actionSync->setEnabled( b && m_roboConfig->downloadVisible() );

    ui->actionStore->setEnabled( b );

    ui->actionHome->setVisible( b );
}

void MainWindow::slot_plugin_online( bool b )
{
    ui->actionHome->setEnabled( b );
}

void MainWindow::slot_request_sysOpPanel()
{
    Q_ASSERT( NULL != m_pDockOps );
    Q_ASSERT( NULL != m_pSysLogout );

    m_pDockOps->setWidget( m_pSysLogout );
}

void MainWindow::slot_save_sysPref()
{ savePref(); }

//! change the language
void MainWindow::slot_post_startup()
{
    adaptToUserRole();

    //! change the language
    if ( mPref.mLangIndex == 0 )
    { m_pEnAction->setChecked( true ); }
    else if ( mPref.mLangIndex == 1 )
    { m_pChAction->setChecked( true ); }
    else
    { m_pTrChAction->setChecked( true); }

    //! change the style
    if ( mPref.mStyleIndex == 0 )
    { m_pMegaAction->setChecked( true ); }
    else
    { m_pSystemAction->setChecked( true); }

    Q_ASSERT( NULL != m_roboConfig );
    m_roboConfig->postStartup();

    m_pHelpAction->triggered(true);

    //! max mize
    if ( mPref.mbAutoMax )
    { showMaximized(); }
}

void MainWindow::on_actionStop_triggered()
{
    slot_emergency_stop();
}

void MainWindow::on_actionAbout_triggered()
{
    aboutDlg dlg(this);
    dlg.exec();
}

void MainWindow::slot_lang_changed()
{
    if ( m_pEnAction->isChecked() )
    { mPref.mLangIndex = LANG_EN; }
    else if ( m_pChAction->isChecked() )
    { mPref.mLangIndex = LANG_CN; }
    else
    { mPref.mLangIndex = LANG_TR_CN; }

    changeLanguage();

    emit signal_pref_changed();
}
void MainWindow::slot_style_changed()
{
    if ( m_pMegaAction->isChecked() )
    {
        mPref.mStyleIndex = STYLE_MEGAROBO;
    }
//    else if ( m_pClasAction->isChecked() )
//    {
//        mPref.mStyleIndex = STYLE_CLASSIC;
//    }
    else
    {
        mPref.mStyleIndex = STYLE_SYSTEM;
    }

    changeStyle();

    emit signal_pref_changed();
}

void MainWindow::slot_emergency_stop()
{
    m_pStopWidget->setEnabled( false );
        QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
            m_roboConfig->slot_plugins_stop();
        QApplication::restoreOverrideCursor();
    m_pStopWidget->setEnabled( true );
}

void MainWindow::slot_logout( const QString &str, int lev, int outHandle )
{
    //! \note add the time stamp
    QString fullStr;

    fullStr = QDateTime::currentDateTime().toString( "yyyy/M/d h:m:s.z, ") + str;

    //! to console
    if ( outHandle == 0 )
    { mLogModel.append( fullStr, (LogStr::eLogLevel)lev ); }

    //! to file
    if ( outHandle == 1 )
    {
        QString lineStr = LogStr::toString( (LogStr::eLogLevel)lev ) + "," + fullStr + "\n";

        //! sure the path
        if ( assurePath( log_file_path ) != 0 )
        { return; }

        QFile file( log_file_name );
        if ( file.open( QIODevice::Append ) )
        {
            do
            {
                QTextCodec *pCoder = QTextCodec::codecForName("UTF-8");
                if ( NULL == pCoder )
                { break; }

                QTextEncoder *pEnc = pCoder->makeEncoder();
                if ( NULL == pEnc )
                { break; }

                file.write( pEnc->fromUnicode( lineStr ) );
                delete pEnc;
            }while( 0 );

            file.close();
        }
    }
}

void MainWindow::slot_status( const QString &str )
{
    if ( m_pLabStatus!=NULL )
    { m_pLabStatus->setText(str); }
}

void MainWindow::slot_progress( const QString &info, bool b, int now, int mi, int ma )
{
    if ( NULL == m_pProgress )
    {
        m_pProgress = new QProgressDialog();
        if ( NULL == m_pProgress )
        { return; }

        connect( m_pProgress, SIGNAL(canceled()),
                 this, SLOT(slot_progress_canceled()) );
    }
    else
    {}
    if ( b )
    {
        m_pProgress->setMinimum( mi );
        m_pProgress->setMaximum( ma );
        m_pProgress->setValue( now );
        m_pProgress->show();
    }
    else
    { m_pProgress->hide(); }
}

void MainWindow::slot_progress_canceled()
{
    m_roboConfig->cancelBgWorking();
}

void MainWindow::slot_prompt( const QString &info, int lev )
{
    //! prompt info
    if ( NULL == m_pPrompt )
    {
        m_pPrompt = new Prompt( this );
        if ( NULL == m_pPrompt )
        { return; }
    }

    //! show the item
    m_pPrompt->addInfo( info, lev );
    if ( !m_pPrompt->isVisible() )
    { m_pPrompt->show(); }
}

//! mrx-t4/info.html
void MainWindow::slot_focus_in( const QString &model,
                                const QString &name )
{
    mHelpModel = model;
    mHelpName = name;

    QString strName =  QApplication::applicationDirPath() + "/help/" + model + languageSuffix() + "/" + name + ".html";
    if ( name.length() <= 0 )
    { return; }

    if ( QFile::exists( strName ) )
    {}
    else
    {
        sysError( strName + " not exist");
        return;
    }

    //! help panel
    if ( m_pHelpPanel == NULL )
    { return; }

    m_pHelpPanel->setFile( strName );
    logDbg()<< strName;
}

void MainWindow::changeEvent( QEvent * event )
{
    QMainWindow::changeEvent( event );

    Q_ASSERT( NULL != event && NULL != ui );

    //! language change
    if (event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi( this );

        retranslateUi();

        //! help changed
        if ( mHelpModel.length() > 0 && mHelpName.length() > 0 )
        { slot_focus_in( mHelpModel, mHelpName); }

        //! \todo for each plugin and widgets
//        for ( int i = 0; i < ui->widget->count(); i++ )
//        {
//            Q_ASSERT( NULL != ui->widget->widget(i) );

//            qApp->postEvent( (modelView*)ui->widget->widget( i ),
//                          new QEvent( QEvent::LanguageChange )
//                          );
//        }
    }
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    //! calc the last ratio
    if ( m_pDockHelp->isVisible() && event->oldSize().width() > 0 )
    { resizeDocks( { m_pDockHelp }, { m_pHelpPanel->size().width() * event->size().width()/ event->oldSize().width() }, Qt::Horizontal  ); }

    int h;
    if ( m_pDockOps->isVisible() && event->oldSize().height() > 0 )
    {
        h = m_pSysLogout->size().height() * event->size().height()/ event->oldSize().height();
        if ( h < 420 )
        { h = 420; }
//        resizeDocks( { m_pDockOps }, { h }, Qt::Vertical  );
        logDbg()<<h;
    }
    else
    {}

    QWidget::resizeEvent(event);
}

void MainWindow::emit_logout( const QString &info, int level, int oHandle )
{
    emit signal_logout( info, level, oHandle );
}

void MainWindow::emit_status( const QString &str )
{ emit signal_status( str ); }

void MainWindow::emit_progress( const QString &info, bool b, int now, int mi, int ma )
{ emit signal_progress( info, b, now, mi, ma ); }

void MainWindow::emit_prompt( const QString &info, int lev )
{ emit signal_prompt( info, lev ); }

//int MainWindow::loginRet()
//{ return mLogInRet; }

int MainWindow::languageIndex()
{ return mPref.mLangIndex; }

void MainWindow::retranslateUi()
{
//    m_pClasAction->setText( tr("Classic") );
    m_pMegaAction->setText( tr("MEGAROBO") );
    m_pSystemAction->setText( tr("System") );

    m_pHelpAction->setText(tr("Show Help"));
    m_pDockHelp->setWindowTitle(tr("Help"));
    m_pDockOps->setWindowTitle(tr("Ops"));
    m_pSysLogout->reTranslate();
    m_pLabStatus->setText(tr("MEGAROBO Configuration Tool"));
}

void MainWindow::adaptToUserRole()
{
//    ui->actionChange_Password->setVisible( sysMode() == sysPara::e_sys_admin );
}

void MainWindow::savePref()
{
    int ret;

    //! create path
    ret = assurePath( pref_file_path );
    if ( ret != 0 )
    { return; }

    //! save file
    ret = mPref.save( pref_file_name );
    if ( ret != 0 )
    {
        sysError( tr("Pref save fail") );

        logDbg()<<pref_file_name;
    }
}
void MainWindow::loadPref()
{
    int ret;
    ret = mPref.load( pref_file_name );
    if ( ret != 0 )
    {
        sysError( tr("Pref load fail") );
    }
}

void MainWindow::on_actionPoweroff_triggered()
{
//    m_roboConfig->slotExit();
    QThread::msleep(1000);
#ifndef _WIN32
    system("poweroff");
#else
    system("shutdown -s -t 0");
#endif
}

void MainWindow::on_actionReboot_triggered()
{
//    m_roboConfig->slotExit();
    QThread::msleep(1000);
#ifndef _WIN32
    system("reboot");
#else
    system("shutdown -s -r 0");
#endif
}

void MainWindow::on_actionPref_triggered()
{
    //! get from the pref
    SysPref pref( mPref );

    if ( pref.exec() == QDialog::Accepted )
    {
        mPref = pref;
        savePref();
    }
    else
    {}
}

//void MainWindow::on_actionChange_Password_triggered()
//{
//    ChangedPw  changePwWnd;

//    if ( changePwWnd.exec() == QDialog::Accepted )
//    {
//        mPref.mPw = qCompress( changePwWnd.getPw().toLatin1() );
//        savePref();
//    }
//    else
//    {}
//}

//void MainWindow::on_actionSwitch_User_triggered()
//{
//    //! log in
//    LogIn logIn;
//    logIn.setPw( mPref.mPw );

//    int ret = logIn.exec();
//    if ( ret == QDialog::Accepted )
//    {
//        if ( logIn.getUserRole() == 0 )
//        { setSysMode( sysPara::e_sys_user ); }
//        else
//        { setSysMode( sysPara::e_sys_admin ); }

//        slot_role_changed();

//        m_roboConfig->userRoleChanged();
//    }
//    else
//    {
//        return;
//    }
//}

//! test used
#include "../plugin/factory/pluginfactory.h"
void MainWindow::on_actiontest_triggered()
{
//    XPlugin *plugin = PluginFactory::createPlugin( "mrx-t4","" );
//    if ( NULL == plugin )
//    { return; }

//    //! ops panel
//    QWidget *pWig = plugin->createOpsPanel( nullptr );
//    if ( NULL != pWig )
//    {
//        pWig->show();
//        m_pDockOps->setWidget( pWig );
//    }

//    //! pref pages
//    QTreeWidgetItem *pRoboRoot = plugin->createPrefPages( m_roboConfig->stackWidget() );
//    m_roboConfig->rootItem()->addChild( pRoboRoot );
}

//void MainWindow::on_actionErrant_triggered()
//{
//    explorerDocFile( "errant.txt" );
//}

void MainWindow::on_actionFAQ_triggered()
{
    explorerDocFile( "faq.txt");
}

void MainWindow::on_actionRead_me_triggered()
{
    explorerDocFile( "readme.txt" );
}



