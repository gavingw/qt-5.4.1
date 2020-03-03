#ifndef FRM_SYSINFOIMPL_H
#define FRM_SYSINFOIMPL_H
//
#include <QWidget>
#include "ui_frm_sysinfo.h"
//
class frm_sysinfoImpl : public QWidget, public Ui::frm_sysinfo
{
Q_OBJECT
public:
	frm_sysinfoImpl( QWidget * parent = 0, Qt::WindowFlags f = 0 );
private slots:
    void on_pbtn_ok_clicked();
};
#endif





