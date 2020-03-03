#include "mplayer.h"
#include "playerlist.h"
#include <QtGui>
#include <QProcess>
#include <iostream>
#include <QtCore>
#include <QtDebug>
#include <sys/types.h>
#include <sys/stat.h>
//#include <ioctl.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <QString>
#include <QMessageBox>
//#include <linux/soundcard.h>

//#define ARM_LINUX
#if defined(Q_OS_LINUX)
#define AUTPLAYER 1
#endif
MyPlayer::MyPlayer(QWidget *parent)														//构造函数
{
	QIcon volumeIcon(":/images/volume.png");										//音量图标
	QIcon muteIcon(":/images/mute.png");												//静音图标
	
	MyPlayerCreate();																//创建函数
	MyPlayerConnect();															//连接函数
	MyPlayerLayout();																//布局函数
	
	isPlaying = false;															//默认没有播放

	QSettings volumeconfig("../settings/volume.ini", QSettings::IniFormat);	//音量设置
	isMute = volumeconfig.value("isMute").toBool();													//是否静音
	currentVolume = volumeconfig.value("volume").toInt();										//当前音量
	volumeSlider->setValue(currentVolume);																	//根据当前音量来设置音量条的值
	
	if(isMute)								//是否静音
	{
		set_volume(0);					//如果是静音的话就将音量设置为0
		VolumeButton->setIcon(muteIcon);//将图标设置为静音的
	}
	else
	{
		set_volume(currentVolume);					//否则将音量设置为当前音量
		VolumeButton->setIcon(volumeIcon);	//将图标设置为音量图标
	}
	

	
    setGeometry(0,0,800,480);							//设置坐标
	
	setWindowFlags(Qt::FramelessWindowHint);//设置无边框
#if defined(AUTPLAYER)
    ap=new AUTPlayer();
#endif
    printf("MyPlayer\r\n");
}
MyPlayer::~MyPlayer()
{
    printf("~MyPlayer\r\n");
#if defined(AUTPLAYER)
    delete ap;
#endif

}
void MyPlayer::MyPlayerCreate()															//创建MPlayer
{

	showlabel = new QLabel;					//创建显示标签
    //showlabel->setBackgroundRole(QPalette::Dark);			//设置背景色为黑色
    //showlabel->setStyleSheet();
    showlabel->setStyleSheet(QStringLiteral("background-color: rgba(112, 200, 11, 0);"));
    showlabel->resize(800,480);												//重置大小
	showlabel->setAutoFillBackground(true);						//设置自动填充
	showlabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);	//设置对齐方式为左上角对齐
	
	process = new QProcess(this);			//创建进程

	StartButton = new QPushButton;		//开始按钮
	StopButton = new QPushButton;			//停止按钮
	NextButton = new QPushButton;			//快进按钮
	BackButton = new QPushButton;			//快退按钮
	OpenButton = new QPushButton;			//打开按钮
	VolumeButton = new QPushButton;		//音量按钮
	hideButton = new QPushButton;			//隐藏按钮
	closeButton = new QPushButton;		//关闭按钮
	NextVidioButton  = new QPushButton;	//下一曲按钮
	BackVidioButton  = new QPushButton;	//上一曲按钮
	
	QIcon playIcon(":/images/play.png");		//播放图标
	QIcon pauseIcon(":/images/pause.png");	//暂停图标
	QIcon nextIcon(":/images/next.png");		//快进图标
	QIcon backIcon(":/images/prev.png");		//快退图标
	QIcon stopIcon(":/images/stop.gif");		//停止图标
	QIcon openIcon(":/images/open.png");		//打开图标
	QIcon volumeIcon(":/images/volume.png");//音量图标
	QIcon muteIcon(":/images/mute.png");		//静音图标
	QIcon hideIcon(":/images/back1.png");		//隐藏图标
	QIcon closeIcon(":/images/close.png");	//关闭图标
	QIcon nextvidioIcon(":/images/next.gif");	//下一曲图标
	QIcon backvidioIcon(":/images/back.gif");	//上一曲图标

	showtimeLabel = new QLabel;								//显示时间标签
	showtimeLabel->setFixedSize(80,6);				//设置显示时间标签的大小
	QFont font("Helvetica", 7, QFont::Bold);	//构造字体
	showtimeLabel->setFont(font);							//设置显示时间标签的字体

	StartButton->setIcon(pauseIcon);					//将开始按钮的图标设置为暂停图标
    StartButton->setIconSize(QSize(50,50));		//设置开始图标的大小
    StartButton->setFixedSize(50,50);					//设置固定大小
	StartButton->setFlat(true);								//设置扁平

	StopButton->setIcon(stopIcon);						//将停止按钮的图标设置为停止图标
    StopButton->setIconSize(QSize(50,50));		//设置大小
    StopButton->setFixedSize(50,50);					//设置固定大小
	StopButton->setFlat(true);								//设置扁平
	//Flat :If this property is set, most styles will not paint the button background unless the button is being pressed

	NextButton->setIcon(nextIcon);						//设置快进按钮的图标为快进图标
    NextButton->setIconSize(QSize(50,50));		//设置大小
    NextButton->setFixedSize(50,50);					//设置固定大小
	NextButton->setFlat(true);								//设置扁平

	BackButton->setIcon(backIcon);						//设置快退按钮的图标为快退图标
    BackButton->setIconSize(QSize(50,50));		//设置大小
    BackButton->setFixedSize(50,50);					//设置固定大小
	BackButton->setFlat(true);								//设置扁平

	OpenButton->setIcon(openIcon);						//设置打开按钮的图标
    OpenButton->setIconSize(QSize(50,50));		//设置大小
    OpenButton->setFixedSize(50,50);					//设置大小固定
	OpenButton->setFlat(true);								//设置扁平

	VolumeButton->setIcon(volumeIcon);				//设置音量按钮的图标为音量
    VolumeButton->setIconSize(QSize(50,50));	//设置大小
    VolumeButton->setFixedSize(50,50);				//设置固定大小
	VolumeButton->setFlat(true);							//设置扁平

	hideButton->setIcon(hideIcon);						//设置隐藏按钮的图标
    hideButton->setIconSize(QSize(50,50));		//设置图标大小
    hideButton->setFixedSize(50,50);					//设置固定大小
	hideButton->setFlat(true);								//设置扁平

	closeButton->setIcon(closeIcon);					//设置关闭按钮的图标
    closeButton->setIconSize(QSize(50,50));		//设置图标大小
    closeButton->setFixedSize(50,50);					//设置固定大小
	closeButton->setFlat(true);								//设置扁平

	NextVidioButton->setIcon(nextvidioIcon);			//设置下一曲按钮的图标
    NextVidioButton->setIconSize(QSize(50,50));		//设置图标大小
    NextVidioButton->setFixedSize(50,50);					//设置固定大小
	NextVidioButton->setFlat(true);								//设置扁平

	BackVidioButton->setIcon(backvidioIcon);			//设置上一曲按钮的图标
    BackVidioButton->setIconSize(QSize(50,50));		//设置图标大小
    BackVidioButton->setFixedSize(50,50);					//设置固定大小
	BackVidioButton->setFlat(true);								//设置扁平
	

	TimeLineSlider = new QSlider(Qt::Horizontal);	//设置进度条为水平的
    TimeLineSlider->setRange(0,1020);							//设置进度条范围
	volumeSlider = new QSlider(Qt::Horizontal);		//设置音量条为水平
	volumeSlider->setRange(0,100);								//设置范围
	volumeSlider->setValue(10);										//设置默认值
	volumeSlider->setFixedSize(60,4);							//设置固定大小


	splitter = new QSplitter(Qt::Horizontal);			//设置过滤器为水平
	mylist = new PlayerList;											//构造播放列表
	playIndex = 0;																//初始化播放索引为0

}


