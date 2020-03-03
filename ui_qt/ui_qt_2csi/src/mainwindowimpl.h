#ifndef MAINWINDOWIMPL_H
#define MAINWINDOWIMPL_H
//
#include <QMainWindow>
#include "ui_mainwindow.h"
#include "fileview.h"
#include "frm_playimpl.h"
#include "frm_settingimpl.h"
//
class frm_managetoolsImpl;
class frm_queryLogImpl;
class frm_netSettingsImpl;
class frm_imageSettingsImpl;
class frm_recordsettingsImpl;
class frm_yuntaisettingsImpl;
class Play;
class fileview;
class WpaGui;
class MyPlayer;
class QAutplayer;
class frm_settingimpl;

#define AUTPLAYER_UI2 1
class MainWindowImpl : public QMainWindow, public Ui::MainWindow
{
Q_OBJECT
private:
     frm_managetoolsImpl * m_managetoolform;
     fileview * m_querylogimpl;
     WpaGui * m_netSettingsImpl;

#ifdef AUTPLAYER_UI2
     QAutplayer * m_recordsettingsImpl;
#else
     MyPlayer * m_recordsettingsImpl;
#endif
    // frm_yuntaisettingsImpl * m_yuntaisettingsImpl;
    frm_settingimpl *m_settingimpl;
      QProcess *process;
public:
    MainWindowImpl( QWidget * parent = 0, Qt::WindowFlags f = 0 );
	~MainWindowImpl();
    Play * m_imageSettingsImpl;
public slots:
	void tbtn_managetools_clicked();
	void tbtn_querylog_clicked();
	void tbtn_userManage_clicked();
	void tbtn_netSettings_clicked();
	void tbtn_imageSettings_clicked();
	void tbtn_moveSettings_clicked();
    void tbtn_Settings_clicked();
	void tbtn_alarmSettings_clicked();
	void tbtn_except_clicked();
    void startRecorder();
    void end(int exitCode, QProcess::ExitStatus exitStatus);
private slots:

};
#endif






