#ifndef TOOLS_H_HH
#define TOOLS_H_HH

#include <QDialog>
#include <QtGui>
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
#include <QFileDialog>
class QWidget;
class QModelIndex;
class QStringListModel;
class QListView;
class QPushButton;
class QLabel;
class QString;
class QStringList;
class QAction;


class PlayerList:public QWidget					//播放列表类
{
	Q_OBJECT;
	
	public:
		PlayerList(QWidget *parent=0);

	signals:
		void playmyList(int index);
		
	private:
		void PlayerListCreate();
		void PlayerListConnect();
		void PlayerListLayout();
		
	private slots:
		//void slotSelect(const QModelIndex &index);
		void slotAddButton();
		void slotDelButton();
		void slotDelAllButton();
		void slotSelect(const QModelIndex &index);
		void slotloopAroundButton();
	
	public:
		QStringList strlist;
		QVector<QString> myPlayList;
		

		QStringListModel *model;
		QListView *listView;

		bool loopAroundAll;
		
	private:
		QLabel *showLabel;
		QPushButton *addButton;
		QPushButton *delButton;
		QPushButton *delAllButton;
		QPushButton *loopAroundButton;
	   

};

#endif
