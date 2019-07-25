#include "updatedialog.h"
#include "ui_updatedialog.h"
#include <QFileDialog>
#include <QDebug>
#include <QtConcurrent>
#include "../../sys/sysapi.h"
#include <QDir>
#include "../../device/assist.h"
#include <QStatusBar>
#include <QVBoxLayout>

#define MRH_UPDATE  "mrh.tar.gz"
#define MRQ_UPDATE_EXE "/MRQ_Update/MegaRobo_Update.exe"

//! file path
#define DIR_TEMP    QDir::tempPath()
#define DIR_TEMP_OUTPUT (DIR_TEMP + "/output")
#define MRQ_FILE    (DIR_TEMP + "/output/temp.dat")

//! ByteArray to int
static int byteArrayToInt(QByteArray arr)
{
    if (arr.size() < 4)
        return 0;

    int res = 0;

    // 小端模式
    res = arr.at(0) & 0x000000FF;
    res |= (arr.at(1) << 8) & 0x0000FF00;
    res |= (arr.at(2) << 16) & 0x00FF0000;
    res |= (arr.at(3) << 24) & 0xFF000000;

    return res;
}

UpdateDialog::UpdateDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    ui->progressBar->hide();

    m_mrqEntity = NULL;
    m_mrhEntity = NULL;
    pWorkThead = NULL;

    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled( false );
    ui->buttonBox->button( QDialogButtonBox::Cancel )->setEnabled( false );

    //! update thread

    pStatusBar = new QStatusBar(this);
    ui->gridLayout->addWidget(pStatusBar, 3, 0, 1, 1);
}

UpdateDialog::~UpdateDialog()
{
    if( pWorkThead != NULL ){
        pWorkThead->requestInterruption();
        pWorkThead->wait();
        delete pWorkThead;
    }

    delete ui;
}

void UpdateDialog::attatchPlugin(XPlugin *xp)
{
    m_pPlugin = xp;
    m_addr = m_pPlugin->addr();
    isAdmin = m_pPlugin->isAdmin();
}

void UpdateDialog::updateUi( int i )
{
    //! \todo ui

    QString str;

    switch (i)
    {
        case -1:
            str = tr("Update Backbord Error");
            break;
        case -2:
            str = tr("Net Error");
            break;
        case -3:
            str = tr("File Invalid");
            break;
        case -4:
            str = tr("Config Error");
            break;
        case -5:
            str = tr("Connect Fail");
            break;
        case -6:
            str = tr("The BOOT Version Is Old");
            break;
        case -7:
            str = tr("The MRQ File Is Invalid");
            break;
        case -8:
            str = tr("Cannot Get Ehe BOOT Version");
            break;
        case -9:
            str = tr("Reload Failed");
            break;
        case -10:
            str = tr("Get Software Version Error");
            break;
        case -11:
            str = tr("Arm File Check Error");
            break;
        case -12:
            str = tr("BOOT Data Download Failed");
            break;
        case -13:
            str = tr("Does Not Respond");
            break;
        case -14:
            str = tr("FPGA1 File Check Error");
            break;
        case -15:
            str = tr("Erase The FLASH Failed");
            break;
        case -16:
            str = tr("FPGA2 File Check Error");
            break;
        case -18:
            str = tr("Open MRH-T Failed");
            break;
        case 0:
            str = tr("Update Complete");
            ui->buttonBox->hide();
            break;
        case 1:
            str = tr("MRQ Begin Update");
            break;
        case 2:
            str = tr("MRH Begin Update");
            break;
        case 9:
            str = tr("MRQ Update Complete");
            break;
    }

    pStatusBar->showMessage( str );
}
void UpdateDialog::updateProgress( QString str )
{
    ui->progressBar->setValue( str.toInt() );
    ui->progressBar->show();
}

