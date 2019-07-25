#include "xpage.h"

#include "xplugin.h"
#include "xpluginworkingthread.h"
//#include "../../../source/sys/sysapi.h"
XPage::XPage(QWidget *parent) : QWidget(parent)
{
    mUri = "data";
    m_pPlugin = NULL;
    m_pPref = NULL;
    mbMissionWorking = false;

    mAttr = page_attr_unk;

    setFocusPolicy( Qt::WheelFocus );
}

bool XPage::event(QEvent *event)
{
    if ( event->type() == QEvent::FocusIn )
    {logDbg();
        focusInEvent( (QFocusEvent*)event );
    }
    else
    {}

    return QWidget::event( event );
}

void XPage::changeEvent(QEvent *event)
{
    if ( event->type() == QEvent::LanguageChange )
    {
        retranslateUi();
        event->accept();
    }
    else
    { QWidget::changeEvent( event ); }
}

void XPage::focusInEvent(QFocusEvent *event)
{
    showFocusHelp();
}

void XPage::retranslateUi()
{}

void XPage::onPluginChanged()
{}

void XPage::translateUi()
{
    retranslateUi();
}

void XPage::attachPlugin( XPlugin *pPlugin )
{
    Q_ASSERT( NULL != pPlugin );

    mUri = "data";
    m_pPlugin = pPlugin;

    //! virtual
    connectPlugin();

//    updateUi();

    onPluginChanged();
}
XPlugin *XPage::pulgin()
{ return m_pPlugin; }

void XPage::attachPref( SysPara *pPref )
{
    Q_ASSERT( NULL != pPref );
    m_pPref = pPref;
}
SysPara * XPage::pref()
{ return m_pPref; }

bool XPage::isQualified()
{ return true; }

XPage::PageAttr XPage::pageAttr()
{ return mAttr; }

void XPage::rst()
{}

void XPage::connectPlugin()
{
    Q_ASSERT( NULL != m_pPlugin );

    //! focus
    connect( this, SIGNAL(signal_focus_changed(const QString &,const QString &)),
             m_pPlugin, SIGNAL(signal_focus_changed(const QString &,const QString &)) );

    //! change
    connect( m_pPlugin, SIGNAL(signal_setting_changed( XSetting ) ),
             this, SLOT(slot_plugin_setting_changed( XSetting)) ) ;
    //! request save
    connect( this, SIGNAL(signal_request_save()),
             m_pPlugin, SLOT(slot_save_setting()) );

    //! plugin->pages
    connect( m_pPlugin, SIGNAL(signal_request_save()),
             this, SLOT(slot_request_save()) );
    connect( m_pPlugin, SIGNAL(signal_request_load()),
             this, SLOT(slot_request_load()) );

    //! \note only for mission
    connect( m_pPlugin->m_pMissionWorking, SIGNAL( signal_enter_working(WorkingApi *)),
             this, SLOT(slot_enter_mission(WorkingApi *)));
    connect( m_pPlugin->m_pMissionWorking, SIGNAL(signal_exit_working(WorkingApi *, int)),
             this, SLOT(slot_exit_mission(WorkingApi *, int)));
}

void XPage::attachWorkings()
{}

//! data -> ui
void XPage::updateUi()
{}

void XPage::updateData()
{}

void XPage::startup()
{}

void XPage::updateRole()
{}

void XPage::updateWorkingRole( int role )
{
    if ( role == XPluginIntf::working_unk )
    { setEnabled( false ); }
    else if ( role == XPluginIntf::working_disconnected )
    { setEnabled( false ); }
    else if ( role == XPluginIntf::working_normal )
    { setEnabled( true ); }
    else if ( role == XPluginIntf::working_mrq_corrupted )
    { setEnabled( false ); }
    else if ( role == XPluginIntf::working_device_not_ready )
    { setEnabled( false ); }
    else
    {}
}

//! exchange
int XPage::upload()
{ return 0; }
int XPage::download()
{ return 0; }
int XPage::diff()
{ return 0; }

void XPage::onSetting( XSetting setting )
{
    if ( setting.mSetting == XPage::e_setting_op_able )
    {
        check_para1();

        //! enable/disable
        setOperAble( setting.mPara1.toBool() );

        logDbg()<<mUri<<setting.mPara1.toBool();
    }
    else if ( setting.mSetting == XPage::e_setting_online )
    {
        check_para1();

        //! enable/disable
        setOnLine( setting.mPara1.toBool() );
    }
    else if ( setting.mSetting == XPage::e_setting_opened )
    {
        check_para1();

        //! enable/disable
        setOpened( setting.mPara1.toBool() );
    }
    else if ( setting.mSetting == XPage::e_setting_mission_working )
    {
        check_para1();

        mbMissionWorking = setting.mPara1.toBool();

        if ( mbMissionWorking )
        { enterMission(); }
        else
        { exitMission(); }
    }
    else if ( setting.mSetting == XPage::e_setting_update_ui )
    { updateUi(); }
    else if ( setting.mSetting == XPage::e_setting_user_role )
    { updateRole(); }
    else if ( setting.mSetting == XPage::e_setting_work_role )
    {
        check_para1();

        updateWorkingRole( setting.mPara1.toInt() );
    }
    else
    {}
}

