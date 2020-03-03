#include "frm_sysinfoimpl.h"
#include <QPushButton>
#include <QFileDialog>
//
frm_sysinfoImpl::frm_sysinfoImpl( QWidget * parent, Qt::WindowFlags f) 
	: QWidget(parent, f)
{
	setupUi(this);
    lineEdit->setText("T3 LINUX SDK");
    lineEdit_2->setText("T3 LINUX SDK BOARD");
    lineEdit_3->setText("V0.0.1");
}
//


void frm_sysinfoImpl::on_pbtn_ok_clicked()
{
    this->close();
}
