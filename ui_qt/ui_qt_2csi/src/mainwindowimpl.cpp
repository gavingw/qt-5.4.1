#include "mainwindowimpl.h"
#include "frm_managetoolsimpl.h"
//#include "frm_querylogimpl.h"
//#include "frm_netsettingsimpl.h"
//#include "frm_yuntaisettingsimpl.h"
#include "frm_playimpl.h"
#include "fileview.h"
#include "wpagui.h"
#include <QTextCodec>
#include <QString>
#include "mplayer/mplayer.h"
#include "qautplayerimpl.h"
#define AUTPLAYER_UI2 1
//主窗体的构造函数
MainWindowImpl::MainWindowImpl( QWidget * parent, Qt::WindowFlags f)
    : QMainWindow(parent, f)
{
    setupUi(this);
#ifdef QT_ROTATE_APP
    QGraphicsScene *scene = new QGraphicsScene();
    QGraphicsView *view = new QGraphicsView(parent);
    view->setGeometry(0,0,800,480); // actual Display size
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    QGraphicsProxyWidget *proxy = scene->addWidget(this);
    view->setScene(scene);
    view->show();
    view->rotate(180);
#endif
    m_managetoolform     = new frm_managetoolsImpl();
    m_querylogimpl       = new fileview();
    m_netSettingsImpl    = new  WpaGui(qApp);
m_imageSettingsImpl=NULL;
#ifdef AUTPLAYER_UI2
    m_recordsettingsImpl = new QAutplayer; //new MyPlayer();
#else
    m_recordsettingsImpl = new MyPlayer();
#endif

    m_settingimpl =new frm_settingimpl();
    connect(tbtn_managetools,SIGNAL(clicked()), this, SLOT(tbtn_managetools_clicked()) );
    connect(tbtn_querylog,SIGNAL(clicked()), this, SLOT(tbtn_querylog_clicked()));

    connect(tbtn_netSettings,SIGNAL(clicked()), this, SLOT(tbtn_netSettings_clicked()));
    connect(tbtn_imageSettings,SIGNAL(clicked()), this, SLOT(tbtn_imageSettings_clicked()));
    connect(tbtn_moveSettings,SIGNAL(clicked()), this, SLOT(tbtn_moveSettings_clicked()));
    connect(tbtn_Settings,SIGNAL(clicked()), this, SLOT(tbtn_Settings_clicked()));

}

//析构函数
MainWindowImpl::~MainWindowImpl()
{
    delete(m_querylogimpl);
    delete(m_netSettingsImpl);
    delete(m_imageSettingsImpl);
    delete(m_recordsettingsImpl);
    delete(m_settingimpl);
}
void MainWindowImpl::end(int exitCode, QProcess::ExitStatus exitStatus)		//结束MPlayer
{
    printf("target process ret=%d \r\n",exitStatus);
    if(!process->waitForFinished(100))    // su		等待进程结束
    {
        if(process->state() == QProcess::Running)	//如果进程的状态为正在执行
        {
            process->close(); //close process
        }
    }
    setEnabled(true);
    showNormal();
    setHidden(false);
    if (exitStatus == QProcess::NormalExit)
    {


    }

}
//显示管理工具
void MainWindowImpl::tbtn_managetools_clicked()
{
    if (m_managetoolform)
    {
        m_managetoolform->showNormal();
    }
#if 0
    QStringList args;
    // /usr/local/Qt-5.4.1/examples/quick/demos/samegame
    printf("---------------mngt tool \r\n");
    showMinimized();
    setWindowState(Qt::WindowMinimized);
    setEnabled(false);
    setHidden(true);



    process = new QProcess(this);
    connect(process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(end(int, QProcess::ExitStatus)));
    args <<"";
    process->start("/usr/local/Qt-5.4.1/examples/quick/demos/samegame",args);

    QFrame *frame = new QFrame;
    frame->setObjectName("myframe");
    frame->resize(800,480);
    //frame->setStyleSheet("QFrame#myframe{border-image:url(images/frame.png)}" );
    frame->setStyleSheet(QStringLiteral("background-color: rgba(112, 200, 11, 0);"));
    frame->show();

#endif
}

void MainWindowImpl::tbtn_querylog_clicked()
{
    if (m_querylogimpl)
    {
        m_querylogimpl->showNormal();
    }
}

void MainWindowImpl::tbtn_userManage_clicked()
{
//	if (m_userManagerImpl)
    {
        //m_userManagerImpl->showNormal();
    }
}

void MainWindowImpl::tbtn_netSettings_clicked()
{
    if (m_netSettingsImpl)
    {
        qApp->setProperty("noinput",false);
        m_netSettingsImpl->showNormal();
        //QProcess::execute("/root/wpa_gui");
    }
}
void MainWindowImpl::tbtn_imageSettings_clicked()
{
    if (m_imageSettingsImpl)
    {
        //qApp->setProperty("noinput",true);
        setHidden(true);
        m_imageSettingsImpl->showNormal();
        m_imageSettingsImpl->videoshow();
    }
}

void MainWindowImpl::tbtn_moveSettings_clicked()
{
    if (m_recordsettingsImpl)
    {
        setHidden(true);
        qApp->setProperty("noinput",true);
        m_recordsettingsImpl->showNormal();
    }
}

void MainWindowImpl::tbtn_Settings_clicked()
{
    printf("camera setting \r\n");
    if (m_settingimpl)
    {
        m_settingimpl->showNormal();
    }
#if 0
    showMinimized();
    setWindowState(Qt::WindowMinimized);
    setEnabled(false);
    process = new QProcess(this);
    connect(process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(end(int, QProcess::ExitStatus)));
    process->start("/usr/local/Qt-5.4.1/examples/quick/demos/samegame/samegame",NULL);

    if (m_yuntaisettingsImpl)
    {
        //m_yuntaisettingsImpl->showNormal();
    }
#endif
}

void MainWindowImpl::tbtn_alarmSettings_clicked()
{
    //if (m_alarmsettingsImpl)
    {
        //m_alarmsettingsImpl->showNormal();
    }
}

void MainWindowImpl::tbtn_except_clicked()
{
    //if (m_excepthandleImpl)
    {
        //m_excepthandleImpl->showNormal();
    }
}
void MainWindowImpl::startRecorder()
{
    printf("startRecorder---start record---!\r\n");
#if defined(Q_OS_LINUX)
    //for boot logo display quickly
    //comment for stability by fk
    //system("insmod /system/vendor/modules/camera.ko");
    //system("insmod /system/vendor/modules/hm1375.ko");
    //system("insmod /system/vendor/modules/sunxi_csi0.ko ccm=\"hm1375\" i2c_addr=0x48");

    //gEmulatedCameraFactory.HALCameraFactoryInit();
#endif
    if(m_imageSettingsImpl==NULL){
        m_imageSettingsImpl  = new Play();
        printf("--------------m_imageSettingsImpl-------------\n");
    }else{

        printf("--------------m_imageSettingsImpl--=%p-----------\n",m_imageSettingsImpl);

    }

    m_imageSettingsImpl->startCurCamera();
}
