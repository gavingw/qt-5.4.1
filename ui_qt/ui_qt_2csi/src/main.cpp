#include <QApplication>
#include <QWidget>
#include "mainwindowimpl.h"
#include <QTextCodec>
#include "frminput.h"
#include "frmnum.h"
#include <QSplashScreen>
#include <QGraphicsScene>
#include <QGraphicsView>

//CModallessDialog  *toastdialog;  //最好定义成全局变量
MainWindowImpl *pMainWindowImpl;
#if defined(Q_OS_LINUX)
void testStop(int signo)
{
    printf("oops! stop!!! %d\n",signo);
    if(pMainWindowImpl->m_imageSettingsImpl!=NULL)
    {
        printf("oops! stop preview!!! %d\n",signo);
        pMainWindowImpl->m_imageSettingsImpl->stopRecord();

    }
    //abort();
    _exit(0);
}
#endif
int main(int argc, char ** argv)
{
	QApplication app( argc, argv );
    MainWindowImpl win;
    pMainWindowImpl=&win;
#if defined(Q_OS_LINUX)
    signal(SIGINT, testStop);
#endif
    QTranslator translator(0);
    translator.load("filemanager_zh",".");
    app.installTranslator(&translator);

    QTranslator qttranslator(0);
    qttranslator.load("qt_zh_CN",".");
    app.installTranslator(&qttranslator);

    QTranslator fileinfotranslator(0);
    fileinfotranslator.load("qfile_zh",".");
    app.installTranslator(&fileinfotranslator);

    //QApplication::setStyle("macintosh");


#if (QT_VERSION <= QT_VERSION_CHECK(5,0,0))
    //QT5版本以上默认就是采用UTF-8编码
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForTr(codec);
#endif

    qApp->setFont(QFont("WenQuanYi Zen Hei", 12));

#ifndef __arm__
    //如果输入法字库文件不存在,自动拷贝字库文件
    QString path = qApp->applicationDirPath();
    QFile file(path + "/py.db");
    if (!file.exists() || file.size()==0) {
        QStringList list = path.split("/");
        QString fileName, newName;
        for (int i = 0; i < list.count() - 2; i++) {
            fileName = fileName + list.at(i) + "/";
        }
        fileName += "InputTool/py.db";
        newName = path + "/py.db";
        file.remove();
        QFile::copy(fileName, newName);
    }
#endif
    //在main函数中就加载输入法面板,保证整个应用程序可以使用输入法
    //以下方法打开中文输入法
    frmInput::Instance()->Init("control", "silvery", 10, 10);
    qApp->setProperty("noinput",true);
    //以下方法打开数字键盘
    //frmNum::Instance()->Init("black", 10);
    //加载并显示启动画面
     QSplashScreen splash(QPixmap(":/images/image/bg.png"));
     splash.setDisabled(true); //禁用用户的输入事件响应
     splash.show();

     splash.showMessage(QObject::tr("asteroid 正在启动中...."),
        Qt::AlignLeft|Qt::AlignBottom,Qt::green);
     QElapsedTimer t;
     t.start();
     // printf("wind show---start sp---!\r\n");
    // win.m_imageSettingsImpl  = new Play();
     //win.m_imageSettingsImpl->startRecord();
     while(t.elapsed()<500)
     {
         QCoreApplication::processEvents();
     }

    printf("wind show---test---!\r\n");
    win.show();
   // splash.finish(&win);

    qApp->setProperty("noinput",true);
    win.m_imageSettingsImpl  = new Play();
    win.m_imageSettingsImpl->showNormal();
    win.m_imageSettingsImpl->videoshow();
    splash.finish(win.m_imageSettingsImpl);
	app.connect( &app, SIGNAL( lastWindowClosed() ), &app, SLOT( quit() ) );
    //QTimer::singleShot(1000, &win, SLOT(startRecorder()));
	return app.exec();
}
