#include "frm_managetoolsimpl.h"
#include "frm_restoreImpldialogimpl.h"
#include "frm_SysParmimpl.h"
#include "frm_driverimpl.h"
//#include "frm_onlineuserimpl.h"
#include "functions.h"
#include <QMessageBox>
#include <QTextCodec>
#include <QPushButton>
#include <QString>
#include <QFont>
//
frm_managetoolsImpl::frm_managetoolsImpl( QWidget * parent, Qt::WindowFlags f)
    : QWidget(parent, f)
{
    setupUi(this);
    m_restoreForm = new frm_restoreImpl(); //出厂设置
    m_sysParmImpl = new Frm_SysParmImpl(); //系统参数
    m_driverImpl = new frm_driverImpl(); //磁盘管理
    m_sysinfo =new frm_sysinfoImpl();
    m_update =new frm_UpdateImpl();
    connect(tbtn_sysparm,SIGNAL(clicked()), this, SLOT(tbtn_sysparm_clicked()));
    connect(m_sysParmImpl->pbtn_ok,SIGNAL(clicked()), this, SLOT(tbtn_sysparm_accept()));

    connect(tbtn_hardmanage,SIGNAL(clicked()), this, SLOT(tbtn_drive_clicked()));
    connect(m_driverImpl->pbtn_ok,SIGNAL(clicked()), this, SLOT(tbtn_drive_format()));
    connect(m_restoreForm->buttonBox,SIGNAL(accepted()), this, SLOT(tbtn_restore_accept()));
    connect(m_restoreForm->buttonBox,SIGNAL(rejected()), this, SLOT(tbtn_restore_reject()));
    connect(tbtn_restore,SIGNAL(clicked()), this, SLOT(tbtn_restore_clicked()));
#if 0
    m_restoreForm = new frm_restoreImpl(); //出厂设置
    m_sysParmImpl = new Frm_SysParmImpl(); //系统参数
    m_driverImpl = new frm_driverImpl(); //磁盘管理
    m_onlineuserimpl = new frm_onlineUserImpl(); //在线用户

    connect(tbtn_sysparm,SIGNAL(clicked()), this, SLOT(tbtn_sysparm_clicked()));
    connect(m_sysParmImpl->pbtn_ok,SIGNAL(clicked()), this, SLOT(tbtn_sysparm_accept()));

    connect(tbtn_hardmanage,SIGNAL(clicked()), this, SLOT(tbtn_drive_clicked()));
    connect(m_driverImpl->pbtn_ok,SIGNAL(clicked()), this, SLOT(tbtn_drive_format()));

    connect(tbtn_onlineUser,SIGNAL(clicked()), this, SLOT(tbtn_onlneUser_clicked()));

    connect(tbtn_stopAlarm,SIGNAL(clicked()), this, SLOT(tbtn_clearAlarm_clicked()));
    connect(m_restoreForm->buttonBox,SIGNAL(accepted()), this, SLOT(tbtn_restore_accept()));
    connect(m_restoreForm->buttonBox,SIGNAL(rejected()), this, SLOT(tbtn_restore_reject()));
    connect(tbtn_restore,SIGNAL(clicked()), this, SLOT(tbtn_restore_clicked()));
#endif
}

frm_managetoolsImpl::~frm_managetoolsImpl()
{
    if (m_restoreForm)
    {
        delete(m_restoreForm);
    }

    if (m_sysParmImpl)
    {
        delete(m_sysParmImpl);
    }

    if (m_driverImpl)
    {
        delete(m_driverImpl);
    }

    if (m_sysinfo)
    {
        delete(m_sysinfo);
    }
    if (m_update)
    {
        delete(m_update);
    }

}

void frm_managetoolsImpl::tbtn_sysparm_clicked() //系统参数
{
    m_sysParmImpl->showNormal();

}

void frm_managetoolsImpl::tbtn_sysparm_accept()  //系统参数确认
{


}

void frm_managetoolsImpl::tbtn_drive_clicked()   //磁盘管理
{
    m_driverImpl->showNormal();
}

void frm_managetoolsImpl::tbtn_drive_format()    //磁盘格式化
{


}


//恢复出厂设置
void frm_managetoolsImpl::tbtn_restore_clicked()
{
    m_restoreForm->showNormal();
}

void frm_managetoolsImpl::tbtn_restore_accept()
{

}

void frm_managetoolsImpl::tbtn_restore_reject()
{

}



//


void frm_managetoolsImpl::on_tbtn_sysinfo_clicked()
{
    if(m_sysinfo)
        m_sysinfo->showNormal();
}

void frm_managetoolsImpl::on_tbtn_Update_clicked()
{

    if(m_update)
        m_update->showNormal();
}

void frm_managetoolsImpl::on_tbtn_systime_clicked()
{

}
