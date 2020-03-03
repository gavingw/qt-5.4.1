#ifndef FRM_MANAGETOOLSIMPL_H
#define FRM_MANAGETOOLSIMPL_H
//
#include <QWidget>
#include "ui_frm_managetools.h"
#include "frm_sysinfoimpl.h"
#include "frm_updateimpl.h"
//
class frm_restoreImpl;
class Frm_SysParmImpl;
class frm_driverImpl;
class frm_onlineUserImpl;
class frm_sysinfoImpl;

class frm_managetoolsImpl : public QWidget, public Ui::frm_managetools
{
    Q_OBJECT
private:
    frm_restoreImpl * m_restoreForm;
    Frm_SysParmImpl * m_sysParmImpl;
    frm_driverImpl * m_driverImpl;
    frm_sysinfoImpl * m_sysinfo;
    frm_UpdateImpl *m_update;

public:
    frm_managetoolsImpl( QWidget * parent = 0, Qt::WindowFlags f = 0 );
    ~frm_managetoolsImpl();
private slots:
    void tbtn_sysparm_clicked(); //系统参数
    void tbtn_sysparm_accept();  //系统参数确认

    void tbtn_drive_clicked();   //磁盘管理
    void tbtn_drive_format();    //磁盘格式化

    void tbtn_restore_accept();
    void tbtn_restore_reject();
    void tbtn_restore_clicked(); //恢复出厂设置

    void on_tbtn_sysinfo_clicked();
    void on_tbtn_Update_clicked();
    void on_tbtn_systime_clicked();
};
#endif