void UpdateDialog::on_buttonBox_clicked(QAbstractButton *button)
{
    begin_page_log();
    end_page_log();

    if((QPushButton*)(button) ==
        ui->buttonBox->button(QDialogButtonBox::Ok))
    {
        //! Warning message
        int ret = QMessageBox::warning(this, tr("Warning"),
                                       tr("Please do not power off\n"
                                          "during the upgrade."),
                                       QMessageBox::Ok | QMessageBox::Cancel
                                       );
        if( ret == QMessageBox::Ok ){

        }else{
            return;
        }

        m_pPlugin->close();

        button->setDisabled( true );
        ui->buttonBox->button( QDialogButtonBox::Cancel )->setEnabled( true );

        ui->lineEdit->setReadOnly( true );
        ui->toolButton->setDisabled( true );

        if( pWorkThead != NULL ){
            delete pWorkThead;
            pWorkThead == NULL;
        }
        pWorkThead = new MThead;
        connect( this, SIGNAL(changeTheadWorkMode(int)), pWorkThead, SLOT(switchWorkMode(int)) );
        connect( pWorkThead, SIGNAL( resultReady(int)), this, SLOT( updateUi( int ) ) );
        connect( pWorkThead, SIGNAL( resultReady(QString)), this, SLOT( updateProgress( QString ) ));
        pWorkThead->start();
        pWorkThead->setAddr( m_addr );
        pWorkThead->attachEntity_MRQ( m_mrqEntity );
        pWorkThead->attachEntity_MRH( m_mrhEntity );
        pWorkThead->attatchPlugin( m_pPlugin );
        changeTheadWorkMode(1);
    }else{
        if( pWorkThead != NULL ){
            pWorkThead->requestInterruption();
            pWorkThead->wait();
            delete pWorkThead;
            pWorkThead = NULL;
        }

        ui->lineEdit->setReadOnly( false );
        ui->toolButton->setDisabled( false );
        ui->lineEdit->clear();
        ui->progressBar->hide();
        pStatusBar->showMessage( tr("Cancle"),  5000);
        ui->buttonBox->button( QDialogButtonBox::Cancel )->setEnabled( false );
    }

}

//! return value:0 =; 1: !=;
int UpdateDialog::versionComparison( const QString &inVersion )
{
    //! MRX-T4_R0.0.0.1
    //! MRX-T4_M0.0.0.1

    int ret = 0;
    QString version, mrh, mrq, builTime;

    QByteArray ba;
    ba.reserve( 1024*1024 );
    ret = mrgStorageReadFile( m_pPlugin->deviceVi(), 0,
                        (QString(mct_path) + "/" + "MRX-T4").toLatin1().data(),
                        "update.xml",
                        (quint8*)ba.data() );
    ba.resize( ret );

    QXmlStreamReader reader( ba );
    while( reader.readNextStartElement() ){
        if( reader.name() == "data" ){
            while( reader.readNextStartElement() ){
                if( reader.name() == "block" ){
                    if( reader.attributes().hasAttribute("id") ){
                        qDebug() << reader.attributes().value("id").toString();
                        while(reader.readNextStartElement()){
                            if( reader.name() == "version" ){
                                version = reader.readElementText();
                            }else if( reader.name() == "mrh" ){
                                mrh = reader.readElementText();
                            }else if( reader.name() == "mrq" ){
                                mrq = reader.readElementText();
                            }else if( reader.name() == "build_time" ){
                                builTime = reader.readElementText();
                            }else{
                                reader.skipCurrentElement();
                            }
                        }
                        break;
                    }
                }else{
                    reader.skipCurrentElement();
                }
                break;
            }
        }else{
            reader.skipCurrentElement();
        }
    }
    //! \todo check version
    QStringList inList = inVersion.mid(8).split(".");
    QStringList tList = version.mid( 8 ).split(".");
    for( int i = 0; i < inList.size(); i++ ){
        if( inList.at( i ) != tList.at( i ) )
        return 1;
    }
    return 0;
}

void UpdateDialog::on_toolButton_clicked()
{
    begin_page_log();
    end_page_log();

    sPath = QFileDialog::getOpenFileName(this,
          tr("Open File"), NULL, tr("(*.upd)"));
    ui->lineEdit->setText(sPath);
}

