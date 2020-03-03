/********************************************************************************
** Form generated from reading UI file 'music_player.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MUSIC_PLAYER_H
#define UI_MUSIC_PLAYER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Music_Player
{
public:
    QGridLayout *gridLayout_6;
    QWidget *titlewidget;
    QGridLayout *gridLayout_3;
    QLabel *lab_Ico;
    QLabel *label;
    QSpacerItem *horizontalSpacer;
    QPushButton *btnMenu_Min;
    QPushButton *btnMenu_Max;
    QPushButton *btnMenu_Close;
    QSpacerItem *verticalSpacer;
    QSpacerItem *verticalSpacer_2;
    QGridLayout *gridLayout_4;
    QSpacerItem *horizontalSpacer_2;
    QLabel *posterLabel;
    QSpacerItem *horizontalSpacer_4;
    QGridLayout *gridLayout_2;
    QHBoxLayout *horizontalLayout_4;
    QLabel *Label1;
    QLabel *musicNameLabel;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label2;
    QLabel *authorNameLabel;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label3;
    QLabel *AlbumLabel;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_2;
    QLabel *yearLabel;
    QSpacerItem *horizontalSpacer_3;
    QGridLayout *gridLayout_5;
    QSpacerItem *horizontalSpacer_5;
    QLabel *lyricLabel;
    QSpacerItem *horizontalSpacer_6;
    QWidget *widget;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QPushButton *preMovieButton;
    QPushButton *playButton;
    QPushButton *nextMovieButton;
    QHBoxLayout *horizontalLayout_3;
    QSlider *progressSlider;
    QLabel *nowTimeLabel;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *muteButton;
    QSlider *voiceSlider;
    QPushButton *otherInfo;
    QPushButton *playListButton;

    void setupUi(QDialog *Music_Player)
    {
        if (Music_Player->objectName().isEmpty())
            Music_Player->setObjectName(QStringLiteral("Music_Player"));
        Music_Player->resize(1024, 600);
        Music_Player->setStyleSheet(QStringLiteral("background-color:rgb(91, 91, 91);"));
        gridLayout_6 = new QGridLayout(Music_Player);
        gridLayout_6->setObjectName(QStringLiteral("gridLayout_6"));
        gridLayout_6->setContentsMargins(1, 0, 0, -1);
        titlewidget = new QWidget(Music_Player);
        titlewidget->setObjectName(QStringLiteral("titlewidget"));
        titlewidget->setMinimumSize(QSize(0, 0));
        titlewidget->setMaximumSize(QSize(16000, 33));
        titlewidget->setStyleSheet(QLatin1String("QWidget{\n"
"	background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #0F7DBE, stop:1 #1582C3);\n"
"border-radius:0px;\n"
"}"));
        gridLayout_3 = new QGridLayout(titlewidget);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        gridLayout_3->setContentsMargins(5, 0, 0, 0);
        lab_Ico = new QLabel(titlewidget);
        lab_Ico->setObjectName(QStringLiteral("lab_Ico"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(lab_Ico->sizePolicy().hasHeightForWidth());
        lab_Ico->setSizePolicy(sizePolicy);
        lab_Ico->setMinimumSize(QSize(0, 0));
        lab_Ico->setMaximumSize(QSize(32, 33));
        lab_Ico->setStyleSheet(QLatin1String("QLabel{\n"
"	border-radius:0px;\n"
"	color: #F0F0F0;\n"
"	background-color:rgba(0,0,0,0);\n"
"	border-style:none;\n"
"}"));
        lab_Ico->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(lab_Ico, 0, 0, 1, 1);

        label = new QLabel(titlewidget);
        label->setObjectName(QStringLiteral("label"));
        label->setMinimumSize(QSize(0, 0));
        label->setMaximumSize(QSize(761, 33));
        label->setStyleSheet(QLatin1String("QLabel{\n"
"	border-radius:0px;\n"
"	color: #F0F0F0;\n"
"	background-color:rgba(0,0,0,0);\n"
"	border-style:none;\n"
"}"));

        gridLayout_3->addWidget(label, 0, 1, 1, 1);

        horizontalSpacer = new QSpacerItem(103, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer, 0, 2, 1, 1);

        btnMenu_Min = new QPushButton(titlewidget);
        btnMenu_Min->setObjectName(QStringLiteral("btnMenu_Min"));
        sizePolicy.setHeightForWidth(btnMenu_Min->sizePolicy().hasHeightForWidth());
        btnMenu_Min->setSizePolicy(sizePolicy);
        btnMenu_Min->setMinimumSize(QSize(0, 0));
        btnMenu_Min->setMaximumSize(QSize(32, 33));
        btnMenu_Min->setCursor(QCursor(Qt::ArrowCursor));
        btnMenu_Min->setFocusPolicy(Qt::NoFocus);
        btnMenu_Min->setStyleSheet(QLatin1String("QPushButton{\n"
"	border-radius:0px;\n"
"	color: #F0F0F0;\n"
"	background-color:rgba(0,0,0,0);\n"
"	border-style:none;\n"
"}\n"
"QPushButton:hover{\n"
"	background-color: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(25, 134, 199, 0), stop:1 #50A3F0);\n"
"}"));
        btnMenu_Min->setFlat(true);

        gridLayout_3->addWidget(btnMenu_Min, 0, 3, 1, 1);

        btnMenu_Max = new QPushButton(titlewidget);
        btnMenu_Max->setObjectName(QStringLiteral("btnMenu_Max"));
        sizePolicy.setHeightForWidth(btnMenu_Max->sizePolicy().hasHeightForWidth());
        btnMenu_Max->setSizePolicy(sizePolicy);
        btnMenu_Max->setMinimumSize(QSize(0, 0));
        btnMenu_Max->setMaximumSize(QSize(32, 33));
        btnMenu_Max->setCursor(QCursor(Qt::ArrowCursor));
        btnMenu_Max->setFocusPolicy(Qt::NoFocus);
        btnMenu_Max->setStyleSheet(QLatin1String("QPushButton{\n"
"	border-radius:0px;\n"
"	color: #F0F0F0;\n"
"	background-color:rgba(0,0,0,0);\n"
"	border-style:none;\n"
"}\n"
"QPushButton:hover{\n"
"	background-color: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(25, 134, 199, 0), stop:1 #50A3F0);\n"
"}"));
        btnMenu_Max->setFlat(true);

        gridLayout_3->addWidget(btnMenu_Max, 0, 4, 1, 1);

        btnMenu_Close = new QPushButton(titlewidget);
        btnMenu_Close->setObjectName(QStringLiteral("btnMenu_Close"));
        sizePolicy.setHeightForWidth(btnMenu_Close->sizePolicy().hasHeightForWidth());
        btnMenu_Close->setSizePolicy(sizePolicy);
        btnMenu_Close->setMinimumSize(QSize(0, 0));
        btnMenu_Close->setMaximumSize(QSize(32, 33));
        btnMenu_Close->setCursor(QCursor(Qt::ArrowCursor));
        btnMenu_Close->setFocusPolicy(Qt::NoFocus);
        btnMenu_Close->setStyleSheet(QLatin1String("QPushButton{\n"
"	border-radius:0px;\n"
"	color: #F0F0F0;\n"
"	background-color:rgba(0,0,0,0);\n"
"	border-style:none;\n"
"}\n"
"QPushButton:hover{\n"
"	background-color: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(238, 0, 0, 128), stop:1 rgba(238, 44, 44, 255));\n"
"}"));
        btnMenu_Close->setFlat(true);

        gridLayout_3->addWidget(btnMenu_Close, 0, 5, 1, 1);

        gridLayout_3->setColumnStretch(0, 1);
        gridLayout_3->setColumnStretch(1, 5);
        gridLayout_3->setColumnStretch(2, 1);
        gridLayout_3->setColumnStretch(3, 1);
        gridLayout_3->setColumnStretch(4, 1);
        gridLayout_3->setColumnStretch(5, 1);

        gridLayout_6->addWidget(titlewidget, 0, 0, 1, 2);

        verticalSpacer = new QSpacerItem(20, 56, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_6->addItem(verticalSpacer, 1, 0, 1, 1);

        verticalSpacer_2 = new QSpacerItem(20, 56, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_6->addItem(verticalSpacer_2, 1, 1, 1, 1);

        gridLayout_4 = new QGridLayout();
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_4->addItem(horizontalSpacer_2, 0, 0, 1, 1);

        posterLabel = new QLabel(Music_Player);
        posterLabel->setObjectName(QStringLiteral("posterLabel"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(posterLabel->sizePolicy().hasHeightForWidth());
        posterLabel->setSizePolicy(sizePolicy1);
        posterLabel->setStyleSheet(QLatin1String("background-color:rgb(255,255,255,0);\n"
""));

        gridLayout_4->addWidget(posterLabel, 0, 1, 2, 1);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_4->addItem(horizontalSpacer_4, 0, 2, 1, 1);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        Label1 = new QLabel(Music_Player);
        Label1->setObjectName(QStringLiteral("Label1"));
        Label1->setMinimumSize(QSize(80, 50));
        Label1->setMaximumSize(QSize(80, 50));
        Label1->setStyleSheet(QLatin1String("color:#ffffff;\n"
"background-color:rgb(255,255,255,0);"));
        Label1->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_4->addWidget(Label1);

        musicNameLabel = new QLabel(Music_Player);
        musicNameLabel->setObjectName(QStringLiteral("musicNameLabel"));
        musicNameLabel->setMinimumSize(QSize(0, 0));
        musicNameLabel->setMaximumSize(QSize(300, 50));
        musicNameLabel->setStyleSheet(QLatin1String("color:#ffffff;\n"
"background-color:rgb(255,255,255,0);"));

        horizontalLayout_4->addWidget(musicNameLabel);


        gridLayout_2->addLayout(horizontalLayout_4, 0, 0, 1, 1);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        label2 = new QLabel(Music_Player);
        label2->setObjectName(QStringLiteral("label2"));
        label2->setMinimumSize(QSize(80, 50));
        label2->setMaximumSize(QSize(80, 50));
        label2->setStyleSheet(QLatin1String("color:#ffffff;\n"
"background-color:rgb(255,255,255,0);"));
        label2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_5->addWidget(label2);

        authorNameLabel = new QLabel(Music_Player);
        authorNameLabel->setObjectName(QStringLiteral("authorNameLabel"));
        authorNameLabel->setMinimumSize(QSize(0, 0));
        authorNameLabel->setMaximumSize(QSize(300, 50));
        authorNameLabel->setStyleSheet(QLatin1String("color:#ffffff;\n"
"background-color:rgb(255,255,255,0);"));

        horizontalLayout_5->addWidget(authorNameLabel);


        gridLayout_2->addLayout(horizontalLayout_5, 1, 0, 1, 1);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        label3 = new QLabel(Music_Player);
        label3->setObjectName(QStringLiteral("label3"));
        label3->setMinimumSize(QSize(80, 50));
        label3->setMaximumSize(QSize(80, 50));
        label3->setStyleSheet(QLatin1String("color:#ffffff;\n"
"background-color:rgb(255,255,255,0);"));
        label3->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_6->addWidget(label3);

        AlbumLabel = new QLabel(Music_Player);
        AlbumLabel->setObjectName(QStringLiteral("AlbumLabel"));
        AlbumLabel->setMinimumSize(QSize(0, 0));
        AlbumLabel->setMaximumSize(QSize(300, 50));
        AlbumLabel->setStyleSheet(QLatin1String("color:#ffffff;\n"
"background-color:rgb(255,255,255,0);"));

        horizontalLayout_6->addWidget(AlbumLabel);


        gridLayout_2->addLayout(horizontalLayout_6, 2, 0, 1, 1);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        label_2 = new QLabel(Music_Player);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setMinimumSize(QSize(80, 50));
        label_2->setMaximumSize(QSize(80, 50));
        label_2->setStyleSheet(QLatin1String("color:#ffffff;\n"
"background-color:rgb(255,255,255,0);"));
        label_2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_7->addWidget(label_2);

        yearLabel = new QLabel(Music_Player);
        yearLabel->setObjectName(QStringLiteral("yearLabel"));
        yearLabel->setMinimumSize(QSize(0, 0));
        yearLabel->setMaximumSize(QSize(300, 50));
        yearLabel->setStyleSheet(QLatin1String("color:#ffffff;\n"
"background-color:rgb(255,255,255,0);"));

        horizontalLayout_7->addWidget(yearLabel);


        gridLayout_2->addLayout(horizontalLayout_7, 3, 0, 1, 1);


        gridLayout_4->addLayout(gridLayout_2, 0, 3, 2, 1);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_4->addItem(horizontalSpacer_3, 1, 4, 1, 1);

        gridLayout_4->setColumnStretch(0, 1);
        gridLayout_4->setColumnStretch(1, 3);
        gridLayout_4->setColumnStretch(2, 1);
        gridLayout_4->setColumnStretch(3, 3);
        gridLayout_4->setColumnStretch(4, 1);

        gridLayout_6->addLayout(gridLayout_4, 2, 0, 1, 2);

        gridLayout_5 = new QGridLayout();
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_5->addItem(horizontalSpacer_5, 0, 0, 1, 1);

        lyricLabel = new QLabel(Music_Player);
        lyricLabel->setObjectName(QStringLiteral("lyricLabel"));
        lyricLabel->setMaximumSize(QSize(16777215, 60));
        lyricLabel->setStyleSheet(QStringLiteral("background-color:rgb(255,255,255,0);"));

        gridLayout_5->addWidget(lyricLabel, 0, 1, 1, 1);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_5->addItem(horizontalSpacer_6, 0, 2, 1, 1);

        gridLayout_5->setColumnStretch(0, 1);
        gridLayout_5->setColumnStretch(1, 5);
        gridLayout_5->setColumnStretch(2, 1);

        gridLayout_6->addLayout(gridLayout_5, 3, 0, 1, 2);

        widget = new QWidget(Music_Player);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setMinimumSize(QSize(0, 0));
        widget->setMaximumSize(QSize(16000, 65));
        widget->setStyleSheet(QStringLiteral("background-color:rgb(255,255,255,0);"));
        gridLayout = new QGridLayout(widget);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 4, 16, -1);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        preMovieButton = new QPushButton(widget);
        preMovieButton->setObjectName(QStringLiteral("preMovieButton"));
        preMovieButton->setMinimumSize(QSize(0, 0));
        preMovieButton->setMaximumSize(QSize(51, 51));
        preMovieButton->setFocusPolicy(Qt::NoFocus);
        preMovieButton->setStyleSheet(QLatin1String("\n"
"\n"
".QPushButton:hover{ \n"
"}\n"
"\n"
".QPushButton:pressed{ \n"
"}\n"
"QPushButton{\n"
"border-image:url(:/icon/skip-backward.png)\n"
"}"));

        horizontalLayout->addWidget(preMovieButton);

        playButton = new QPushButton(widget);
        playButton->setObjectName(QStringLiteral("playButton"));
        playButton->setMinimumSize(QSize(0, 0));
        playButton->setMaximumSize(QSize(51, 51));
        playButton->setFocusPolicy(Qt::NoFocus);
        playButton->setStyleSheet(QLatin1String("QPushButton{\n"
"	border-style: none;\n"
"	border: 0px;\n"
"	color: #F0F0F0;\n"
"	padding: 5px;	\n"
"	border-radius:5px;\n"
"}\n"
"\n"
".QPushButton[focusPolicy=\"0\"] {\n"
"	border-style: none;\n"
"	color: #F0F0F0;\n"
"	padding: 0px;	\n"
"	border-radius:3px;\n"
"\n"
"}\n"
"\n"
".QPushButton:hover{ \n"
"}\n"
"\n"
".QPushButton:pressed{ \n"
"}\n"
""));
        playButton->setIconSize(QSize(16, 16));

        horizontalLayout->addWidget(playButton);

        nextMovieButton = new QPushButton(widget);
        nextMovieButton->setObjectName(QStringLiteral("nextMovieButton"));
        nextMovieButton->setMinimumSize(QSize(0, 0));
        nextMovieButton->setMaximumSize(QSize(51, 51));
        nextMovieButton->setFocusPolicy(Qt::NoFocus);
        nextMovieButton->setStyleSheet(QLatin1String("\n"
".QPushButton:pressed{ \n"
"}\n"
"QPushButton{\n"
"border-image:url(:/icon/skip-forward.png);\n"
"}"));

        horizontalLayout->addWidget(nextMovieButton);


        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        progressSlider = new QSlider(widget);
        progressSlider->setObjectName(QStringLiteral("progressSlider"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(progressSlider->sizePolicy().hasHeightForWidth());
        progressSlider->setSizePolicy(sizePolicy2);
        progressSlider->setMinimumSize(QSize(0, 0));
        progressSlider->setMaximumSize(QSize(350, 19));
        progressSlider->setFocusPolicy(Qt::NoFocus);
        progressSlider->setStyleSheet(QLatin1String("QSlider::groove:horizontal,QSlider::add-page:horizontal { \n"
"	background: #808080; \n"
"	height: 8px; \n"
"	border-radius: 3px; \n"
"}\n"
"\n"
"QSlider::sub-page:horizontal {\n"
"	height: 8px; \n"
"	border-radius: 3px;\n"
"	background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #0F7DBE, stop:1 #1582C3); \n"
"}\n"
"\n"
"QSlider::handle:horizontal {\n"
"	width: 13px; \n"
"	margin-top: -3px; \n"
"	margin-bottom: -3px; \n"
"	border-radius: 6px;\n"
"	background: qradialgradient(spread:pad, cx:0.5, cy:0.5, radius:0.5, fx:0.5, fy:0.5,stop:0.6 #F0F0F0, stop:0.778409 #50A3F0);\n"
"}\n"
"\n"
"QSlider::handle:horizontal:hover {\n"
"	background: qradialgradient(spread: pad, cx: 0.5, cy: 0.5, radius: 0.5, fx: 0.5, fy: 0.5, stop: 0.6 #F0F0F0,stop:0.778409 #0F7DBE);\n"
"}"));
        progressSlider->setOrientation(Qt::Horizontal);

        horizontalLayout_3->addWidget(progressSlider);

        nowTimeLabel = new QLabel(widget);
        nowTimeLabel->setObjectName(QStringLiteral("nowTimeLabel"));
        nowTimeLabel->setMinimumSize(QSize(0, 0));
        nowTimeLabel->setMaximumSize(QSize(150, 51));
        QFont font;
        font.setPointSize(12);
        nowTimeLabel->setFont(font);
        nowTimeLabel->setStyleSheet(QStringLiteral("color:rgb(255, 255, 255)"));
        nowTimeLabel->setAlignment(Qt::AlignCenter);

        horizontalLayout_3->addWidget(nowTimeLabel);

        horizontalLayout_3->setStretch(0, 4);
        horizontalLayout_3->setStretch(1, 2);

        gridLayout->addLayout(horizontalLayout_3, 0, 1, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, -1, -1, -1);
        muteButton = new QPushButton(widget);
        muteButton->setObjectName(QStringLiteral("muteButton"));
        muteButton->setMinimumSize(QSize(0, 0));
        muteButton->setMaximumSize(QSize(51, 51));
        muteButton->setFocusPolicy(Qt::NoFocus);
        muteButton->setStyleSheet(QLatin1String("QPushButton{\n"
"	border-style: none;\n"
"	border: 0px;\n"
"	color: #F0F0F0;\n"
"	padding: 5px;	\n"
"	border-radius:5px;\n"
"}\n"
"\n"
".QPushButton[focusPolicy=\"0\"] {\n"
"	border-style: none;\n"
"	border: 0px;\n"
"	color: #F0F0F0;\n"
"	padding: 0px;	\n"
"	border-radius:3px;\n"
"}\n"
"\n"
".QPushButton:hover{ \n"
"}\n"
"\n"
".QPushButton:pressed{ \n"
"}"));

        horizontalLayout_2->addWidget(muteButton);

        voiceSlider = new QSlider(widget);
        voiceSlider->setObjectName(QStringLiteral("voiceSlider"));
        voiceSlider->setMinimumSize(QSize(0, 0));
        voiceSlider->setMaximumSize(QSize(98, 19));
        voiceSlider->setFocusPolicy(Qt::NoFocus);
        voiceSlider->setStyleSheet(QLatin1String("QSlider::groove:horizontal,QSlider::add-page:horizontal { \n"
"	background: #808080; \n"
"	height: 8px; \n"
"	border-radius: 3px; \n"
"}\n"
"\n"
"QSlider::sub-page:horizontal {\n"
"	height: 8px; \n"
"	border-radius: 3px;\n"
"	background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #0F7DBE, stop:1 #1582C3); \n"
"}\n"
"\n"
"QSlider::handle:horizontal {\n"
"	width: 13px; \n"
"	margin-top: -3px; \n"
"	margin-bottom: -3px; \n"
"	border-radius: 6px;\n"
"	background: qradialgradient(spread:pad, cx:0.5, cy:0.5, radius:0.5, fx:0.5, fy:0.5,stop:0.6 #F0F0F0, stop:0.778409 #50A3F0);\n"
"}\n"
"\n"
"QSlider::handle:horizontal:hover {\n"
"	background: qradialgradient(spread: pad, cx: 0.5, cy: 0.5, radius: 0.5, fx: 0.5, fy: 0.5, stop: 0.6 #F0F0F0,stop:0.778409 #0F7DBE);\n"
"}"));
        voiceSlider->setOrientation(Qt::Horizontal);

        horizontalLayout_2->addWidget(voiceSlider);


        gridLayout->addLayout(horizontalLayout_2, 0, 2, 1, 1);

        otherInfo = new QPushButton(widget);
        otherInfo->setObjectName(QStringLiteral("otherInfo"));
        otherInfo->setMinimumSize(QSize(0, 0));
        otherInfo->setMaximumSize(QSize(51, 51));
        otherInfo->setFocusPolicy(Qt::NoFocus);
        otherInfo->setStyleSheet(QStringLiteral("border-image:url(:/icon/checklist.png);"));

        gridLayout->addWidget(otherInfo, 0, 3, 1, 1);

        playListButton = new QPushButton(widget);
        playListButton->setObjectName(QStringLiteral("playListButton"));
        playListButton->setMinimumSize(QSize(0, 0));
        playListButton->setMaximumSize(QSize(51, 51));
        playListButton->setFocusPolicy(Qt::NoFocus);

        gridLayout->addWidget(playListButton, 0, 4, 1, 1);

        gridLayout->setColumnStretch(0, 1);
        gridLayout->setColumnStretch(1, 3);
        gridLayout->setColumnStretch(2, 1);
        gridLayout->setColumnStretch(3, 1);
        gridLayout->setColumnStretch(4, 1);

        gridLayout_6->addWidget(widget, 4, 0, 1, 2);

        gridLayout_6->setRowStretch(0, 1);
        gridLayout_6->setRowStretch(1, 1);
        gridLayout_6->setRowStretch(2, 4);
        gridLayout_6->setRowStretch(3, 1);
        gridLayout_6->setRowStretch(4, 1);
        titlewidget->raise();
        widget->raise();

        retranslateUi(Music_Player);

        QMetaObject::connectSlotsByName(Music_Player);
    } // setupUi

    void retranslateUi(QDialog *Music_Player)
    {
        Music_Player->setWindowTitle(QApplication::translate("Music_Player", "Dialog", 0));
        lab_Ico->setText(QString());
        label->setText(QString());
#ifndef QT_NO_TOOLTIP
        btnMenu_Min->setToolTip(QApplication::translate("Music_Player", "\346\234\200\345\260\217\345\214\226", 0));
#endif // QT_NO_TOOLTIP
        btnMenu_Min->setText(QString());
#ifndef QT_NO_TOOLTIP
        btnMenu_Max->setToolTip(QApplication::translate("Music_Player", "\346\234\200\345\244\247\345\214\226", 0));
#endif // QT_NO_TOOLTIP
        btnMenu_Max->setText(QString());
#ifndef QT_NO_TOOLTIP
        btnMenu_Close->setToolTip(QApplication::translate("Music_Player", "\345\205\263\351\227\255", 0));
#endif // QT_NO_TOOLTIP
        btnMenu_Close->setText(QString());
        posterLabel->setText(QString());
        Label1->setText(QApplication::translate("Music_Player", "\346\255\214\345\220\215\357\274\232", 0));
        musicNameLabel->setText(QString());
        label2->setText(QApplication::translate("Music_Player", "\346\255\214\346\211\213\357\274\232", 0));
        authorNameLabel->setText(QString());
        label3->setText(QApplication::translate("Music_Player", "\344\270\223\350\276\221\357\274\232", 0));
        AlbumLabel->setText(QString());
        label_2->setText(QApplication::translate("Music_Player", "\346\227\266\351\227\264\357\274\232", 0));
        yearLabel->setText(QString());
        lyricLabel->setText(QString());
        preMovieButton->setText(QString());
        playButton->setText(QString());
        nextMovieButton->setText(QString());
        nowTimeLabel->setText(QString());
        muteButton->setText(QString());
        otherInfo->setText(QString());
        playListButton->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Music_Player: public Ui_Music_Player {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MUSIC_PLAYER_H