void MyPlayer::MyPlayerConnect()							//信号和槽的连接
{

	connect(this,SIGNAL(GetcurrentTimePos(int)),this,SLOT(changeTimeSlider(int)));	//进度条的位置
	connect(this,SIGNAL(GetvidioLength(int)),this,SLOT(setTimeSliderRange(int)));		//进度条的长度

	connect(StartButton,SIGNAL(clicked()),this,SLOT(resume()));			//开始按钮点击后播放
	connect(StopButton,SIGNAL(clicked()),this,SLOT(stop()));				//停止按钮点击后停止
	connect(OpenButton,SIGNAL(clicked()),this,SLOT(slotOpenButton()));	//打开按钮点击后打开
	
	connect(TimeLineSlider,SIGNAL(sliderPressed()),this,SLOT(slotTimeLineSliderpress()));//按下进度条
	connect(TimeLineSlider,SIGNAL(sliderReleased()),this,SLOT(slotTimeLineSliderreleased()));//释放音量条

	connect(NextButton,SIGNAL(clicked()),this,SLOT(slotNextButton()));				//按下快进按钮
	connect(BackButton,SIGNAL(clicked()),this,SLOT(slotBackButton()));				//按下快退按钮

	connect(NextVidioButton,SIGNAL(clicked()),this,SLOT(slotNextVidioButton()));	//按下下一曲按钮
	connect(BackVidioButton,SIGNAL(clicked()),this,SLOT(slotBackVidioButton()));	//按下上一曲按钮

	connect(VolumeButton,SIGNAL(clicked()),this,SLOT(slotVolumeButton()));				//按下音量按钮
	connect(volumeSlider,SIGNAL(valueChanged(int)),this,SLOT(slotVolumeChange(int)));	//改变音量条的值
	connect(hideButton,SIGNAL(clicked()),this,SLOT(slotHideButton()));						//隐藏按钮被点击
	connect(closeButton,SIGNAL(clicked()),this,SLOT(slotCloseButton()));					//关闭按钮被点击

	connect(mylist,SIGNAL(playmyList(int)),this,SLOT(slotmyListSIGNAL(int)));			//点击播放列表

	
	//connect(process, SIGNAL(readyRead()), this, SLOT(receivedData())); 
    #if defined(AUTPLAYER)
#else
	connect(process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(end(int, QProcess::ExitStatus)));
#endif																																			//进程执行完，退出
}

