/********************************************************************************
** Form generated from reading UI file 'video_player.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VIDEO_PLAYER_H
#define UI_VIDEO_PLAYER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Video_Player
{
public:
    QGridLayout *gridLayout_3;
    QWidget *titlewidget;
    QGridLayout *gridLayout_2;
    QLabel *lab_Ico;
    QLabel *label;
    QSpacerItem *horizontalSpacer;
    QPushButton *btnMenu_Min;
    QPushButton *btnMenu_Max;
    QPushButton *btnMenu_Close;
    QWidget *widget_2;
    QWidget *widget;
    QGridLayout *gridLayout;
    QPushButton *fastFrontButton;
    QPushButton *fastBackButton;
    QPushButton *preMovieButton;
    QPushButton *nextMovieButton;
    QPushButton *stopButton;
    QSlider *progressSlider;
    QLabel *nowTimeLabel;
    QPushButton *muteButton;
    QSlider *voiceSlider;
    QPushButton *playButton;
    QSpacerItem *verticalSpacer;

    void setupUi(QDialog *Video_Player)
    {
        if (Video_Player->objectName().isEmpty())
            Video_Player->setObjectName(QStringLiteral("Video_Player"));
        Video_Player->resize(800, 480);
        Video_Player->setStyleSheet(QStringLiteral("background-color:rgb(93, 93, 93)"));
        gridLayout_3 = new QGridLayout(Video_Player);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        gridLayout_3->setContentsMargins(0, 0, 0, 0);
        titlewidget = new QWidget(Video_Player);
        titlewidget->setObjectName(QStringLiteral("titlewidget"));
        titlewidget->setMinimumSize(QSize(0, 0));
        titlewidget->setMaximumSize(QSize(160000, 33));
        titlewidget->setStyleSheet(QLatin1String("QWidget{\n"
"	background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #0F7DBE, stop:1 #1582C3);\n"
"border-radius:0px;\n"
"}"));
        gridLayout_2 = new QGridLayout(titlewidget);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout_2->setHorizontalSpacing(2);
        gridLayout_2->setVerticalSpacing(1);
        gridLayout_2->setContentsMargins(1, 1, 1, 1);
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

        gridLayout_2->addWidget(lab_Ico, 0, 0, 1, 1);

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

        gridLayout_2->addWidget(label, 0, 1, 1, 1);

        horizontalSpacer = new QSpacerItem(107, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer, 0, 2, 1, 1);

        btnMenu_Min = new QPushButton(titlewidget);
        btnMenu_Min->setObjectName(QStringLiteral("btnMenu_Min"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(btnMenu_Min->sizePolicy().hasHeightForWidth());
        btnMenu_Min->setSizePolicy(sizePolicy1);
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

        gridLayout_2->addWidget(btnMenu_Min, 0, 3, 1, 1);

        btnMenu_Max = new QPushButton(titlewidget);
        btnMenu_Max->setObjectName(QStringLiteral("btnMenu_Max"));
        sizePolicy1.setHeightForWidth(btnMenu_Max->sizePolicy().hasHeightForWidth());
        btnMenu_Max->setSizePolicy(sizePolicy1);
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

        gridLayout_2->addWidget(btnMenu_Max, 0, 4, 1, 1);

        btnMenu_Close = new QPushButton(titlewidget);
        btnMenu_Close->setObjectName(QStringLiteral("btnMenu_Close"));
        sizePolicy1.setHeightForWidth(btnMenu_Close->sizePolicy().hasHeightForWidth());
        btnMenu_Close->setSizePolicy(sizePolicy1);
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

        gridLayout_2->addWidget(btnMenu_Close, 0, 5, 1, 1);

        gridLayout_2->setColumnStretch(0, 1);
        gridLayout_2->setColumnStretch(1, 5);
        gridLayout_2->setColumnStretch(2, 1);
        gridLayout_2->setColumnStretch(3, 1);
        gridLayout_2->setColumnStretch(4, 1);
        gridLayout_2->setColumnStretch(5, 1);

        gridLayout_3->addWidget(titlewidget, 0, 0, 1, 1);

        widget_2 = new QWidget(Video_Player);
        widget_2->setObjectName(QStringLiteral("widget_2"));
        widget_2->setMaximumSize(QSize(16777215, 60));
        QFont font;
        font.setPointSize(12);
        widget_2->setFont(font);
        widget_2->setStyleSheet(QStringLiteral("background-color:rgb(255, 255, 255,0)"));

        gridLayout_3->addWidget(widget_2, 2, 0, 1, 1);

        widget = new QWidget(Video_Player);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setMinimumSize(QSize(0, 0));
        widget->setMaximumSize(QSize(160000, 65));
        widget->setStyleSheet(QStringLiteral("background-color:rgb(255, 255, 255,0)"));
        gridLayout = new QGridLayout(widget);
        gridLayout->setSpacing(0);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
        gridLayout->setContentsMargins(5, 0, 5, 0);
        fastFrontButton = new QPushButton(widget);
        fastFrontButton->setObjectName(QStringLiteral("fastFrontButton"));
        fastFrontButton->setMinimumSize(QSize(0, 0));
        fastFrontButton->setMaximumSize(QSize(51, 51));
        fastFrontButton->setFocusPolicy(Qt::NoFocus);
        fastFrontButton->setStyleSheet(QLatin1String("QPushButton{\n"
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
"}\n"
"QPushButton{\n"
"border-image:url(:/icon/forward.png)\n"
"}"));

        gridLayout->addWidget(fastFrontButton, 0, 3, 1, 1);

        fastBackButton = new QPushButton(widget);
        fastBackButton->setObjectName(QStringLiteral("fastBackButton"));
        fastBackButton->setMinimumSize(QSize(0, 0));
        fastBackButton->setMaximumSize(QSize(51, 51));
        fastBackButton->setFocusPolicy(Qt::NoFocus);
        fastBackButton->setStyleSheet(QLatin1String("QPushButton{\n"
"	border-style: none;\n"
"	border: 0px;\n"
"	color: #F0F0F0;\n"
"	padding: 5px;	\n"
"	border-radius:5px;\n"
"	 \n"
"}\n"
"\n"
".QPushButton[focusPolicy=\"0\"] {\n"
"	border-style: none;\n"
"	border: 0px;\n"
"	color: #F0F0F0;\n"
"	padding: 0px;	\n"
"	border-radius:3px;\n"
"	 \n"
"}\n"
"\n"
".QPushButton:hover{ \n"
"	\n"
"}\n"
"\n"
".QPushButton:pressed{ \n"
"\n"
"}\n"
"QPushButton{\n"
"border-image:url(:/icon/fastback.png);\n"
"}"));

        gridLayout->addWidget(fastBackButton, 0, 1, 1, 1);

        preMovieButton = new QPushButton(widget);
        preMovieButton->setObjectName(QStringLiteral("preMovieButton"));
        preMovieButton->setMinimumSize(QSize(0, 51));
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

        gridLayout->addWidget(preMovieButton, 0, 0, 1, 1);

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

        gridLayout->addWidget(nextMovieButton, 0, 4, 1, 1);

        stopButton = new QPushButton(widget);
        stopButton->setObjectName(QStringLiteral("stopButton"));
        stopButton->setMinimumSize(QSize(0, 0));
        stopButton->setMaximumSize(QSize(51, 51));
        stopButton->setFocusPolicy(Qt::NoFocus);
        stopButton->setStyleSheet(QLatin1String("QPushButton{\n"
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
"}\n"
"QPushButton{\n"
"border-image:url(:/icon/Stop.png)\n"
"}"));

        gridLayout->addWidget(stopButton, 0, 5, 1, 1);

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

        gridLayout->addWidget(progressSlider, 0, 6, 1, 1);

        nowTimeLabel = new QLabel(widget);
        nowTimeLabel->setObjectName(QStringLiteral("nowTimeLabel"));
        nowTimeLabel->setMinimumSize(QSize(0, 0));
        nowTimeLabel->setMaximumSize(QSize(150, 51));
        nowTimeLabel->setFont(font);
        nowTimeLabel->setStyleSheet(QStringLiteral("color:rgb(255, 255, 255)"));
        nowTimeLabel->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(nowTimeLabel, 0, 7, 1, 1);

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

        gridLayout->addWidget(muteButton, 0, 8, 1, 1);

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

        gridLayout->addWidget(voiceSlider, 0, 9, 1, 1);

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

        gridLayout->addWidget(playButton, 0, 2, 1, 1);


        gridLayout_3->addWidget(widget, 3, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 297, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_3->addItem(verticalSpacer, 1, 0, 1, 1);

        gridLayout_3->setRowStretch(0, 1);
        gridLayout_3->setRowStretch(1, 2);
        gridLayout_3->setRowStretch(2, 1);
        gridLayout_3->setRowStretch(3, 1);
        widget->raise();
        titlewidget->raise();
        widget_2->raise();

        retranslateUi(Video_Player);

        QMetaObject::connectSlotsByName(Video_Player);
    } // setupUi

    void retranslateUi(QDialog *Video_Player)
    {
        Video_Player->setWindowTitle(QApplication::translate("Video_Player", "Dialog", 0));
        lab_Ico->setText(QString());
        label->setText(QString());
#ifndef QT_NO_TOOLTIP
        btnMenu_Min->setToolTip(QApplication::translate("Video_Player", "\346\234\200\345\260\217\345\214\226", 0));
#endif // QT_NO_TOOLTIP
        btnMenu_Min->setText(QString());
#ifndef QT_NO_TOOLTIP
        btnMenu_Max->setToolTip(QApplication::translate("Video_Player", "\346\234\200\345\244\247\345\214\226", 0));
#endif // QT_NO_TOOLTIP
        btnMenu_Max->setText(QString());
#ifndef QT_NO_TOOLTIP
        btnMenu_Close->setToolTip(QApplication::translate("Video_Player", "\345\205\263\351\227\255", 0));
#endif // QT_NO_TOOLTIP
        btnMenu_Close->setText(QString());
        fastFrontButton->setText(QString());
        fastBackButton->setText(QString());
        preMovieButton->setText(QString());
        nextMovieButton->setText(QString());
        stopButton->setText(QString());
        nowTimeLabel->setText(QString());
        muteButton->setText(QString());
        playButton->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Video_Player: public Ui_Video_Player {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VIDEO_PLAYER_H