void UpdateDialog::on_lineEdit_textChanged(const QString &s)
{
    pStatusBar->clearMessage();

    if( s.isEmpty() ){
        return;
    }

    QFile f(s);
    if( f.open(QIODevice::ReadOnly) ){

    }else{
        pStatusBar->showMessage( tr("Open Fail") );
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled( false );
        return;
    }

    QByteArray ba = f.readAll();

    if( parseUpdateFile( ba ) != 0){
        pStatusBar->showMessage( tr("Invalid File") );
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled( false );
    }else{
        if( !versionComparison( m_desc ) && !isAdmin){
            this->pStatusBar->showMessage(QString("Desc: %1, %2").arg(m_desc).arg(QString("Permission not allowed")));
            ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled( false );
        }else{
            this->pStatusBar->showMessage(QString("Desc: %1").arg(m_desc));
            ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(s.length()>0);
        }

    }

}

int UpdateDialog::openDevice()
{
    begin_page_log();
    end_page_log();

    int ret = 0;
    int vi = -1;
    do{
        //! device handle
        vi = mrgOpenGateWay( 1, m_addr.toLocal8Bit().data(), 2000);
        if( vi < 0){
            ret = -1;
            break;
        }

        int robotNames[128] = {0};
        ret = mrgGetRobotName(vi, robotNames);
        if(ret <=0){
            sysInfo("Get Robot Name Fail", 1);
            ret = -1;
            break;
        }
        m_robotID = robotNames[0];

        int deviceNames[128] = {0};
        //ret = mrgGetRobotDevice(m_vi, m_robotID, deviceNames);
        recvID = QString::number(1);

        //! info
        ret = loadRemoteInfo( vi );
        if ( ret != 0 )
        { break; }

    }while(0);
    if ( vi > 0 ){
        mrgCloseGateWay( vi );
    }

    return ret;
}

int UpdateDialog::loadRemoteInfo( int vi )
{
    QByteArray ary;
    ary.reserve( 1024 * 1024 );

    int ret = 0;

    //! read the history from remote and show
    ret = mrgStorageReadFile( vi, 0,
                        (QString(mct_path) + "/" + "MRX-T4").toLatin1().data(),
                        "update.xml",
                        (quint8*)ary.data() );
    if( ret  == 0 ){
        return 1;
    }
    ary.resize( ret );

    mRemoteVerionStream = ary;

    return 0;
}

int UpdateDialog::parseUpdateFile(QByteArray &in)
{
    if( in.isEmpty()){
        return -1;
    }

    int iDesLen =  byteArrayToInt( in.mid( 0, 4 ) );
    m_desc = QString::fromLocal8Bit( in.mid( 4, iDesLen ) );
    int iId = byteArrayToInt( in.mid( 4+iDesLen, 4 ) );
    if( iId != Entity::PACKAGE ){
        m_desc.clear();
        return -1;
    }
    int iSize = byteArrayToInt( in.mid( iDesLen+8, 4 ) );
    int iCheck = byteArrayToInt( in.mid( iDesLen+12, 4 ) );
    int iFormat = byteArrayToInt( in.mid( iDesLen+16, 4 ));
    int iSections = byteArrayToInt( in.mid( iDesLen+20, 4 ));
    QByteArray ba = in.mid( iDesLen+24, iSize );
    QByteArray t = qUncompress( ba );

    for( quint32 i=0; i< iSections; i++ )
    {
        Entity *e = new Entity(this);
        e->mDescLen = byteArrayToInt( t.mid(0, 4) );
        e->mDescription = QString::fromLocal8Bit( t.mid( 4, e->mDescLen ) );
        e->mId = byteArrayToInt(t.mid( 4+e->mDescLen, 4 ));
        e->mSize = byteArrayToInt(t.mid( e->mDescLen+8, 4 ));
        e->mCheck = byteArrayToInt(t.mid( e->mDescLen+12, 4 ));
        e->mFormat = byteArrayToInt(t.mid( e->mDescLen+16, 4 ));
        e->mSections = byteArrayToInt(t.mid( e->mDescLen+20, 4 ));
        e->mPayload = t.mid( e->mDescLen+24, e->mSize );

        if( e->mId == Entity::MRQ_ENTITY ){
            m_mrqEntity = e;
        }else if( e->mId == Entity::MRH_ENTITY ){
            m_mrhEntity = e;
        }else{}

        t = t.mid( e->mDescLen+24+e->mSize );
    }

    return 0;
}