void MyPlayer::MyPlayerLayout()					//布局管理
{

	splitter->addWidget(showlabel);				//添加显示标签
	//splitter->addWidget(mylist);					//添加列表

	QHBoxLayout *buttonLayout = new QHBoxLayout;		//按钮布局管理器，用于布局刚刚创建的按钮
	buttonLayout->addWidget(OpenButton);
	buttonLayout->addWidget(StopButton);
	//buttonLayout->addWidget(BackVidioButton);
	buttonLayout->addWidget(BackButton);
	buttonLayout->addWidget(StartButton);
	buttonLayout->addWidget(NextButton);
	//buttonLayout->addWidget(NextVidioButton);
	//buttonLayout->addWidget(hideButton);
	//buttonLayout->addWidget(closeButton);
	buttonLayout->addWidget(VolumeButton);
	buttonLayout->addWidget(volumeSlider);					//添加音量条
    //buttonLayout->addStretch();
	buttonLayout->addWidget(closeButton);
	

	QHBoxLayout *lineLayout = new QHBoxLayout;			//进度条布局管理
	lineLayout->addWidget(TimeLineSlider);
	lineLayout->addWidget(showtimeLabel);

	QVBoxLayout *layout = new QVBoxLayout;					//整体布局管理器
	layout->addWidget(splitter);							//添加过滤器
	layout->addLayout(lineLayout);						//添加进度条布局管理器
	layout->addLayout(buttonLayout);					//添加按钮布局管理器
	
	setLayout(layout);	//设置总的布局管理

	
}
int MyPlayer::informerr()
{
    QMessageBox* box = new QMessageBox;
    QTimer::singleShot(1000, box, SLOT(close()));

    box->setWindowTitle(tr("takepic"));
    box->setStyleSheet(QStringLiteral("background-color: rgb(0, 200, 11);"));
    box->setIcon(QMessageBox::Warning);
    box->move(30,430);
    box->show();
    return 0;
}
void MyPlayer::MyPlayerStart(const QString &filename,const QString &wid)		//mplayer启动
{

	QStringList args;				//MPlayer启动参数列表
#if defined(AUTPLAYER)
    printf("mplayer start is %s \r\n",filename.toStdString().c_str());
   int ret = ap->setUrl((char*)filename.toStdString().c_str());
    if(ret<0){
        printf("------set url err\r\n");
        informerr();
        return;
    }
   ap->setViewWindow(0,0,ap->getDispwidth(),ap->getDispheigth()-80);
   //sleep(1);
   ap->setUserCb(end,this);
   ret=ap->play();
   if(ret<0){
       printf("------set url err\r\n");
       informerr();
       return;
   }
    isPlaying = true;			//设置正在播放
    isMute = false;				//设置不是静音
    receivedData();				//获得数据
    getTimeLength();			//获得时间长度
   mytimerID = startTimer(500);		//获得时间ID
#else
	args << "-slave";
	args << "-quiet";
	
#ifdef ARM_LINUX

	args << "-ac";   
	args << "mad";
#endif	
	args << "-zoom";
	args << "-aspect"; //screen rate command
	args << "16:9";     //screen rate
	args << "-wid";    //play window path   
	args << wid;   
	args << "-autosync" << "100";
	args << filename;  

	isPlaying = true;			//设置正在播放
	isMute = false;				//设置不是静音
	process->start("mplayer",args);	//进程执行

	receivedData();				//获得数据
	getTimeLength();			//获得时间长度
	mytimerID = startTimer(20);		//获得时间ID


#endif
	
}






