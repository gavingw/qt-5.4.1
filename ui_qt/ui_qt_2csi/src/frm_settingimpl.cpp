#include "frm_settingimpl.h"
#include <QMessageBox>
#include <QPushButton>
#include <QFileDialog>

#if defined(Q_OS_LINUX)
#include "CameraDebug.h"

#include <sdklog.h>
#include "CameraFileCfg.h"
#include "V4L2CameraDevice2.h"
#include "CallbackNotifier.h"
#include "PreviewWindow.h"
#include "CameraHardware2.h"
#include "HALCameraFactory.h"
#include "CameraHardwareInterface.h"
#include "DvrFactory.h"

using namespace android;

#endif

#define TEST_CAMERA_NAME 0
frm_settingimpl::frm_settingimpl(QWidget *parent) : QWidget(parent),m_startup(1),m_horizontal(1280),
    m_vertical(720),m_cycletime(60),m_adas_en(0),m_tvout_type(0)
{
    setupUi(this);
    strcpy(m_filedir,"/mnt/UDISK");
    #if defined(Q_OS_LINUX)
    m_startup=config_get_startup(TEST_CAMERA_NAME);
     m_horizontal=config_get_weith(TEST_CAMERA_NAME);
     m_vertical=config_get_heigth(TEST_CAMERA_NAME);
     m_cycletime=config_get_cyctime(TEST_CAMERA_NAME);
     m_adas_en=config_get_adas(TEST_CAMERA_NAME);

     config_get_curfiledir(TEST_CAMERA_NAME,m_filedir);
     m_tvout_type=config_get_tvout(TEST_CAMERA_NAME);
#endif

     if(m_startup ==1)
        comboBox_3->setCurrentIndex(1);
     else
        comboBox_3->setCurrentIndex(0);

    int time_idx;
    if(m_cycletime == 60)
        time_idx=0;
    else if(m_cycletime == 180)
        time_idx=1;
    else if(m_cycletime == 300)
        time_idx=2;
    else
        time_idx=0;

    comboBox_4->setCurrentIndex(time_idx);

    int adas_en;
    if(m_adas_en ==0)
        adas_en = 0;
    else if(m_adas_en ==1)
        adas_en = 1;
    else
        adas_en = 0;

    comboBox_5->setCurrentIndex(adas_en);

    int resolution;

         if(m_horizontal ==1280)
             resolution = 0;
         else if(m_horizontal ==1280)
             resolution = 1;
          else
             resolution = 0;
    comboBox_6->setCurrentIndex(resolution);

    int tvout_type;
            if(m_tvout_type ==0)
                tvout_type = 0;
            else if(m_tvout_type ==1)
                tvout_type = 1;
             else if(m_tvout_type ==2)
                tvout_type = 0;
            else
                tvout_type = 0;
       comboBox_7->setCurrentIndex(tvout_type);
lineEdit_3->setText(m_filedir);
}

frm_settingimpl::~frm_settingimpl()
{

}

void frm_settingimpl::on_comboBox_5_currentIndexChanged(int index)
{
  printf("adas You select=%d \r\n",index);
  if(index==0)
      m_adas_en=0;
  else
      m_adas_en=1;
  #if defined(Q_OS_LINUX)
  //config_set_adas(m_adas_en);
#endif
}

void frm_settingimpl::on_comboBox_3_currentIndexChanged(int index)
{
    printf("setup You select=%d \r\n",index);
    if(index==0)
        m_startup=0;
    else
        m_startup=1;
#if defined(Q_OS_LINUX)
    //config_set_startup(m_startup);
#endif
}

void frm_settingimpl::on_comboBox_4_currentIndexChanged(int index)
{
   printf("time You select=%d \r\n",index);
   if(index==2)
       m_cycletime=300;
   else if(index==1)
       m_cycletime=180;
   else
       m_cycletime=60;
#if defined(Q_OS_LINUX)
    //config_set_cyctime(m_cycletime);
#endif
}

