#include "maintainpage.h"
#include "ui_maintainpage.h"

#include "t4.h"
#include "MegaGateway.h"
#include "updatedialog.h"

#include "../../wnd/changedpw.h"

namespace mrx_t4 {
MaintainPage::MaintainPage(QWidget *parent) :
    XPage(parent),
    ui(new Ui::MaintainPage)
{
    ui->setupUi(this);

    //! create movie
    m_pDemoMovie = new QMovie( this );
    m_pDemoMovie->setCacheMode( QMovie::CacheAll );

    setContextHelp("maintain");

    on_cmbDemo_currentIndexChanged( ui->cmbDemo->currentIndex() );
}

MaintainPage::~MaintainPage()
{
    delete ui;
}

void MaintainPage::enterMission()
{ setOperAble( false ) ;}
void MaintainPage::exitMission()
{ setOperAble( true ); }

void MaintainPage::retranslateUi()
{
    ui->retranslateUi( this );
}

void MaintainPage::setOperAble( bool b )
{
    //! for each page operate able
    setOpened( b );
}

void MaintainPage::setOnLine( bool b )
{
    //! demo tab disabled
    ui->tab_3->setEnabled( b );
}

void MaintainPage::setOpened( bool b )
{
    //! for each page operate
    for ( int i = 0; i < ui->tabWidget->count(); i++ )
    {
        ui->tabWidget->widget(i)->setEnabled( b );
    }

    logDbg()<<b;
}

void MaintainPage::updateRole()
{
    bool bAdmin = m_pPlugin->isAdmin();

    ui->btnResetPw->setVisible( bAdmin );
    ui->cmbRstUser->setVisible( bAdmin );
    ui->btnBuild->setVisible( bAdmin );
}

void MaintainPage::updateWorkingRole( int wRole )
{
    if ( wRole == XPluginIntf::working_mrq_corrupted )
    {
        //! enabled at first
        setEnabled( true );

        //! \note only the update can be used
        ui->tab_2->setEnabled( false );
        ui->tab_3->setEnabled( false );
        ui->tab_4->setEnabled( false );
        ui->tab_5->setEnabled( false );

        ui->btnHistory->setEnabled( false );
        ui->btnFold->setEnabled( false );
        ui->btnReset->setEnabled( false );
        ui->btnBuild->setEnabled( false );
    }
    else
    { XPage::updateWorkingRole( wRole); }
}

void MaintainPage::updateUi()
{
    ui->chkAutoLogin->setChecked( m_pPlugin->isAutoLogin() );
}

void MaintainPage::on_cmbDemo_currentIndexChanged(int index)
{
    begin_page_log1(index);
    end_page_log();
    //! set the demo info

    QStringList strList;
    strList<<tr("Demo1:\n"
                "* drag and drop in two points.\n"
                )
            <<tr("Demo2:\n"
                 "* Moving in the working space.\n"
                 "* It is used in the factory test procedure.\n"
                  );
//            <<tr("Demo3:\n"
//             "* Draw the logo.\n"
//             "* Draw \"MEGAROBO\".\n"
//              );

    if ( index < strList.size() )
    {
        ui->txtDemoInfo->setText( strList.at(index) );
    }
    else
    {
        ui->txtDemoInfo->setText( tr("No demo descripton") );
    }

    m_pDemoMovie->stop();
    ui->label_3->clear();

    //! \todo record the gif
    if ( index == 0 )
    { m_pDemoMovie->setFileName( ":/res/image/t4/ttt.gif" ); }
    else
    { m_pDemoMovie->setFileName( ":/res/image/t4/ttt2.gif" ); }

    ui->label_3->setMovie( m_pDemoMovie );
    m_pDemoMovie->start();

}

void MaintainPage::on_btnDemo_clicked()
{
    begin_page_log();
    end_page_log();

    Q_ASSERT( NULL != m_pPlugin );

    int ret = m_pPlugin->startDemo( ui->cmbDemo->currentIndex() );

    if ( ret != 0 )
    {
        sysPrompt( tr("Demo fail"), 0 );
    }
}

void MaintainPage::on_btnRestart_clicked()
{
    begin_page_log();
    end_page_log();

    if( msgBox_Warning_ok( tr("Warning"), tr("Confirm Reboot?") ) )
    { m_pPlugin->reboot(); }
}

void MaintainPage::on_btnShutDown_clicked()
{
    begin_page_log();
    end_page_log();

    if(msgBox_Warning_ok( tr("Warning"), tr("Confirm PowerOff?") ))
    { m_pPlugin->powerOff(); }
}

void MaintainPage::on_btnReset_clicked()
{
    begin_page_log();
    end_page_log();

    if ( msgBox_Warning_ok( tr("Warning"), tr("Confirm Reset?") ))
    { m_pPlugin->rst(); }
}

void MaintainPage::on_btnUpdate_clicked()
{
    begin_page_log();
    end_page_log();

    UpdateDialog w;
    w.attatchPlugin(m_pPlugin);
    w.exec();
}

void MaintainPage::on_btnHistory_clicked()
{
    begin_page_log();
    end_page_log();

    //! \todo need the file size api
    QByteArray ary;
    ary.reserve( 1024 * 1024 );

    int ret;
    //! read the history from remote and show
    ret = mrgStorageReadFile( m_pPlugin->deviceVi(), 0,
                        (QString(mct_path) + "/" + "MRX-T4").toLatin1().data(),
                        update_file_name,
                        (quint8*)ary.data() );
    if ( ret <= 0 )
    {
        sysError(tr("Read File Fail"));
        return;
    }

    ary.resize( ret );

    //! write
    QString fileName = QDir::tempPath() + "/" + update_file_name;
    QFile file( fileName );
    if ( file.open(QIODevice::WriteOnly ) )
    {
        file.write( ary );
        file.close();
    }
    else
    { return; }

    //! show doc
    QStringList args;
    QString str;
    str = fileName;
    str.replace("/","\\");
    args<<str;

    //! \todo linux
    QProcess::execute( "explorer.exe", args );
}

void MaintainPage::on_btnFold_clicked()
{
    begin_page_log();
    end_page_log();

    m_pPlugin->fold();
}

void MaintainPage::on_btnBackup_clicked()
{
    begin_page_log();
    end_page_log();

    FileManager manager;
    manager.attachPlugin(m_pPlugin);
    manager.setPath( m_pPlugin->selfPath()+"/backup/" );
    manager.setMode( BACKUP );
    if( manager.exec() == QDialog::Accepted ){
    }else{ return; }

    QString str = manager.strResult();
    int ret;
    QString cmd, dstPath;
    do{
        dstPath = m_pPlugin->selfPath() + "/backup/" + str;
        cmd = "mkdir -p " + dstPath;
        ret = mrgSystemRunCmd( m_pPlugin->deviceVi(), cmd.toLocal8Bit().data(), 0 );
        if ( ret != 0 )
        { ret = -1; break; }

        //! copy the data in
        cmd = "cp " + m_pPlugin->selfPath() + "/*.xml " + dstPath;
        ret = mrgSystemRunCmd( m_pPlugin->deviceVi(), cmd.toLocal8Bit().data(), 0 );
        if ( ret != 0 )
        { ret = -1;break; }

        cmd = "cp " + m_pPlugin->selfPath() + "/*.mrp " + dstPath;
        ret = mrgSystemRunCmd( m_pPlugin->deviceVi(), cmd.toLocal8Bit().data(), 0 );
        if ( ret != 0 )
        { ret = -1;break; }

        //! copy the log
        cmd = "cp -r /home/megarobo/MRH-T/log " + dstPath + "/log";
        ret = mrgSystemRunCmd( m_pPlugin->deviceVi(), cmd.toLocal8Bit().data(), 0 );
        if ( ret != 0 )
        { ret = -1;break; }

        //! write the description
        ret = mrgStorageWriteFile( m_pPlugin->deviceVi(),
                                   0,
                                   dstPath.toLocal8Bit().data(),
                                   "description",
                                   (quint8*)str.toLocal8Bit().data(),
                                   str.length()
                                   );
        if ( ret != 0 )
        { ret = -1;break; }
    }while(0);

    if( ret == -1){
        cmd = "rm -rf " + dstPath;
        ret = mrgSystemRunCmd( m_pPlugin->deviceVi(), cmd.toLatin1().data(), 0 );
        sysError(tr("Backup Fail"));

    }else{
        sysInfo(tr("Backup Complete"));
    }
}

void MaintainPage::on_btnClearBackup_clicked()
{
    begin_page_log();
    end_page_log();

    int ret = -1;
    if( msgBox_Warning_ok( tr("Warning"), tr("Confirm Clear?") ) )
    {
        QString cmd;

        cmd = "rm -rf " + m_pPlugin->selfPath() + "/backup";
        ret = mrgSystemRunCmd( m_pPlugin->deviceVi(), cmd.toLatin1().data(), 0 );
    }else{ return; }

    if ( ret != 0 )
    {
        sysPrompt( tr("Clear fail") );
    }
    else
    {
        sysPrompt( tr("Clear completed"),0 );
    }
}

void MaintainPage::on_cmbUser_currentIndexChanged(int index)
{
    begin_page_log1( index );
    end_page_log();

    //! is equal
    if ( index == (int)m_pPlugin->userRole() )
    { return; }

    bool bOk;
    QString adminPw;

    adminPw = m_pPlugin->getPw( XPluginIntf::user_admin, bOk );
    if ( bOk && adminPw.length() > 0 )
    {}
    else
    { return; }

    //! require
    //! to user
    if ( index == 0 )
    {
        m_pPlugin->setUserRole( (XPluginIntf::eUserRole)index );
    }
    //! to admin
    else if ( index == 1 )
    {
        do
        {
            QString pw;
            pw = QInputDialog::getText( this, tr("Password"), tr("Password"), QLineEdit::Password, QString(), &bOk );

            if ( bOk && adminPw == pw )
            {
                m_pPlugin->setUserRole( (XPluginIntf::eUserRole)index );
                break;
            }
            if ( bOk )
            {
                QMessageBox::critical( this, tr("Error"), tr("Invalid password") );
            }
            ui->cmbUser->setCurrentIndex( 0 );
        }while( 0 );
    }
}

void MaintainPage::on_btnChange_clicked()
{
    begin_page_log();
    end_page_log();

    ChangedPw changePw;

    if ( QDialog::Accepted != changePw.exec() )
    { return; }

    //! check pw
    bool bOk;
    if ( changePw.getOldPw() == m_pPlugin->getPw( (XPluginIntf::eUserRole)ui->cmbUser->currentIndex(), bOk) && bOk )
    {}
    else
    {
        QMessageBox::critical( this, tr("Error"), tr("Invalid password") );
        return;
    }

    //! check the pw
    m_pPlugin->setPw( (XPluginIntf::eUserRole)ui->cmbUser->currentIndex(),
                      changePw.getNewPw() );
}

//! \note only for admin
void MaintainPage::on_btnResetPw_clicked()
{
    begin_page_log();
    end_page_log();

    bool bOk;
    QString pw;
    pw = QInputDialog::getText( this, tr("Password"), tr("Admin Password"), QLineEdit::Password, QString(), &bOk );
    if ( bOk && pw.simplified().size() > 0 )
    {}
    else
    { return; }

    if ( pw == m_pPlugin->getPw( XPluginIntf::user_admin, bOk) && bOk )
    {}
    else
    {
        QMessageBox::critical( this, tr("Error"), tr("Invalid password") );
        return;
    }

    //! reset
    m_pPlugin->rstPw( (XPluginIntf::eUserRole)ui->cmbRstUser->currentIndex() );
}

void MaintainPage::on_chkAutoLogin_stateChanged(int arg1)
{
    begin_page_log();
    end_page_log();

    Q_ASSERT( m_pPlugin != NULL );
    m_pPlugin->setAutoLogin( ui->chkAutoLogin->isChecked() );
}

void MaintainPage::on_btnRestore_clicked()
{
    begin_page_log();
    end_page_log();

    FileManager manage;
    QString str;
    int ret;
    manage.setPath(m_pPlugin->selfPath()+"/backup/");
    manage.attachPlugin(m_pPlugin);
    manage.setMode(RESTORE);
    if( manage.exec() == QDialog::Accepted ){
    }else{return;}

    do{
        str = manage.strResult();
        QString sourcePath = m_pPlugin->selfPath()+"/backup/"+str;

        QString cmd = "cp " + sourcePath + "*.xml " + m_pPlugin->selfPath();
        ret = mrgSystemRunCmd(m_pPlugin->deviceVi(), cmd.toLocal8Bit().data(), 0);
        if( ret !=0 )
        { ret = -1; break; }

        cmd = "cp " + sourcePath + "*.mrp " + m_pPlugin->selfPath();
        ret = mrgSystemRunCmd(m_pPlugin->deviceVi(), cmd.toLocal8Bit().data(), 0);
        if(ret !=0)
        { ret = -1; break; }

        //! log ?

    }while(0);

    if(ret ==-1){
        sysError(tr("Restore Fail"));
    }else{
        sysInfo(tr("Restore Complete"));
    }
}

void MaintainPage::on_btnExport_clicked()
{
    begin_page_log();
    end_page_log();

    int ret;
    FileManager manager;
    manager.setMode(EXPORT);
    manager.attachPlugin(m_pPlugin);
    manager.setPath(m_pPlugin->selfPath()+"/backup/");
    if(manager.exec() == QDialog::Accepted){

    }else{ return; }

    QString str = manager.strResult();

    QString sourceDir = m_pPlugin->selfPath()+"/backup/"+str;
    QString distDir = QFileDialog::getExistingDirectory(this,tr("Export") );
    if(distDir.isEmpty())   return;

    QDir dir;

    //! check exist
    if ( QDir( distDir+"/"+ str +"log" ).exists() )
    {
        if ( msgBox_Warning_ok( tr("Warning"), tr("Directory is not empty, overwrite?") ) )
        {}
        else
        { return; }
    }
    else
    {
        bool bOk = dir.mkpath( distDir+"/"+ str +"log" );
        if(bOk){
        }else{ sysInfo(tr("Mkdir Fail"),1);return; }
    }

    QStringList fileList;
    fileList << "config.xml" << "debug.xml" << "description"
             << "diagnosis.xml" << "MCT_motion.mrp" << "password.xml";

    foreach (QString s, fileList) {
        QByteArray ba;
        ret = mrgStorageGetFileSize( m_pPlugin->deviceVi(), 0,
                                     sourceDir.toLocal8Bit().data(),
                                     s.toLocal8Bit().data());
        if(ret<0){
            ret = -1;
            break;
        }

        ba.resize(ret);
        ret = mrgStorageReadFile(m_pPlugin->deviceVi(), 0,
                                 sourceDir.toLocal8Bit().data(),
                                 s.toLocal8Bit().data(),
                                 (quint8*)ba.data());
        if( ret <0 ){logDbg()<<s<<ret;
            ret = -1;
            break;
        }

        //! write
        QFile f(distDir+"/"+str+s);
        if(!f.open(QIODevice::WriteOnly)){
            sysInfo(tr("Open Fail"),1);
            ret = -1;
            break;
        }
        f.write(ba);
        f.close();
    }

    //! log
    char Buf[4096]="";
    int iLen = sizeof(Buf);
    ret = mrgStorageDirectoryEnum(m_pPlugin->deviceVi(),0,
                            (sourceDir+"log/").toLocal8Bit().data(),
                            Buf,
                            &iLen);
    if(ret <0){ sysInfo(tr("Enum log fail"));return; }

    QStringList logList = QString(Buf).split("\n", QString::SkipEmptyParts);
    foreach (QString l, logList) {
        QByteArray ba;
        ret = mrgStorageGetFileSize(m_pPlugin->deviceVi(), 0,
                                    (sourceDir+"log/").toLocal8Bit().data(),
                                     l.toLocal8Bit().data());
        if(ret<0){
            break;
        }

        ba.resize(ret);
        ret = mrgStorageReadFile(m_pPlugin->deviceVi(), 0,
                                 (sourceDir+"log/").toLocal8Bit().data(),
                                 l.toLocal8Bit().data(),
                                 (quint8*)ba.data());
        if( ret <0 ){logDbg()<<l<<ret;
            break;
        }

        //! write
        QFile f(distDir+"/"+str+"log/"+l);
        if(!f.open(QIODevice::WriteOnly)){
            sysInfo(tr("Open Fail"),1);
            break;
        }
        f.write(ba);
        f.close();
    }
}

void MaintainPage::on_btnBuild_clicked()
{
    begin_page_log();
    end_page_log();

    int ret;
    ret = mrgSystemSetMRQConfig( m_pPlugin->deviceVi(),
                                 m_pPlugin->SoftVer_MRQ().toLatin1().data(),
                                 m_pPlugin->SN_MRQ().toLatin1().data() );
    if ( ret != 0 )
    { sysPrompt( "Build fail", 2 );  }
    else
    { sysPrompt( "Build complete", 0 ); }
}

}

