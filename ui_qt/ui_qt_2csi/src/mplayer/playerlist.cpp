#include "playerlist.h"

#include <QListWidget>
#include <QWidget>
#include <QModelIndex>
#include <QListView>
#include <QSplashScreen>
#include <QPixmap>
#include <QStringListModel>
#include <QtGui>
#include <QListView>

#include <iostream>

PlayerList::PlayerList(QWidget * parent)
{

	PlayerListCreate();
	PlayerListLayout();
	PlayerListConnect();

	loopAroundAll = true;
	
	listView->setModel(model);
	
	setFixedSize(90,200);
}

void PlayerList::PlayerListCreate()					//创建列表
{
	model = new QStringListModel(this);			//构造字符串列表模式
	listView = new QListView;								//构造列表视图

	showLabel = new QLabel(tr("Play List"));		//构造显示标题的标签
	showLabel->setFixedSize(90,15);							//设置大小
	QFont font("Helvetica", 8, QFont::Bold);
	showLabel->setFont(font);
	
	addButton = new QPushButton;
	delButton = new QPushButton;
	delAllButton = new QPushButton;
	loopAroundButton = new QPushButton;

	QIcon addIcon(":/images/add.png");
	QIcon delIcon(":/images/del.png");
	QIcon delAllIcon(":/images/close.png");
	QIcon singleIcon(":/images/single.gif");
	QIcon allPlayIcon(":/images/allpaly.gif");

	addButton->setIcon(addIcon);
	addButton->setIconSize(QSize(15,15));
	addButton->setFixedSize(15,15);
	addButton->setFlat(true);

	delButton->setIcon(delIcon);
	delButton->setIconSize(QSize(15,15));
	delButton->setFixedSize(15,15);
	delButton->setFlat(true);


	delAllButton->setIcon(delAllIcon);
	delAllButton->setIconSize(QSize(15,15));
	delAllButton->setFixedSize(15,15);
	delAllButton->setFlat(true);

	loopAroundButton->setIcon(allPlayIcon);
	loopAroundButton->setIconSize(QSize(15,15));
	loopAroundButton->setFixedSize(15,15);
	loopAroundButton->setFlat(true);

	
}
void PlayerList::PlayerListLayout()
{
	QHBoxLayout *ButtonLayout = new QHBoxLayout;
	ButtonLayout->addWidget(addButton);
	ButtonLayout->addWidget(delButton);
	ButtonLayout->addWidget(delAllButton);
	ButtonLayout->addWidget(loopAroundButton);


	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(showLabel);
	layout->addWidget(listView);
	layout->addLayout(ButtonLayout);

	setLayout(layout);
	
}


void PlayerList::PlayerListConnect()
{
	connect(listView,SIGNAL(clicked(const QModelIndex)),this,SLOT(slotSelect(const QModelIndex)));
	connect(addButton,SIGNAL(clicked()),this,SLOT(slotAddButton()));
	connect(delButton,SIGNAL(clicked()),this,SLOT(slotDelButton()));
	connect(delAllButton,SIGNAL(clicked()),this,SLOT(slotDelAllButton()));
	connect(loopAroundButton,SIGNAL(clicked()),this,SLOT(slotloopAroundButton()));
}

void PlayerList::slotAddButton()
{
	QString fileName = QFileDialog::getOpenFileName(this,
                                   tr("Open video"), "./movie",
                                  tr("video files (*.wmv *.avi *.mp3)"));
	if(!fileName.isEmpty())
	{
		myPlayList.append(fileName);
		strlist<<QFileInfo(fileName).fileName();
		model->setStringList(strlist);
	}
}

void PlayerList::slotDelButton()
{
	int row = listView->currentIndex().row();

	if(row>=0)
	{
		model->removeRows(row,1);
		myPlayList.remove(row);
	}
	emit playmyList(row);
}

void PlayerList::slotloopAroundButton()
{
	QIcon singleIcon(":/images/single.gif");
	QIcon allPlayIcon(":/images/allpaly.gif");
	
	if(loopAroundAll)
	{
		loopAroundAll = false;
		loopAroundButton->setIcon(singleIcon);
	}
	else
	{
		loopAroundAll = true;
		loopAroundButton->setIcon(allPlayIcon);
	}
}

void PlayerList::slotDelAllButton()
{
	int i=0;

	for(i=model->rowCount()-1;i>=0;i--)
	{
		model->removeRows(0,1);
		myPlayList.remove(0);
	}
}

void PlayerList::slotSelect(const QModelIndex &index)
{
	emit playmyList(index.row());
}
