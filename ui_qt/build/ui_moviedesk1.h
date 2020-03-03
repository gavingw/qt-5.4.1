/********************************************************************************
** Form generated from reading UI file 'moviedesk1.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MOVIEDESK1_H
#define UI_MOVIEDESK1_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_movieDesk1
{
public:
    QGridLayout *gridLayout_4;
    QWidget *widget_2;
    QGridLayout *gridLayout_3;
    QWidget *widget;
    QGridLayout *gridLayout_2;
    QPushButton *videoButton;
    QPushButton *picButton;
    QPushButton *scanButton;
    QSpacerItem *verticalSpacer;
    QPushButton *mainCameraButton;
    QPushButton *viceButton;
    QGridLayout *gridLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *returnButton;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *musicButton;
    QSpacerItem *horizontalSpacer_3;
    QSpacerItem *horizontalSpacer_4;
    QStackedWidget *stackedWidget;
    QWidget *page;
    QWidget *page_2;

    void setupUi(QDialog *movieDesk1)
    {
        if (movieDesk1->objectName().isEmpty())
            movieDesk1->setObjectName(QStringLiteral("movieDesk1"));
        movieDesk1->resize(673, 457);
        movieDesk1->setStyleSheet(QLatin1String("background-color:rgb(86, 76, 75,0);\n"
"border-radius:5px;"));
        gridLayout_4 = new QGridLayout(movieDesk1);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        widget_2 = new QWidget(movieDesk1);
        widget_2->setObjectName(QStringLiteral("widget_2"));
        widget_2->setStyleSheet(QLatin1String("background-color:rgb(86, 76, 75,30);\n"
"border-radius:15px;"));
        gridLayout_3 = new QGridLayout(widget_2);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        widget = new QWidget(widget_2);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setStyleSheet(QLatin1String("background-color:rgb(86, 76, 75,90);\n"
"border-radius:10px;"));
        gridLayout_2 = new QGridLayout(widget);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        videoButton = new QPushButton(widget);
        videoButton->setObjectName(QStringLiteral("videoButton"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(videoButton->sizePolicy().hasHeightForWidth());
        videoButton->setSizePolicy(sizePolicy);
        videoButton->setMinimumSize(QSize(0, 0));
        videoButton->setMaximumSize(QSize(119, 51));
        videoButton->setFocusPolicy(Qt::NoFocus);
        videoButton->setStyleSheet(QLatin1String("QPushButton{\n"
"	border-style: none;\n"
"	border: 0px;\n"
"	color: #F0F0F0;\n"
"	padding: 5px;	\n"
"	border-radius:5px;\n"
"	background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #0F7DBE, stop:1 #1582C3); \n"
"}\n"
"\n"
".QPushButton[focusPolicy=\"0\"] {\n"
"	border-style: none;\n"
"	border: 0px;\n"
"	color: #F0F0F0;\n"
"	padding: 0px;	\n"
"	border-radius:3px;\n"
"	background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #0F7DBE, stop:1 #1582C3); \n"
"}\n"
"\n"
".QPushButton:hover{ \n"
"	background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #50A3F0, stop:1 #489CEA);\n"
"}\n"
"\n"
".QPushButton:pressed{ \n"
"	background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #0F7DBE, stop:1 #1582C3);\n"
"}\n"
""));

        gridLayout_2->addWidget(videoButton, 0, 1, 1, 1);

        picButton = new QPushButton(widget);
        picButton->setObjectName(QStringLiteral("picButton"));
        sizePolicy.setHeightForWidth(picButton->sizePolicy().hasHeightForWidth());
        picButton->setSizePolicy(sizePolicy);
        picButton->setMinimumSize(QSize(0, 0));
        picButton->setMaximumSize(QSize(119, 51));
        picButton->setFocusPolicy(Qt::NoFocus);
        picButton->setStyleSheet(QLatin1String("QPushButton{\n"
"	border-style: none;\n"
"	border: 0px;\n"
"	color: #F0F0F0;\n"
"	padding: 5px;	\n"
"	border-radius:5px;\n"
"	background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #0F7DBE, stop:1 #1582C3); \n"
"}\n"
"\n"
".QPushButton[focusPolicy=\"0\"] {\n"
"	border-style: none;\n"
"	border: 0px;\n"
"	color: #F0F0F0;\n"
"	padding: 0px;	\n"
"	border-radius:3px;\n"
"	background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #0F7DBE, stop:1 #1582C3); \n"
"}\n"
"\n"
".QPushButton:hover{ \n"
"	background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #50A3F0, stop:1 #489CEA);\n"
"}\n"
"\n"
".QPushButton:pressed{ \n"
"	background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #0F7DBE, stop:1 #1582C3);\n"
"}\n"
""));

        gridLayout_2->addWidget(picButton, 1, 1, 1, 1);

        scanButton = new QPushButton(widget);
        scanButton->setObjectName(QStringLiteral("scanButton"));
        sizePolicy.setHeightForWidth(scanButton->sizePolicy().hasHeightForWidth());
        scanButton->setSizePolicy(sizePolicy);
        scanButton->setMinimumSize(QSize(0, 0));
        scanButton->setMaximumSize(QSize(119, 51));
        scanButton->setFocusPolicy(Qt::NoFocus);
        scanButton->setStyleSheet(QLatin1String("QPushButton{\n"
"	border-style: none;\n"
"	border: 0px;\n"
"	color: #F0F0F0;\n"
"	padding: 5px;	\n"
"	border-radius:5px;\n"
"	background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #0F7DBE, stop:1 #1582C3); \n"
"}\n"
"\n"
".QPushButton[focusPolicy=\"0\"] {\n"
"	border-style: none;\n"
"	border: 0px;\n"
"	color: #F0F0F0;\n"
"	padding: 0px;	\n"
"	border-radius:3px;\n"
"	background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #0F7DBE, stop:1 #1582C3); \n"
"}\n"
"\n"
".QPushButton:hover{ \n"
"	background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #50A3F0, stop:1 #489CEA);\n"
"}\n"
"\n"
".QPushButton:pressed{ \n"
"	background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #0F7DBE, stop:1 #1582C3);\n"
"}\n"
""));

        gridLayout_2->addWidget(scanButton, 3, 1, 1, 1);

        verticalSpacer = new QSpacerItem(20, 177, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_2->addItem(verticalSpacer, 4, 1, 1, 1);

        mainCameraButton = new QPushButton(widget);
        mainCameraButton->setObjectName(QStringLiteral("mainCameraButton"));
        sizePolicy.setHeightForWidth(mainCameraButton->sizePolicy().hasHeightForWidth());
        mainCameraButton->setSizePolicy(sizePolicy);
        mainCameraButton->setMinimumSize(QSize(0, 0));
        mainCameraButton->setMaximumSize(QSize(119, 51));
        mainCameraButton->setFocusPolicy(Qt::NoFocus);
        mainCameraButton->setStyleSheet(QLatin1String("QPushButton{\n"
"	border-style: none;\n"
"	border: 0px;\n"
"	color: #F0F0F0;\n"
"	padding: 5px;	\n"
"	border-radius:5px;\n"
"	background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #0F7DBE, stop:1 #1582C3); \n"
"}\n"
"\n"
".QPushButton[focusPolicy=\"0\"] {\n"
"	border-style: none;\n"
"	border: 0px;\n"
"	color: #F0F0F0;\n"
"	padding: 0px;	\n"
"	border-radius:3px;\n"
"	background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #0F7DBE, stop:1 #1582C3); \n"
"}\n"
"\n"
".QPushButton:hover{ \n"
"	background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #50A3F0, stop:1 #489CEA);\n"
"}\n"
"\n"
".QPushButton:pressed{ \n"
"	background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #0F7DBE, stop:1 #1582C3);\n"
"}\n"
""));

        gridLayout_2->addWidget(mainCameraButton, 5, 1, 1, 1);

        viceButton = new QPushButton(widget);
        viceButton->setObjectName(QStringLiteral("viceButton"));
        sizePolicy.setHeightForWidth(viceButton->sizePolicy().hasHeightForWidth());
        viceButton->setSizePolicy(sizePolicy);
        viceButton->setMinimumSize(QSize(0, 0));
        viceButton->setMaximumSize(QSize(119, 51));
        viceButton->setFocusPolicy(Qt::NoFocus);
        viceButton->setStyleSheet(QLatin1String("QPushButton{\n"
"	border-style: none;\n"
"	border: 0px;\n"
"	color: #F0F0F0;\n"
"	padding: 5px;	\n"
"	border-radius:5px;\n"
"	background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #0F7DBE, stop:1 #1582C3); \n"
"}\n"
"\n"
".QPushButton[focusPolicy=\"0\"] {\n"
"	border-style: none;\n"
"	border: 0px;\n"
"	color: #F0F0F0;\n"
"	padding: 0px;	\n"
"	border-radius:3px;\n"
"	background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #0F7DBE, stop:1 #1582C3); \n"
"}\n"
"\n"
".QPushButton:hover{ \n"
"	background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #50A3F0, stop:1 #489CEA);\n"
"}\n"
"\n"
".QPushButton:pressed{ \n"
"	background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #0F7DBE, stop:1 #1582C3);\n"
"}\n"
""));

        gridLayout_2->addWidget(viceButton, 6, 1, 1, 1);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        horizontalSpacer = new QSpacerItem(28, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 0, 0, 1, 1);

        returnButton = new QPushButton(widget);
        returnButton->setObjectName(QStringLiteral("returnButton"));
        sizePolicy.setHeightForWidth(returnButton->sizePolicy().hasHeightForWidth());
        returnButton->setSizePolicy(sizePolicy);
        returnButton->setMinimumSize(QSize(41, 41));
        returnButton->setMaximumSize(QSize(41, 41));
        returnButton->setFocusPolicy(Qt::NoFocus);
        returnButton->setStyleSheet(QLatin1String("border-image:url(:/icon/Back.png);\n"
"background-color:rgb(255,255,255,0);"));

        gridLayout->addWidget(returnButton, 0, 1, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 0, 2, 1, 1);

        gridLayout->setColumnStretch(0, 1);
        gridLayout->setColumnStretch(1, 2);
        gridLayout->setColumnStretch(2, 1);

        gridLayout_2->addLayout(gridLayout, 7, 1, 1, 1);

        musicButton = new QPushButton(widget);
        musicButton->setObjectName(QStringLiteral("musicButton"));
        sizePolicy.setHeightForWidth(musicButton->sizePolicy().hasHeightForWidth());
        musicButton->setSizePolicy(sizePolicy);
        musicButton->setMinimumSize(QSize(0, 0));
        musicButton->setMaximumSize(QSize(119, 51));
        musicButton->setFocusPolicy(Qt::NoFocus);
        musicButton->setStyleSheet(QLatin1String("QPushButton{\n"
"	border-style: none;\n"
"	border: 0px;\n"
"	color: #F0F0F0;\n"
"	padding: 5px;	\n"
"	border-radius:5px;\n"
"	background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #0F7DBE, stop:1 #1582C3); \n"
"}\n"
"\n"
".QPushButton[focusPolicy=\"0\"] {\n"
"	border-style: none;\n"
"	border: 0px;\n"
"	color: #F0F0F0;\n"
"	padding: 0px;	\n"
"	border-radius:3px;\n"
"	background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #0F7DBE, stop:1 #1582C3); \n"
"}\n"
"\n"
".QPushButton:hover{ \n"
"	background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #50A3F0, stop:1 #489CEA);\n"
"}\n"
"\n"
".QPushButton:pressed{ \n"
"	background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #0F7DBE, stop:1 #1582C3);\n"
"}"));

        gridLayout_2->addWidget(musicButton, 2, 1, 1, 1);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_3, 4, 0, 1, 1);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_4, 4, 2, 1, 1);

        gridLayout_2->setRowStretch(0, 1);
        gridLayout_2->setRowStretch(1, 1);
        gridLayout_2->setRowStretch(2, 1);
        gridLayout_2->setRowStretch(3, 1);
        gridLayout_2->setRowStretch(4, 2);
        gridLayout_2->setRowStretch(5, 1);
        gridLayout_2->setRowStretch(6, 1);
        gridLayout_2->setRowStretch(7, 1);
        gridLayout_2->setColumnStretch(1, 5);
        gridLayout_2->setRowMinimumHeight(0, 45);
        gridLayout_2->setRowMinimumHeight(1, 45);
        gridLayout_2->setRowMinimumHeight(2, 45);
        gridLayout_2->setRowMinimumHeight(3, 45);
        gridLayout_2->setRowMinimumHeight(5, 45);
        gridLayout_2->setRowMinimumHeight(6, 45);
        gridLayout_2->setRowMinimumHeight(7, 45);

        gridLayout_3->addWidget(widget, 0, 0, 1, 1);

        stackedWidget = new QStackedWidget(widget_2);
        stackedWidget->setObjectName(QStringLiteral("stackedWidget"));
        stackedWidget->setMinimumSize(QSize(0, 0));
        stackedWidget->setMaximumSize(QSize(160000, 1600000));
        stackedWidget->setStyleSheet(QLatin1String("background-color:rgb(160,160,160,60);\n"
"border-radius:10px;"));
        page = new QWidget();
        page->setObjectName(QStringLiteral("page"));
        stackedWidget->addWidget(page);
        page_2 = new QWidget();
        page_2->setObjectName(QStringLiteral("page_2"));
        stackedWidget->addWidget(page_2);

        gridLayout_3->addWidget(stackedWidget, 0, 1, 1, 1);

        gridLayout_3->setColumnStretch(0, 1);
        gridLayout_3->setColumnStretch(1, 6);

        gridLayout_4->addWidget(widget_2, 0, 0, 1, 1);


        retranslateUi(movieDesk1);

        stackedWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(movieDesk1);
    } // setupUi

    void retranslateUi(QDialog *movieDesk1)
    {
        movieDesk1->setWindowTitle(QApplication::translate("movieDesk1", "Dialog", 0));
        videoButton->setText(QApplication::translate("movieDesk1", "Video", 0));
        picButton->setText(QApplication::translate("movieDesk1", "Picture", 0));
        scanButton->setText(QApplication::translate("movieDesk1", "Find...", 0));
        mainCameraButton->setText(QApplication::translate("movieDesk1", "Front Camera", 0));
        viceButton->setText(QApplication::translate("movieDesk1", "Rear Camera", 0));
        returnButton->setText(QString());
        musicButton->setText(QApplication::translate("movieDesk1", "music", 0));
    } // retranslateUi

};

namespace Ui {
    class movieDesk1: public Ui_movieDesk1 {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MOVIEDESK1_H