void MyPlayer::changeTimeSlider(int pos)			//改变进度条
{
	QString s;
	if((pos<TimeLineSlider->maximum() )&& (pos >0))		//判断时间位置是否正确
	{
		currentpos = pos;										//当前位置
		TimeLineSlider->setValue(pos);			//设置进度条的值
		s = TimetoString(pos);							//获得播放到的时间
		s += "/";														
		s += fileLength;
		showtimeLabel->setText(s);					//设置时间标签的内容
	}
	
}

void MyPlayer::setTimeSliderRange(int length)		//设置进度条的范围
{
	QString s;
	TimeLineSlider->setRange(0,length);						
	fileLength = TimetoString(length);
	s = "00:00 / ";
	s += fileLength;
	showtimeLabel->setText(s);
}

void MyPlayer::timerEvent(QTimerEvent *event)		//记事器事件
{
    if(event->timerId() == mytimerID)				//如果是自己获得的ID
	{	
		getCurTimePos();							//获得当前时间位置
		receivedData();								//获得数据
	}
	else
		QWidget::timerEvent(event);		//将事件参数传给父类
}


void MyPlayer::slotTimeLineSliderpress()		//处理进度条按下事件
{
	isPause = false;													//暂停否
	resume();																	//重新播放
}
void MyPlayer::slotTimeLineSliderreleased()	//事进度条释放函数
{
	isPause = true;														//暂停真
	resume();																	//重新播放
	int movetime;															//要移动到的时间
	movetime = TimeLineSlider->value();				//获得要移动到的时间值
	seek(movetime,2);													//定位

}

void MyPlayer::slotNextButton()							//处理快进按钮
{
	if(isPlaying)											//如果正在播放
	{
		isPause = false;
		resume();
		
		seek(4,0);			//重新定位
		
		isPause = true;
		resume();
	}
}

void MyPlayer::slotBackButton()							//处理快退按钮
{
	if(isPlaying)											//如果正在播放
	{
		isPause = false;
		resume();
		
		seek(-4,0);		//重新定位
		
		isPause = true;
		resume();
	}
}


void MyPlayer::replay()						//重新播放
{
    printf("--------------replay \r\n");
	//QTest::qSleep(1000);				//睡一秒	
	sleep(1000);		
	stop();									//停止播放
	qDebug("replay stop");
		
	wid =QString::number(showlabel->winId()) ;				//获得显示标签的ID

	MyPlayerStart(currentfile,wid);					//启动mplayer
	
}