//! Class MThead
MThead::MThead(QObject *parent):
    QThread(parent),
    pProc(NULL),
    iFlag(0),
    pEntity_MRQ(NULL),
    PEntity_MRH(NULL),
    iEndFlag(0)
{
    pSemaphore = new QSemaphore(1);
    vi = 0;
}

int MThead::generateDevUpdateFile()
{
    Q_ASSERT( pEntity_MRQ != NULL );

    QDir dir;
    if( !dir.mkpath( QString(DIR_TEMP_OUTPUT) ) )
    {
        return -1;
    }

    QFile f( MRQ_FILE );
    if( f.open(QIODevice::WriteOnly) ){

    }else{
        return -1;
    }

    f.write( pEntity_MRQ->mPayload );
    f.close();

    return 0;
}

int MThead::updateDevice()
{
    if( generateDevUpdateFile() != 0 ){
        return -1;
    }

    if( pProc != NULL ){
        delete pProc;
        pProc = NULL;
    }
    pProc = new QProcess;

    QStringList argument;

    argument << MRQ_FILE << m_addr <<QString::number(1);
    connect( pProc,
             SIGNAL(readyRead()),
             this,
             SLOT(parseStandOutput())
            );

    pProc->start( (qApp->applicationDirPath() + MRQ_UPDATE_EXE), argument );
    pProc->waitForStarted();

    do{
        localSleep( 100 );

        //! process complted
        if ( pProc->waitForFinished(0) )
        {
            break;
        }
        else
        {}
    }while( true );

    return 0;
}

int MThead::updateController()
{
    int ret = 0;

        vi = mrgOpenGateWay(1, m_addr.toLocal8Bit().data(), 2000);
        if( vi < 0 ){
            ret = -2;
        }

        do{
            if( PEntity_MRH->mPayload.isEmpty() ){
                ret = -3;
                break;
            }

            ret = mrgStorageWriteFile(vi, 0, (char *)"/media/usb0/",
                                      (char *)MRH_UPDATE,
                                      (unsigned char*)(PEntity_MRH->mPayload.data()),
                                      PEntity_MRH->mPayload.size());
            if(ret!=0){
                ret = -2;
                break;
            }

            emit resultReady( QString::number( 60 ) );

            QString strCmd = "tar xzvf /media/usb0/mrh.tar.gz -C /media/usb0/";
            ret = mrgSystemRunCmd(vi, strCmd.toLocal8Bit().data(), 0);
            if( ret !=0 ){
                ret = -3;
                break;
            }

            emit resultReady( QString::number( 70 ) );

            strCmd = "cp /media/usb0/update.sh /home/megarobo/MCT/MRX-T4/update.sh";
            ret = mrgSystemRunCmd(vi, strCmd.toLocal8Bit().data(), 0);
            if( ret !=0 ){
                ret = -3;
                break;
            }

            strCmd = "sh /home/megarobo/MCT/MRX-T4/update.sh";
            ret = mrgSystemRunCmd(vi, strCmd.toLocal8Bit().data(), 0);
            if(ret !=0){
                ret = -3;
                break;
            }

            emit resultReady( QString::number( 80 ) );

            ret = mrgSysUpdateFileStart(vi, (char *)"mrh.dat");
            if(ret !=0 ){
                ret = -1;
                break;
            }

            ret = mrgSystemSetMRQConfig( vi,
                                         pEntity_MRQ->mDescription.toLatin1().data(),
                                         pXPlugin->SN_MRQ().toLatin1().data() );
            if ( ret != 0 ){
                sysError( tr("Set MRQConfig Error"), e_out_log );
                ret = -4;
                break;
            }

            emit resultReady( QString::number( 100 ) );

        }while(0);

        QString str = "rm -rf /media/usb0/*";
        mrgSystemRunCmd(vi, str.toLocal8Bit().data(), 0);

        if( ret == 0 ){
            mrgSystemRunCmd( vi, (char *)"reboot", 0 );
        }

    mrgCloseGateWay(vi);

    return ret;
}

