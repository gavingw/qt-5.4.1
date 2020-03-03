/********************************************************************************
** Form generated from reading UI file 'rear_adas_setting.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REAR_ADAS_SETTING_H
#define UI_REAR_ADAS_SETTING_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_rear_ADAS_setting
{
public:
    QGridLayout *gridLayout_5;
    QGridLayout *gridLayout_2;
    QPushButton *titleButton;
    QSpacerItem *horizontalSpacer_4;
    QSpacerItem *verticalSpacer;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_3;
    QGridLayout *gridLayout_4;
    QLabel *highLabel;
    QSlider *high_low_Slider;
    QLabel *lowLabel;
    QWidget *widget;
    QFrame *HorizontalLine;
    QFrame *VerticelLine;
    QSpacerItem *verticalSpacer_3;
    QGridLayout *gridLayout;
    QLabel *leftLabel;
    QSlider *left_right_Slider;
    QLabel *rightLabel;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *verticalSpacer_2;
    QPushButton *returnButton;
    QPushButton *nextButton;

    void setupUi(QDialog *rear_ADAS_setting)
    {
        if (rear_ADAS_setting->objectName().isEmpty())
            rear_ADAS_setting->setObjectName(QStringLiteral("rear_ADAS_setting"));
        rear_ADAS_setting->resize(660, 430);
        rear_ADAS_setting->setStyleSheet(QString::fromUtf8("font: 75 10pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
        gridLayout_5 = new QGridLayout(rear_ADAS_setting);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        titleButton = new QPushButton(rear_ADAS_setting);
        titleButton->setObjectName(QStringLiteral("titleButton"));
        titleButton->setEnabled(false);
        titleButton->setMinimumSize(QSize(121, 51));
        titleButton->setMaximumSize(QSize(121, 51));
        titleButton->setStyleSheet(QLatin1String("background-color:rgb(71, 154, 255);\n"
"color:rgb(255,255,255)"));

        gridLayout_2->addWidget(titleButton, 0, 0, 1, 1);

        horizontalSpacer_4 = new QSpacerItem(498, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_4, 0, 1, 1, 1);


        gridLayout_5->addLayout(gridLayout_2, 0, 0, 1, 3);

        verticalSpacer = new QSpacerItem(20, 303, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_5->addItem(verticalSpacer, 1, 0, 1, 1);

        groupBox = new QGroupBox(rear_ADAS_setting);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setMinimumSize(QSize(371, 351));
        groupBox->setMaximumSize(QSize(371, 351));
        groupBox->setStyleSheet(QString::fromUtf8("QGroupBox{\n"
"border-width:2px;\n"
"border-style:solid;\n"
"border-radius: 10px;\n"
"border-color:blue;\n"
"margin-top:0.5ex;\n"
"}\n"
"font: 75 10pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
        gridLayout_3 = new QGridLayout(groupBox);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        gridLayout_4 = new QGridLayout();
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        highLabel = new QLabel(groupBox);
        highLabel->setObjectName(QStringLiteral("highLabel"));
        highLabel->setStyleSheet(QString::fromUtf8("font: 75 10pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";color:rgb(255,255,255)"));

        gridLayout_4->addWidget(highLabel, 0, 0, 1, 1);

        high_low_Slider = new QSlider(groupBox);
        high_low_Slider->setObjectName(QStringLiteral("high_low_Slider"));
        high_low_Slider->setOrientation(Qt::Vertical);

        gridLayout_4->addWidget(high_low_Slider, 1, 0, 1, 1);

        lowLabel = new QLabel(groupBox);
        lowLabel->setObjectName(QStringLiteral("lowLabel"));
        lowLabel->setStyleSheet(QStringLiteral("color:rgb(255,255,255)"));

        gridLayout_4->addWidget(lowLabel, 2, 0, 1, 1);


        gridLayout_3->addLayout(gridLayout_4, 0, 0, 1, 1);

        widget = new QWidget(groupBox);
        widget->setObjectName(QStringLiteral("widget"));
        HorizontalLine = new QFrame(widget);
        HorizontalLine->setObjectName(QStringLiteral("HorizontalLine"));
        HorizontalLine->setGeometry(QRect(0, 260, 301, 10));
        HorizontalLine->setStyleSheet(QLatin1String("color:\n"
"rgb(255, 8, 12)"));
        HorizontalLine->setFrameShape(QFrame::HLine);
        HorizontalLine->setFrameShadow(QFrame::Sunken);
        VerticelLine = new QFrame(widget);
        VerticelLine->setObjectName(QStringLiteral("VerticelLine"));
        VerticelLine->setGeometry(QRect(10, 40, 10, 241));
        VerticelLine->setStyleSheet(QStringLiteral("color:rgb(255, 2, 18)"));
        VerticelLine->setFrameShape(QFrame::VLine);
        VerticelLine->setFrameShadow(QFrame::Sunken);

        gridLayout_3->addWidget(widget, 0, 1, 1, 2);

        verticalSpacer_3 = new QSpacerItem(20, 33, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_3->addItem(verticalSpacer_3, 1, 0, 1, 1);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        leftLabel = new QLabel(groupBox);
        leftLabel->setObjectName(QStringLiteral("leftLabel"));
        leftLabel->setStyleSheet(QStringLiteral("color:rgb(255,255,255)"));

        gridLayout->addWidget(leftLabel, 0, 0, 1, 1);

        left_right_Slider = new QSlider(groupBox);
        left_right_Slider->setObjectName(QStringLiteral("left_right_Slider"));
        left_right_Slider->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(left_right_Slider, 0, 1, 1, 1);

        rightLabel = new QLabel(groupBox);
        rightLabel->setObjectName(QStringLiteral("rightLabel"));
        rightLabel->setStyleSheet(QStringLiteral("color:rgb(255,255,255)"));

        gridLayout->addWidget(rightLabel, 0, 2, 1, 1);


        gridLayout_3->addLayout(gridLayout, 1, 1, 1, 1);

        horizontalSpacer = new QSpacerItem(28, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer, 1, 2, 1, 1);

        gridLayout_3->setRowStretch(0, 8);
        gridLayout_3->setRowStretch(1, 1);
        gridLayout_3->setColumnStretch(0, 1);
        gridLayout_3->setColumnStretch(1, 9);
        gridLayout_3->setColumnStretch(2, 1);
        widget->raise();

        gridLayout_5->addWidget(groupBox, 1, 1, 2, 1);

        verticalSpacer_2 = new QSpacerItem(20, 303, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_5->addItem(verticalSpacer_2, 1, 2, 1, 1);

        returnButton = new QPushButton(rear_ADAS_setting);
        returnButton->setObjectName(QStringLiteral("returnButton"));
        returnButton->setMinimumSize(QSize(41, 41));
        returnButton->setMaximumSize(QSize(41, 41));
        returnButton->setStyleSheet(QStringLiteral("border-image:url(:/icon/Back.png)"));

        gridLayout_5->addWidget(returnButton, 2, 0, 1, 1);

        nextButton = new QPushButton(rear_ADAS_setting);
        nextButton->setObjectName(QStringLiteral("nextButton"));
        nextButton->setMinimumSize(QSize(81, 41));
        nextButton->setMaximumSize(QSize(81, 41));
        nextButton->setStyleSheet(QLatin1String("QPushButton{\n"
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

        gridLayout_5->addWidget(nextButton, 2, 2, 1, 1);


        retranslateUi(rear_ADAS_setting);

        QMetaObject::connectSlotsByName(rear_ADAS_setting);
    } // setupUi

    void retranslateUi(QDialog *rear_ADAS_setting)
    {
        rear_ADAS_setting->setWindowTitle(QApplication::translate("rear_ADAS_setting", "Dialog", 0));
        titleButton->setText(QApplication::translate("rear_ADAS_setting", "\345\220\216ADAS\350\256\276\347\275\256", 0));
        groupBox->setTitle(QString());
        highLabel->setText(QApplication::translate("rear_ADAS_setting", "\351\253\230", 0));
        lowLabel->setText(QApplication::translate("rear_ADAS_setting", "\344\275\216", 0));
        leftLabel->setText(QApplication::translate("rear_ADAS_setting", "\345\267\246", 0));
        rightLabel->setText(QApplication::translate("rear_ADAS_setting", "\345\217\263", 0));
        returnButton->setText(QString());
        nextButton->setText(QApplication::translate("rear_ADAS_setting", "\344\270\213\344\270\200\346\255\245", 0));
    } // retranslateUi

};

namespace Ui {
    class rear_ADAS_setting: public Ui_rear_ADAS_setting {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REAR_ADAS_SETTING_H
