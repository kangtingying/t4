#include "h2oppanel.h"
#include "ui_h2ops.h"

//#include "mydebug.h"

#include <QClipboard>
#include <QTime>
#include <QFileDialog>

//#include "mystd.h"
//#include "sysapi.h"
//#include "megamessagebox.h"
//#include "megaxml.h"

#define set_name( the, name )   the->setFocuHelpName( name );\
                                mSubTabs.append( the );

namespace mrx_h2 {

H2OpPanel::H2OpPanel(QAbstractListModel *pModel, QWidget *parent) :
    XPage(parent),
    ui(new Ui::H2Ops)
{
    ui->setupUi(this);

//    mViHandle = 0;
//    mDeviceName = 0;
//    mRobotName = 0;
//    m_strDevInfo = "";

//    m_recordNumber = -1;
//    m_isDebugRunFlag = false;
//    m_isHomgingRunFlag = false;
//    m_isPrjZeroRunFlag = false;


//    set_name( ui->tab_LogOut,   "tab_LogOut");
//    set_name( ui->tab_Operate,  "tab_Operate");
//    set_name( ui->tab_DigitalIO,"tab_DigitalIO");
//    set_name( ui->tab_Homing,   "tab_Homing");
//    set_name( ui->tab_Manual,   "tab_Manual");
//    set_name( ui->tab_Monitor,  "tab_Monitor");
//    set_name( ui->tab_Debug,    "tab_Debug");
//    set_name( ui->tab_Diagnosis,"tab_Diagnosis");

//    setupUi();

//    m_timerOpsAll = new QTimer;
//    m_timerOpsAll->setInterval(2000);

//    m_timerOpsOpreate = new QTimer;
//    m_timerOpsOpreate->setInterval(500);

//    m_timerOpsIO      = new QTimer;
//    m_timerOpsIO->setInterval(500);

//    m_threadOpsHoming = NULL;
//    m_threadOpsPrjZero = NULL;
//    m_timerOpsHoming  = new QTimer;
//    m_timerOpsHoming->setInterval(500);

//    m_timerOpsManual  = new QTimer;
//    m_timerOpsManual->setInterval(500);

//    m_timerOpsMonitor = new QTimer;
//    m_timerOpsMonitor->setInterval(1000);

//    m_threadOpsDebug = NULL;
//    m_timerOpsDebug = new QTimer;
//    m_timerOpsDebug->setInterval(500);

//    buildConnection();
}

H2OpPanel::~H2OpPanel()
{
    delete ui;
}



}
