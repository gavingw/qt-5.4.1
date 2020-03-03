#include "frm_SysParmimpl.h"
//
Frm_SysParmImpl::Frm_SysParmImpl( QWidget * parent, Qt::WindowFlags f) 
	: QWidget(parent, f)
{
	setupUi(this);
}
//


void Frm_SysParmImpl::on_pbtn_ok_clicked()
{
this->close();
}

void Frm_SysParmImpl::on_pbtn_close_clicked()
{
    this->close();
}