void MyPlayer::slotNextVidioButton()			//处理下一曲按钮按下事件
{
	
	int Countrow = mylist->model->rowCount()-1;				//获得列表中的行数，因为索引从0开始所以要减1
	if (playIndex < Countrow)							//如果播放索引小于行数
	{
		playIndex++;												//播放索引后移
		
		if(playIndex > Countrow)						//如果播放索引大于当前总行数则重新回到起始行
		{
			playIndex = 0;			//重新回到起始行
		}
	}

	if(isPlaying)		//如果正在播放则要停止
	{
		stop();
	}
	
	if(playIndex >=0)			//判断播放索引是否合法
	{
		currentfile = mylist->myPlayList.at(playIndex);			//设置当前文件

		wid =QString::number(showlabel->winId()) ;					//获得显示标签ID

		QModelIndex index = mylist->model->index(playIndex);	//构造模式索引
		mylist->listView->setCurrentIndex(index);							//设置当前索引
		MyPlayerStart(currentfile,wid);												//开始播放
	}
	else
		stop();										//播放索引值不合法时停止播放
}

void MyPlayer::slotBackVidioButton()				//上一曲按钮
{
     printf("--------------slotBackVidioButton \r\n");
	int Countrow = mylist->model->rowCount()-1;			//获得行数
	if (playIndex < Countrow)				//判断播放索引是否合法
	{
		playIndex--;									//索引值减
		if(playIndex < 0)							//如果发现索引值小于0了，则跳到最后一行
			playIndex = Countrow;
	}

	if(isPlaying)									//如果正在播放则要停止
	{
		stop();
	}
	
	if(playIndex >=0)							//判断索引值是否合法
	{
		currentfile = mylist->myPlayList.at(playIndex);		//设置当前文件

		wid =QString::number(showlabel->winId()) ;				//获得显示标签的列表

		QModelIndex index = mylist->model->index(playIndex);		//构造模式索引
		mylist->listView->setCurrentIndex(index);								//设置当前索引

		MyPlayerStart(currentfile,wid);										//启动MPlayer播放
	}
	else
		stop();											//索引值不合法要停止
		
}

void MyPlayer::slotOpenButton()				//处理打开按钮事件
{
    printf("--------------slotOpenButton \r\n");
	QString fileName = QFileDialog::getOpenFileName(this,				//获得要打开的文件的信息
                                   tr("Open video"), ".",
                                  tr("video files (*.wmv *.avi *.mp3 *.mp4)"));
	if(!fileName.isEmpty())					//如果文件名不为空的话
	{
				
		if(isPlaying)					//如果正在播放就应该停止
		{
			stop();
		}
		currentfile = fileName;				//当前文件为刚刚获得的文件名
				
		wid =QString::number(showlabel->winId()) ;			//获得显示标签的ID

		//qDebug("Start 1111");
		MyPlayerStart(currentfile,wid);									//启动MPlayer

		
		mylist->myPlayList.append(fileName);						//将该文件添加到容器中去
		mylist->strlist<<QFileInfo(fileName).fileName();//		QStringList strlist;
																										//	QVector<QString> myPlayList;	
		mylist->model->setStringList(mylist->strlist);	//设置显示的列表
		int row = mylist->model->rowCount()-1;					//获得行数

		if(row>=0)			//如果行数合法的话
		{
			QModelIndex index = mylist->model->index(row);			//构造模式索引
		
			mylist->listView->setCurrentIndex(index);						//设置当前索引
		}
		
	}
}


void MyPlayer::slotVolumeButton()								//音量按钮处理的槽
{
	QSettings volumeconfig("../settings/volume.ini", QSettings::IniFormat);	//构造音量设置集合
	
	QIcon volumeIcon(":/images/volume.png");		//音量按钮图标
	QIcon muteIcon(":/images/mute.png");				//静音图标


	if(isMute)							//如果音量不是静音的话？？？？？？？？？？？？？？？？？？？？？？？？？？
	{
		set_volume(currentVolume);							//设置音量为当前音量
		VolumeButton->setIcon(volumeIcon);			//设置图标
		isMute = false;													//不是静音
	}
	else
	{
		set_volume(0);													//设置音量为0
		VolumeButton->setIcon(muteIcon);				//设置静音图标
		isMute = true;
	}
	volumeconfig.setValue("isMute",isMute);			//改变记录集中的是否静音值
	volumeconfig.setValue("volume",currentVolume);//改变记录集中的音量大小值
}

