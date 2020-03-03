#ifndef FRM_SYSPARMIMPL_H
#define FRM_SYSPARMIMPL_H
//
#include <QWidget>
#include "ui_frm_sysparm.h"
//
class Frm_SysParmImpl : public QWidget, public Ui::Form
{
Q_OBJECT
public:
    Frm_SysParmImpl( QWidget * parent = 0, Qt::WindowFlags f = 0 );
private slots:
    void on_pbtn_ok_clicked();
    void on_pbtn_close_clicked();
};
#endif





