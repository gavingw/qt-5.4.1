/********************************************************************************
** Form generated from reading UI file 'main_desktop.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAIN_DESKTOP_H
#define UI_MAIN_DESKTOP_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_main_desktop
{
public:
    QGridLayout *gridLayout_5;
    QGridLayout *gridLayout_3;
    QPushButton *pushButton;
    QSpacerItem *horizontalSpacer;
    QGridLayout *gridLayout_2;
    QLabel *time_Label;
    QPushButton *pushButton_8;
    QWidget *widget;
    QGridLayout *gridLayout_4;
    QSpacerItem *verticalSpacer;
    QWidget *widget_2;
    QGridLayout *gridLayout;
    QPushButton *camera_change_Button;
    QSpacerItem *horizontalSpacer_6;
    QSpacerItem *horizontalSpacer_7;
    QPushButton *movieButton;
    QPushButton *recordButton;
    QPushButton *setFirstButton;
    QSpacerItem *horizontalSpacer_5;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *cameraButton;
    QSpacerItem *horizontalSpacer_3;
    QSpacerItem *horizontalSpacer_4;

    void setupUi(QWidget *main_desktop)
    {
        if (main_desktop->objectName().isEmpty())
            main_desktop->setObjectName(QStringLiteral("main_desktop"));
        main_desktop->resize(1024, 600);
        main_desktop->setStyleSheet(QStringLiteral(""));
        gridLayout_5 = new QGridLayout(main_desktop);
        gridLayout_5->setSpacing(6);
        gridLayout_5->setContentsMargins(11, 11, 11, 11);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        gridLayout_5->setHorizontalSpacing(0);
        gridLayout_5->setContentsMargins(0, 0, 0, 0);
        gridLayout_3 = new QGridLayout();
        gridLayout_3->setSpacing(6);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        gridLayout_3->setContentsMargins(60, -1, 60, -1);
        pushButton = new QPushButton(main_desktop);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setMinimumSize(QSize(0, 0));
        pushButton->setMaximumSize(QSize(80, 39));
        pushButton->setFocusPolicy(Qt::NoFocus);
        pushButton->setStyleSheet(QLatin1String("\n"
"QPushButton:hover{ \n"
"	background:;\n"
"}\n"
"\n"
"QPushButton:pressed{ \n"
"	background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #0F7DBE, stop:1 #1582C3);\n"
"}\n"
"QPushButton{\n"
"border-image:url(:/icon/switchButton_1.png);\n"
"}"));

        gridLayout_3->addWidget(pushButton, 0, 0, 1, 1);

        horizontalSpacer = new QSpacerItem(418, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer, 0, 1, 1, 1);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setSpacing(6);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        time_Label = new QLabel(main_desktop);
        time_Label->setObjectName(QStringLiteral("time_Label"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(time_Label->sizePolicy().hasHeightForWidth());
        time_Label->setSizePolicy(sizePolicy);
        time_Label->setMinimumSize(QSize(0, 0));
        time_Label->setMaximumSize(QSize(261, 51));
        time_Label->setStyleSheet(QLatin1String("color:rgb(244, 255, 29);\n"
"font: 75 14pt \"Courier 10 Pitch\";\n"
""));
        time_Label->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(time_Label, 0, 0, 1, 1);

        pushButton_8 = new QPushButton(main_desktop);
        pushButton_8->setObjectName(QStringLiteral("pushButton_8"));
        pushButton_8->setMinimumSize(QSize(50, 50));
        pushButton_8->setMaximumSize(QSize(50, 50));
        pushButton_8->setStyleSheet(QStringLiteral("border-image:url(:/icon/gps.png)"));

        gridLayout_2->addWidget(pushButton_8, 0, 1, 1, 1);

        gridLayout_2->setColumnStretch(0, 5);
        gridLayout_2->setColumnStretch(1, 1);

        gridLayout_3->addLayout(gridLayout_2, 0, 2, 1, 1);

        gridLayout_3->setColumnStretch(0, 2);
        gridLayout_3->setColumnStretch(1, 3);
        gridLayout_3->setColumnStretch(2, 4);

        gridLayout_5->addLayout(gridLayout_3, 0, 0, 1, 1);

        widget = new QWidget(main_desktop);
        widget->setObjectName(QStringLiteral("widget"));
        sizePolicy.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
        widget->setSizePolicy(sizePolicy);
        widget->setMinimumSize(QSize(0, 0));
        widget->setMaximumSize(QSize(160000, 16000));

        gridLayout_5->addWidget(widget, 1, 0, 1, 1);

        gridLayout_4 = new QGridLayout();
        gridLayout_4->setSpacing(6);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        verticalSpacer = new QSpacerItem(20, 100, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_4->addItem(verticalSpacer, 0, 1, 1, 1);

        widget_2 = new QWidget(main_desktop);
        widget_2->setObjectName(QStringLiteral("widget_2"));
        sizePolicy.setHeightForWidth(widget_2->sizePolicy().hasHeightForWidth());
        widget_2->setSizePolicy(sizePolicy);
        widget_2->setStyleSheet(QStringLiteral("background-color:rgba(94, 94, 94, 0)"));
        gridLayout = new QGridLayout(widget_2);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 10, 0, -1);
        camera_change_Button = new QPushButton(widget_2);
        camera_change_Button->setObjectName(QStringLiteral("camera_change_Button"));
        camera_change_Button->setMinimumSize(QSize(81, 81));
        camera_change_Button->setMaximumSize(QSize(81, 81));
        camera_change_Button->setFocusPolicy(Qt::NoFocus);
        camera_change_Button->setStyleSheet(QStringLiteral("border-image:url(:/icon/Switch_Camera.png)"));

        gridLayout->addWidget(camera_change_Button, 0, 9, 1, 1);

        horizontalSpacer_6 = new QSpacerItem(34, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_6, 0, 6, 1, 1);

        horizontalSpacer_7 = new QSpacerItem(33, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_7, 0, 8, 1, 1);

        movieButton = new QPushButton(widget_2);
        movieButton->setObjectName(QStringLiteral("movieButton"));
        movieButton->setMinimumSize(QSize(81, 81));
        movieButton->setMaximumSize(QSize(81, 81));
        movieButton->setFocusPolicy(Qt::NoFocus);
        movieButton->setStyleSheet(QStringLiteral("border-image:url(:/icon/video.png)"));

        gridLayout->addWidget(movieButton, 0, 7, 1, 1);

        recordButton = new QPushButton(widget_2);
        recordButton->setObjectName(QStringLiteral("recordButton"));
        recordButton->setMinimumSize(QSize(81, 81));
        recordButton->setMaximumSize(QSize(81, 81));
        recordButton->setFocusPolicy(Qt::NoFocus);
        recordButton->setStyleSheet(QLatin1String("\n"
"\n"
"QPushButton:pressed{ \n"
"	background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #0F7DBE, stop:1 #1582C3);\n"
"}\n"
"QPushButton{\n"
"border-image:url(:/record.png)\n"
"}\n"
""));

        gridLayout->addWidget(recordButton, 0, 3, 1, 1);

        setFirstButton = new QPushButton(widget_2);
        setFirstButton->setObjectName(QStringLiteral("setFirstButton"));
        setFirstButton->setMinimumSize(QSize(81, 81));
        setFirstButton->setMaximumSize(QSize(81, 81));
        setFirstButton->setFocusPolicy(Qt::NoFocus);
        setFirstButton->setStyleSheet(QStringLiteral("border-image:url(:/icon/setting_main.png)"));

        gridLayout->addWidget(setFirstButton, 0, 5, 1, 1);

        horizontalSpacer_5 = new QSpacerItem(33, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_5, 0, 4, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 0, 0, 1, 1);

        cameraButton = new QPushButton(widget_2);
        cameraButton->setObjectName(QStringLiteral("cameraButton"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(cameraButton->sizePolicy().hasHeightForWidth());
        cameraButton->setSizePolicy(sizePolicy1);
        cameraButton->setMinimumSize(QSize(81, 81));
        cameraButton->setMaximumSize(QSize(81, 81));
        cameraButton->setFocusPolicy(Qt::NoFocus);
        cameraButton->setStyleSheet(QLatin1String("\n"
"QPushButton{\n"
"border-image:url(:/icon/camera.png);\n"
"}\n"
""));

        gridLayout->addWidget(cameraButton, 0, 1, 1, 1);

        horizontalSpacer_3 = new QSpacerItem(34, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_3, 0, 2, 1, 1);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_4, 0, 10, 1, 1);

        gridLayout->setColumnStretch(0, 3);
        gridLayout->setColumnStretch(1, 1);
        gridLayout->setColumnStretch(2, 1);
        gridLayout->setColumnStretch(3, 1);
        gridLayout->setColumnStretch(4, 1);
        gridLayout->setColumnStretch(5, 1);
        gridLayout->setColumnStretch(6, 1);
        gridLayout->setColumnStretch(7, 1);
        gridLayout->setColumnStretch(8, 1);
        gridLayout->setColumnStretch(9, 1);
        gridLayout->setColumnStretch(10, 3);

        gridLayout_4->addWidget(widget_2, 0, 0, 1, 1);


        gridLayout_5->addLayout(gridLayout_4, 2, 0, 1, 1);

        gridLayout_5->setRowStretch(0, 1);
        gridLayout_5->setRowStretch(1, 15);
        gridLayout_5->setRowStretch(2, 3);

        retranslateUi(main_desktop);

        QMetaObject::connectSlotsByName(main_desktop);
    } // setupUi

    void retranslateUi(QWidget *main_desktop)
    {
        main_desktop->setWindowTitle(QApplication::translate("main_desktop", "main_desktop", 0));
        pushButton->setText(QString());
        time_Label->setText(QString());
        pushButton_8->setText(QString());
        camera_change_Button->setText(QString());
        movieButton->setText(QString());
        recordButton->setText(QString());
        setFirstButton->setText(QString());
        cameraButton->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class main_desktop: public Ui_main_desktop {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAIN_DESKTOP_H
