#include "frm_updateimpl.h"
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
//
frm_UpdateImpl::frm_UpdateImpl( QWidget * parent, Qt::WindowFlags f) 
	: QWidget(parent, f)
{
	setupUi(this);
}
//


void frm_UpdateImpl::on_buttonBox_clicked(QAbstractButton *button)
{
    QString str;

      if(buttonBox->button(QDialogButtonBox::Ok)  == button)   //判断按下的是否为"确定”按钮
      {
        printf("ok\r\n");
        QMessageBox::information(NULL, tr("update"), tr("system will reboot"));
        #if defined(Q_OS_LINUX)

        #endif
      }
      else if(button == buttonBox->button((QDialogButtonBox::Cancel)))
      {

        printf("cancel\r\n");
#if defined(Q_OS_LINUX)

#endif

      }
    this->close();
}

void frm_UpdateImpl::on_comboBox_currentIndexChanged(int index)
{

}



void frm_UpdateImpl::on_comboBox_2_currentIndexChanged(int index)
{

}