void MyPlayer::slotVolumeChange(int volume)			//音量被改变
{
	QSettings volumeconfig("../settings/volume.ini", QSettings::IniFormat);	//读默认音量值
	
	if(volume > 100)		//防止音量值不合法
		volume = 100;
	if(volume < 0)
		volume = 0;
	
	set_volume(volume);			//设置音量值为刚刚设置的音量
	currentVolume = volume;	//设置当前音量值为刚刚设置的音量
	volumeconfig.setValue("volume",currentVolume);//写当前音量到音量配置集合中
}

void MyPlayer::slotHideButton()			//处理隐藏按钮
{
	this->hide();	
}

void MyPlayer::slotCloseButton()		//处理关闭按钮
{
	
	stop();								//调用停止函数
	
	this->close();

}


QString MyPlayer::TimetoString(int seconds)				//把时间值变成字符串
{
	QString s;
	int hour = seconds / 3600;											//小时
	int minute = (seconds - 3600 * hour) / 60;			//分钟
	int second = seconds - 3600 * hour - 60 * minute;			//秒钟
	if(!hour)
	{
		s.sprintf("%02d%c%02d%c%02d", hour, ':', minute, ':', second);//如果小时为0
	}
	else
	{
		s.sprintf("02d%c%02d", minute, ':', second);
	}

	return s;
}


void MyPlayer::slotmyListSIGNAL(int Index)		//处理列表上的信号
{
 printf("--------------slotmyListSIGNAL \r\n");
	if(isPlaying)				//如果正在播放的话需要停止
	{
		stop();							
	}
	if(Index>=0)				//如果索引值合法
	{
		currentfile = mylist->myPlayList.at(Index);		//获得当前文件

		wid =QString::number(showlabel->winId()) ;		//获得编号

		MyPlayerStart(currentfile,wid);								//启动MPlayer
	}
	else
		stop();
}

#if defined(Q_OS_LINUX)
int  MyPlayer::end(int32_t msgType, void *user)		//结束MPlayer
{
    MyPlayer *p = (MyPlayer*)user;
printf("call end \r\n");

    if (0)
    {p->stop();
        //qDebug("End 1111");
        if(p->mylist->loopAroundAll)
        {
            p->slotNextVidioButton();

        }
        else
        {
            //replay();
        }
    }
}
#endif

void MyPlayer::end(int exitCode, QProcess::ExitStatus exitStatus)		//结束MPlayer
{
    stop();

	if (exitStatus == QProcess::NormalExit)
    {
		//qDebug("End 1111");
		if(mylist->loopAroundAll)
		{
			slotNextVidioButton();
			
		}
		else
		{
			//replay();
		}
	}
}





/*********************************************************
 **	功能   : 暂停播放			 
 *********************************************************/
void MyPlayer::resume()
{
printf("--------------resume \r\n");
	QIcon playIcon(":/images/play.png");							//播放图标
	QIcon pauseIcon(":/images/pause.png");						//暂停图标
	
	if(isPlaying)						//如果正在播放
	{
		if(isPause)						//如果暂停了的话
		{
			isPause = false;		//设置为不是暂停
            mytimerID = startTimer(500);					//获得计时器ID
			StartButton->setIcon(pauseIcon);		//设置开始按钮的图标为暂停图标
		}
		else
		{
			isPause = true;				//如果是没有暂停就设置暂停
			killTimer(mytimerID);	//停止计时器
			StartButton->setIcon(playIcon);			//设置开始按钮图标为播放
		}
#if defined(AUTPLAYER)
        ap->pause();
#else
   		sendCommand("pause"); //send contine command    
   		//qDebug("playing");
#endif
	}
	else
	{
		if(!currentfile.isEmpty())			//如果当前文件是空的的话
		{				
			wid =QString::number(showlabel->winId()) ;		//获得显示标签ID
			StartButton->setIcon(pauseIcon);							//设置开始按钮为暂停
			
			MyPlayerStart(currentfile,wid);								//启动MPlayer
		}
	}
}