void MThead::attatchPlugin(XPlugin *xp)
{
    pXPlugin = xp;
}

void MThead::run()
{
    try{
        while( 1 )
        {
            if ( isInterruptionRequested() )
            { return; }

            if( iFlag )
            {
                iProgress = 0;

                emit resultReady(1);

                pSemaphore->acquire(1);
                int ret = updateDevice();

                //! wait
                if( pSemaphore->tryAcquire(1, 10000) ){

                }else{
                    pSemaphore->release(1);
                    goto SLEEP;
                }
                pSemaphore->release(1);

                if( iEndFlag == 0 || ret < 0 ){
                    pSemaphore->acquire(1);
                    iProgress = 0;
                    ret = updateDevice();
                    if( pSemaphore->tryAcquire(1, 10000) ){

                    }else{
                        pSemaphore->release(1);
                        goto SLEEP;
                    }
                    pSemaphore->release(1);
                }

                if( iEndFlag == 1 )
                {
                    emit resultReady(9);
                    emit resultReady(2);

                    ret = updateController();
                    emit resultReady( ret );
                    if( ret < 0 ){
                        ret = updateController();
                    }

                    emit resultReady( ret );

                    iEndFlag = 0;
                }
                iFlag = 0;
            }

            SLEEP:
            localSleep(500);
        }
    }
    catch( QException &e )
    {
        //! \todo free
        if( pProc )
        pProc->close();

        if( vi > 0 ){
            mrgCloseGateWay(vi);
            vi = 0;
        }
    }
    //delete pProc;
}
void MThead::slotReadyRead()
{
    QByteArray ba = pProc->readAll();
    qDebug() << ba;
    emit resultReady(QString(ba));
}

void MThead::switchWorkMode(int mode)
{
    iFlag = mode;
}

void MThead::parseStandOutput()
{
    iProgress += 1;
    QByteArray ba = pProc->readAll();
    //! \todo err code
    int ret = 99;
    if( ba.contains("Error:Cannot establish communication!") ){
        ret = -5;
    }else if( ba.contains( "Error:The BOOT version is very old" ) ){
        ret = -6;
    }else if( ba.contains( "Error:The update file is invalid" ) ){
        ret = -7;
    }else if( ba.contains( "Error:Cannot get the BOOT version" ) ){
        ret = -8;
    }else if( ba.contains( "Error:Reload failed" ) ){
        ret = -9;
    }else if( ba.contains( "Error:Cannot get the software version" ) ){
        ret = -10;
    }else if( ba.contains( "Error:ARM file has validation error" ) ){
        ret = -11;
    }else if( ba.contains( "Error:BOOT data download failed" ) ){
        ret = -12;
    }else if( ba.contains( "Error:Does not respond" ) ){
        ret = -13;
    }else if( ba.contains( "Error:FPGA1 file has validation" ) ){
        ret = -14;
    }else if( ba.contains( "Error:Erase the FLASH failed" ) ){
        ret = -15;
    }else if( ba.contains( "FPGA2 file has validation error" ) ){
        ret = -16;
    }else if( ba.contains( "Notify:Open MRH-T Failed" ) ){
        ret = -18;
    }else if( ba.contains( "Notify:Update Complete" ) ){
        iEndFlag = 1;
        iProgress = 55*4;   /* 55% */
        ret = 9;
    }else{
        ret = 1;
    }   

    if( ret != 1 ){
        pSemaphore->release(1);
    }

    emit resultReady(ret);
    emit resultReady( QString::number(iProgress/4) );
    //! percent
//    QRegExp rx("(\\d+)");
//    QString _str = QString( ba );
//    QStringList list;
//    int pos = 0;

//    while ((pos = rx.indexIn(_str, pos)) != -1) {
//        list << rx.cap(1);
//        pos += rx.matchedLength();
//    }
}

//! Class Entity
Entity::Entity(QObject *parent) : QObject(parent)
{

}
