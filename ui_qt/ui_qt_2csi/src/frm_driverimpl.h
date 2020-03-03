#ifndef FRM_DRIVERIMPL_H
#define FRM_DRIVERIMPL_H
//
#include <QWidget>
#include "ui_frm_driveSettings.h"
//
class frm_driverImpl : public QWidget, public Ui::frm_driver
{
Q_OBJECT
public:
	frm_driverImpl( QWidget * parent = 0, Qt::WindowFlags f = 0 );
private slots:
    void on_pbtn_ok_clicked();
    void on_pushButton_clicked();
    void on_pbtn_close_clicked();
};
#endif





