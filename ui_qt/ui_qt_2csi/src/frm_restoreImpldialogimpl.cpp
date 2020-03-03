#include "frm_restoreImpldialogimpl.h"
#include <QPushButton>
#include <QFileDialog>
//
frm_restoreImpl::frm_restoreImpl( QWidget * parent, Qt::WindowFlags f) 
	: QDialog(parent, f)
{
	setupUi(this);
	//connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    //connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}
//


void frm_restoreImpl::on_buttonBox_clicked(QAbstractButton *button)
{
    QString str;

      if(buttonBox->button(QDialogButtonBox::Ok)  == button)   //判断按下的是否为"确定”按钮
      {
        printf("ok\r\n");
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
