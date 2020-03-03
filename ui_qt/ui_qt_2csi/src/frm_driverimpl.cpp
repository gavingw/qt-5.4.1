#include "frm_driverimpl.h"
#include <QMessageBox>
//
frm_driverImpl::frm_driverImpl( QWidget * parent, Qt::WindowFlags f) 
	: QWidget(parent, f)
{
	setupUi(this);
}
//


void frm_driverImpl::on_pbtn_ok_clicked()
{
    //int disktype = comboBox->currentIndex();

    QMessageBox::information(NULL, tr("format"), tr("format disk: ")+comboBox->currentText());

}

void frm_driverImpl::on_pushButton_clicked()
{
    this->close();
}

void frm_driverImpl::on_pbtn_close_clicked()
{
    this->close();
}
