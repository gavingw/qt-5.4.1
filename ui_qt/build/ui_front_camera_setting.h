/********************************************************************************
** Form generated from reading UI file 'front_camera_setting.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FRONT_CAMERA_SETTING_H
#define UI_FRONT_CAMERA_SETTING_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>

QT_BEGIN_NAMESPACE

class Ui_Front_camera_setting
{
public:
    QGridLayout *gridLayout_4;
    QGridLayout *gridLayout_3;
    QPushButton *pushButton;
    QSpacerItem *horizontalSpacer;
    QGridLayout *gridLayout_2;
    QPushButton *upperButton;
    QSpacerItem *verticalSpacer;
    QPushButton *downButton;
    QSpacerItem *verticalSpacer_3;
    QGridLayout *gridLayout;
    QPushButton *returnButton;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *sureButton;
    QSpacerItem *verticalSpacer_2;

    void setupUi(QDialog *Front_camera_setting)
    {
        if (Front_camera_setting->objectName().isEmpty())
            Front_camera_setting->setObjectName(QStringLiteral("Front_camera_setting"));
        Front_camera_setting->resize(660, 430);
        gridLayout_4 = new QGridLayout(Front_camera_setting);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        gridLayout_3 = new QGridLayout();
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        pushButton = new QPushButton(Front_camera_setting);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setEnabled(false);
        pushButton->setMinimumSize(QSize(121, 41));
        pushButton->setMaximumSize(QSize(121, 41));
        pushButton->setStyleSheet(QLatin1String("background-color:\n"
"rgb(66, 167, 255);\n"
"color:rgb(255,255,255)"));

        gridLayout_3->addWidget(pushButton, 0, 0, 1, 1);

        horizontalSpacer = new QSpacerItem(508, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer, 0, 1, 1, 1);


        gridLayout_4->addLayout(gridLayout_3, 0, 0, 1, 1);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        upperButton = new QPushButton(Front_camera_setting);
        upperButton->setObjectName(QStringLiteral("upperButton"));
        upperButton->setMinimumSize(QSize(55, 55));
        upperButton->setMaximumSize(QSize(55, 55));
        upperButton->setStyleSheet(QStringLiteral("border-image:url(\":/icon/navigate-up.png\")"));

        gridLayout_2->addWidget(upperButton, 0, 0, 1, 1);

        verticalSpacer = new QSpacerItem(13, 114, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_2->addItem(verticalSpacer, 1, 0, 1, 1);

        downButton = new QPushButton(Front_camera_setting);
        downButton->setObjectName(QStringLiteral("downButton"));
        downButton->setMinimumSize(QSize(55, 55));
        downButton->setMaximumSize(QSize(55, 55));
        downButton->setStyleSheet(QStringLiteral("border-image:url(\":/icon/navigate-down.png\")"));

        gridLayout_2->addWidget(downButton, 2, 0, 1, 1);


        gridLayout_4->addLayout(gridLayout_2, 2, 0, 1, 1);

        verticalSpacer_3 = new QSpacerItem(18, 67, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_4->addItem(verticalSpacer_3, 3, 0, 1, 1);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        returnButton = new QPushButton(Front_camera_setting);
        returnButton->setObjectName(QStringLiteral("returnButton"));
        returnButton->setMinimumSize(QSize(41, 41));
        returnButton->setMaximumSize(QSize(41, 41));
        returnButton->setStyleSheet(QStringLiteral("border-image:url(:/icon/Back.png)"));

        gridLayout->addWidget(returnButton, 0, 0, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(528, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 0, 1, 1, 1);

        sureButton = new QPushButton(Front_camera_setting);
        sureButton->setObjectName(QStringLiteral("sureButton"));
        sureButton->setMinimumSize(QSize(75, 41));
        sureButton->setMaximumSize(QSize(75, 41));
        sureButton->setStyleSheet(QLatin1String("QPushButton{\n"
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

        gridLayout->addWidget(sureButton, 0, 2, 1, 1);


        gridLayout_4->addLayout(gridLayout, 4, 0, 1, 1);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_4->addItem(verticalSpacer_2, 1, 0, 1, 1);

        gridLayout_4->setRowStretch(0, 1);
        gridLayout_4->setRowStretch(1, 1);
        gridLayout_4->setRowStretch(2, 3);
        gridLayout_4->setRowStretch(3, 1);
        gridLayout_4->setRowStretch(4, 1);

        retranslateUi(Front_camera_setting);

        QMetaObject::connectSlotsByName(Front_camera_setting);
    } // setupUi

    void retranslateUi(QDialog *Front_camera_setting)
    {
        Front_camera_setting->setWindowTitle(QApplication::translate("Front_camera_setting", "Dialog", 0));
        pushButton->setText(QApplication::translate("Front_camera_setting", "\345\211\215\346\221\204\345\203\217\345\244\264\350\256\276\347\275\256", 0));
        upperButton->setText(QString());
        downButton->setText(QString());
        returnButton->setText(QString());
        sureButton->setText(QApplication::translate("Front_camera_setting", "next", 0));
    } // retranslateUi

};

namespace Ui {
    class Front_camera_setting: public Ui_Front_camera_setting {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FRONT_CAMERA_SETTING_H