QString XPage::model()
{
    if ( NULL == m_pPlugin )
    { return "sys"; }
    else
    { return m_pPlugin->model(); }
}

void XPage::showFocusHelp()
{
    emit signal_focus_changed(  model(),
                                mContextHelp );
}

void XPage::attachUpdateWorking( XPage::procDo proc,
                                 int wType,
                                 const QString &desc,
                                 void *pContext,
                                 int tmoms
                    )
{
    Q_ASSERT( NULL != m_pPlugin );

    m_pPlugin->attachUpdateWorking( this, (WorkingApi::eWorkingType)wType, proc, desc, pContext, tmoms );
}
void XPage::attachUpdateWorking( XPage::procDo proc,
                                 int wType,
                    XPage::preDo pre,
                    XPage::postDo post,
                    const QString &desc,
                    void *pContext,
                    int tmoms
                    )
{
    Q_ASSERT( NULL != m_pPlugin );

    m_pPlugin->attachUpdateWorking( this, (WorkingApi::eWorkingType)wType, proc, pre, post, desc, pContext,tmoms );
}

void XPage::attachMissionWorking( XPage *pObj,
                    XPage::onMsg onmsg,
                    QVariant var,
                    const QString &desc
                    )
{
    Q_ASSERT( NULL != m_pPlugin );

    m_pPlugin->attachMissionWorking( this, onmsg, var, desc );
}

void XPage::attachEmergencyWorking( XPage *pObj,
                    XPage::onMsg onmsg,
                    QVariant var,
                    const QString &desc
                    )
{
    Q_ASSERT( NULL != m_pPlugin );

    m_pPlugin->attachEmergencyWorking( this, onmsg, var, desc );
}

void XPage::setUri( const QString &uri )
{ mUri = uri; }
QString XPage::Uri( )
{ return mUri; }

void XPage::setContextHelp( const QString &str )
{ mContextHelp = str; }
QString XPage::contextHelp()
{ return mContextHelp; }

int XPage::save( const QString &fileName )
{
    QFile file( fileName );
    if ( file.open( QIODevice::WriteOnly ) )
    {}
    else
    { return -1; }

    int ret;
    QXmlStreamWriter writer( &file );

    writer.writeStartDocument();
    writer.writeStartElement( mUri );
        ret = serialOut( writer );
    writer.writeEndElement();
    writer.writeEndDocument();

    return ret;
}
int XPage::load( const QString &fileName )
{
    QFile file( fileName );
    if ( file.open( QIODevice::ReadOnly ) )
    {}
    else
    { return -1; }

    int ret;
    QXmlStreamReader reader( &file );

    while ( reader.readNextStartElement() )
    {
        if ( reader.name() == mUri )
        {
            ret = serialIn( reader );
        }
        else
        { reader.skipCurrentElement(); }
    }

    return ret;
}

int XPage::serialOut( QXmlStreamWriter &writer )
{ return -1; }
int XPage::serialIn( QXmlStreamReader &reader )
{ return -1; }

void XPage::spyEdited()
{

}

void XPage::spySetting( int setting )
{
    if ( mSpySetting.contains( setting ) )
    {}
    else
    { mSpySetting.append( setting ); }
}

bool XPage::filterSetting( XSetting setting )
{
    foreach (int set, mSpySetting)
    {
        if ( set == setting.mSetting )
        { return false; }
    }

    return true;
}

void XPage::enterMission()
{}
void XPage::exitMission()
{}

void XPage::setOperAble( bool b )
{
    setEnabled( b && isQualified() );
}

void XPage::setOnLine( bool b )
{
    //! \note
}

void XPage::setOpened( bool b )
{
    setEnabled( b && isQualified() );
}

void XPage::slot_plugin_setting_changed( XSetting setting )
{
    do
    {
        //! sys setting
        if ( (int)setting.mSetting < (int)e_setting_user )
        { break; }

        //! not in filter
        if ( filterSetting( setting ) )
        { return; }

    }while( 0 );

    onSetting( setting );
}

void XPage::slot_enter_mission( WorkingApi *api )
{
    Q_ASSERT( NULL != m_pPlugin );

    m_pPlugin->emit_setting_changed( XPage::e_setting_mission_working, true );
}
void XPage::slot_exit_mission( WorkingApi *api, int ret )
{
    m_pPlugin->emit_setting_changed( XPage::e_setting_mission_working, false );
}

void XPage::slot_request_save()
{}
void XPage::slot_request_load()
{}

void XPage::slot_modified()
{
    updateData();

    emit signal_request_save();
}