void frm_settingimpl::on_comboBox_6_currentIndexChanged(int index)
{
     printf("resolution You select=%d \r\n",index);
     if(index==1){
         m_horizontal=640;
         m_vertical=360;
     }
     else{
         m_horizontal=1280;
         m_vertical=720;
     }
     #if defined(Q_OS_LINUX)
    //config_set_weith(m_horizontal);
    //config_set_heigth(m_vertical);
    #endif
}

void frm_settingimpl::on_buttonBox_clicked(QAbstractButton *button)
{
    QString str;

      if(buttonBox->button(QDialogButtonBox::Ok)  == button)   //判断按下的是否为"确定”按钮
      {
        printf("ok\r\n");
        #if defined(Q_OS_LINUX)

         config_set_weith(TEST_CAMERA_NAME,m_horizontal);
         config_set_heigth(TEST_CAMERA_NAME,m_vertical);
         config_set_cyctime(TEST_CAMERA_NAME,m_cycletime);
         config_set_curfiledir(TEST_CAMERA_NAME,m_filedir);
         config_set_tvout(TEST_CAMERA_NAME,m_tvout_type);
         config_set_adas(TEST_CAMERA_NAME,m_adas_en);
        #endif
      }
      else if(button == buttonBox->button((QDialogButtonBox::Cancel)))
      {

        printf("cancel\r\n");
        #if defined(Q_OS_LINUX)
        m_startup=config_get_startup(TEST_CAMERA_NAME);
         m_horizontal=config_get_weith(TEST_CAMERA_NAME);
         m_vertical=config_get_heigth(TEST_CAMERA_NAME);
         m_cycletime=config_get_cyctime(TEST_CAMERA_NAME);
         m_adas_en=config_get_adas(TEST_CAMERA_NAME);

         config_get_curfiledir(TEST_CAMERA_NAME,m_filedir);
         m_tvout_type=config_get_tvout(TEST_CAMERA_NAME);
#endif

         if(m_startup ==1)
            comboBox_3->setCurrentIndex(1);
         else
            comboBox_3->setCurrentIndex(0);

        int time_idx;
        if(m_cycletime == 60)
            time_idx=0;
        else if(m_cycletime == 180)
            time_idx=1;
        else if(m_cycletime == 300)
            time_idx=2;
        else
            time_idx=0;

        comboBox_4->setCurrentIndex(time_idx);

        int adas_en;
        if(m_adas_en ==0)
            adas_en = 0;
        else if(m_adas_en ==1)
            adas_en = 1;
        else
            adas_en = 0;

        comboBox_5->setCurrentIndex(adas_en);

        int resolution;

             if(m_horizontal ==1280)
                 resolution = 0;
             else if(m_horizontal ==1280)
                 resolution = 1;
              else
                 resolution = 0;
        comboBox_6->setCurrentIndex(resolution);

        int tvout_type;
                if(m_tvout_type ==0)
                    tvout_type = 0;
                else if(m_tvout_type ==1)
                    tvout_type = 1;
                 else if(m_tvout_type ==2)
                    tvout_type = 0;
                else
                    tvout_type = 0;
           comboBox_7->setCurrentIndex(m_tvout_type);
    lineEdit_3->setText(m_filedir);

      }
    this->close();
}

void frm_settingimpl::on_pushButton_clicked()
{

    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                     "/",
                                                     QFileDialog::ShowDirsOnly
                                                     | QFileDialog::DontResolveSymlinks);

    lineEdit_3->setText(dir);
    printf("get dir is %s\r\n",dir.toStdString().c_str());

}

void frm_settingimpl::on_comboBox_7_currentIndexChanged(int index)
{
    if(index==2)
        m_tvout_type=2;
    else if(index==1)
        m_tvout_type=1;
    else
        m_tvout_type=0;
}
