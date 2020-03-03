#include "frm_netsettingsimpl.h"
#include <iostream>
using namespace std;
//
frm_netSettingsImpl::frm_netSettingsImpl( QWidget * parent, Qt::WindowFlags f) 
	: QWidget(parent, f)
{
	setupUi(this);
}
//




void frm_netSettingsImpl::on_buttonBox_clicked(QAbstractButton *button)
{
    QString str;

      if(buttonBox->button(QDialogButtonBox::Ok)  == button)   //判断按下的是否为"确定”按钮
      {
        cout<<"press ok"<<endl;
      }
      else if(button == buttonBox->button((QDialogButtonBox::Cancel)))
        {
cout<<"press cancel"<<endl;
      }
    this->close();
}
