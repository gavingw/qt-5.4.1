#ifndef MPLAY_HH_HH_H
#define MPLAY_HH_HH_H

#include <QWidget>
#include <QProcess>
#include <QDialog>
#include <QLCDNumber>
#include <QWidget>
#include<QTextEdit>
#include<QApplication>
#include<QSystemTrayIcon>
#include <QDialog>
#include <QAction>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QSplitter>
#include <QSlider>
#include <QListView>
#include <QFileDialog>

class QProcess;
class QLabel;
class QString;
class QPushButton;
class QSlider;
class QTimerEvent;
class QIcon;
class QLabel;
class QSplitter;
class PlayerList;
#if defined(Q_OS_LINUX)
#define AUTPLAYER 1
#endif

#if defined(AUTPLAYER)
#include "AutPlayer.h"

using namespace android;
#endif
class MyPlayer:public QWidget
{
	Q_OBJECT;
	
	public:
		MyPlayer(QWidget *parent=0);
        ~MyPlayer();

		void timerEvent(QTimerEvent *event);												//时钟事件
#if defined(AUTPLAYER)
        AUTPlayer *ap;
#endif
        int informerr();
	private:
		void MyPlayerCreate();																			//创建Mplayer
		void MyPlayerConnect();																			//连接Mplayer
		void MyPlayerLayout();																			//布局管理

		void MyPlayerStart(const QString &filename,const QString &wid);		//开始播放
		void sendCommand(const QString& command);													//秒控制

		

	private:
		void getCurTimePos();																							//获得当前时间的位置
		void getTimeLength();																							//获得时间长度
		void receivedData();																							//获得数据
		void seek(int value, int type);																		//定位
		int set_volume(int value);																				//设置音量
		QString TimetoString(int seconds);																//将时间转化为字符串
		void replay();																										//重新播放

	signals:
		void GetcurrentTimePos(int pos);																	//获得当前时间位置
		void GetvidioLength(int timeLength);															//获得音视屏播放时间

	private slots:
		void changeTimeSlider(int pos);																		//改变时间进度条
		void setTimeSliderRange(int length);															//设置进度条范围

		void resume();																										//重置
		void stop();																											//停止
#if defined(AUTPLAYER)
        static int end(int32_t msgType, void *user);															//结束
#endif
     void end(int, QProcess::ExitStatus);

        void slotTimeLineSliderpress();																		//按下进度条的槽
		void slotTimeLineSliderreleased();																//释放进度条的槽
		void slotVolumeChange(int volume);																//改变音量
		void slotNextButton();																						//快进
		void slotBackButton();																						//快退
		void slotOpenButton();																						//打开按钮
		void slotVolumeButton();																					//音量按钮
		void slotHideButton();																						//隐藏按钮
		void slotCloseButton();																						//关闭按钮			

		void slotNextVidioButton();																				//下一曲
		void slotBackVidioButton();																				//上一曲

		void slotmyListSIGNAL(int Index);																	//处理列表信号
	private:
		bool isPlaying;									//是否在播放
		bool isPause;										//是否停止
		bool isMute;										//是否静音										
		QString currentfile;						//当前文件
		int currentpos;									//当前位置
		int currentVolume;							//当前音量
		QString fileLength;							//文件长度

	private:
		QProcess *process;							//进程指针
		QLabel *showlabel;							//显示标签
		QString wid;										//
		int mytimerID;									//计时器编号
		int playIndex;									//播放索引

	private:
		QPushButton *StartButton;				//开始按钮
		QPushButton *StopButton;				//停止按钮
		QPushButton *NextButton;				//快进按钮
		QPushButton *BackButton;				//快退按钮
		QPushButton *OpenButton;				//打开按钮
		QPushButton *VolumeButton;			//音量按钮
		QPushButton *hideButton;				//隐藏按钮
		QPushButton *closeButton;				//关闭按钮
		QPushButton *NextVidioButton;		//下一曲按钮
		QPushButton *BackVidioButton;		//上一曲按钮

		QSlider *TimeLineSlider;				//时间进度条
		QLabel *showtimeLabel;					//显示时间标签
		QSlider *volumeSlider;					//音量滑动条
		
		QSplitter *splitter;						//过滤器
		PlayerList *mylist;							//列表，用于放置所有选中的文件的信息
};


#endif