int MyPlayer::set_volume(int value)				//设置音量
{
	int new_vol;									//存储新音量值

/*volume value from 0 to 100*/
	if (value > 100)			//音量不能超过100也不能小于0
	{
	    value = 100;
	}
	if (value < 0)
	{
	    value = 0;
	}

	new_vol = value | (value << 8); //new_vol storage current volume
#if 0
	int mixerfd = open("/dev/mixer",O_WRONLY);//open device driver///dev/dsp", O_RDWR
	if (mixerfd < 0)
	{
		qDebug("Unable to open mixer device.");
		qDebug("\nqDebug:%s,%d",__FILE__,__LINE__);
		return -1;
	}
	if (ioctl(mixerfd, MIXER_WRITE(SOUND_MIXER_VOLUME), &new_vol) == -1)			//设置新的音量值
	{	 
	   qDebug("Unable to ioctl set volume.");
	   qDebug("\nqDebug:%s,%d",__FILE__,__LINE__);
	}
	::close(mixerfd);
#endif
	return value;
}



void MyPlayer::stop( )			//停止播放MPlayer
{
    printf("--------------stop \r\n");
	if(isPlaying)					//如果正在播放
	{
		
		isPlaying = false;	//设置正在播放为否
		
		killTimer(mytimerID);		//结束计时器
		
		QIcon playIcon(":/images/play.png");	//构造播放图标
		StartButton->setIcon(playIcon);				//设置图标为播放
		TimeLineSlider->setValue(0);					//将进度条设为0
#if defined(AUTPLAYER)
        ap->stop();
#else
		sendCommand(QString("quit "));				//发送退出命令

		if(!process->waitForFinished(100))    // su		等待进程结束
		{
		    if(process->state() == QProcess::Running)	//如果进程的状态为正在执行
		    {
			    process->close(); //close process
		    }
		}
#endif
	}
}


void MyPlayer::seek(int value, int type)//value was moved value,type is play variety     //su
{
   QString command;
   if(isPlaying)
   {
#if defined(AUTPLAYER)
    ap->seekto(value+ap->getCurrentPosition());
#else
       sendCommand(command.sprintf("seek %d %d", value, type));//send command
#endif
   }
}


void MyPlayer::getCurTimePos()										//获得当前进度条位置
{
#if defined(AUTPLAYER)
    int pos =  ap->getCurrentPosition();
    //cout<<pos<<endl;
    emit GetcurrentTimePos(pos);//send command for get current time
#else
	sendCommand("get_time_pos");//send command for get current time as file to playing
#endif
}

void MyPlayer::getTimeLength()										//获得播放时间的长度
{
#if defined(AUTPLAYER)
    int timeLength = ap->getDuration();
    //cout<<timeLength<<endl;
    emit GetvidioLength(timeLength);//send command for get file all time
#else
	sendCommand("get_time_length");		//send command for get all length as file to playing
#endif
}


void MyPlayer::sendCommand(const QString& command)		//发送命令
{
    QByteArray commandByteArray = command.toLatin1();
	commandByteArray.append(0x0a);
	if(process->write(commandByteArray) != (command.length() + 1))
	{
		qDebug("\nPlayer sendCommand failed");
		qDebug("\nqDebug:%s,%d",__FILE__,__LINE__);
	}
}


void MyPlayer::receivedData()																		//获得数据
{
#if defined(AUTPLAYER)

#else
	while(process->canReadLine())
	{
		QByteArray output(process->readLine());
        QString str = QString::fromLatin1(output.data());
		//cout<<str.toStdString()<<endl;
		if(str.startsWith(tr("ANS_TIME_POSITION")))
		{
			QString sstr = str.mid(tr("ANS_TIME_POSITION=").length());
			int pos = static_cast<int>(sstr.toFloat()); 
			//cout<<pos<<endl;
			emit GetcurrentTimePos(pos);//send command for get current time
		}
		if(str.startsWith(tr("ANS_LENGTH")))
		{			
			QString sstr = str.mid(tr("ANS_LENGTH=").length());
			int timeLength = static_cast<int>(sstr.toFloat());
			//cout<<timeLength<<endl;
			emit GetvidioLength(timeLength);//send command for get file all time
		}		
	}
#endif
}



