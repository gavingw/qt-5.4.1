/********************************************************************************
** Form generated from reading UI file 'about.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ABOUT_H
#define UI_ABOUT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>

QT_BEGIN_NAMESPACE

class Ui_About
{
public:
    QGridLayout *gridLayout_3;
    QLabel *softerwareVersion;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    QLabel *label;
    QSpacerItem *horizontalSpacer_2;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer_5;
    QProgressBar *ROM_usage;
    QSpacerItem *horizontalSpacer_6;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *formatButton;
    QSpacerItem *horizontalSpacer_4;
    QGridLayout *gridLayout_2;
    QLabel *logLevelLabel;
    QComboBox *logLevelComboBox;

    void setupUi(QDialog *About)
    {
        if (About->objectName().isEmpty())
            About->setObjectName(QStringLiteral("About"));
        About->resize(573, 257);
        About->setStyleSheet(QLatin1String("border-radius:5px;\n"
"background-color:rgb(86, 76, 75,20);"));
        gridLayout_3 = new QGridLayout(About);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        softerwareVersion = new QLabel(About);
        softerwareVersion->setObjectName(QStringLiteral("softerwareVersion"));
        softerwareVersion->setMinimumSize(QSize(531, 41));
        softerwareVersion->setMaximumSize(QSize(531, 41));
        softerwareVersion->setStyleSheet(QString::fromUtf8("font: 75 10pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";\n"
"font: 63 18pt \"Ubuntu\";\n"
"background-color:rgb(86, 76, 75,0);\n"
"color:rgb(255,255,255)"));

        gridLayout_3->addWidget(softerwareVersion, 0, 0, 1, 1);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        label = new QLabel(About);
        label->setObjectName(QStringLiteral("label"));
        label->setMinimumSize(QSize(111, 31));
        label->setMaximumSize(QSize(111, 31));
        label->setStyleSheet(QLatin1String("font: 63 14pt \"Ubuntu\";\n"
"color:rgb(255,255,255);\n"
"background-color:rgb(86, 76, 75,0);"));

        horizontalLayout_2->addWidget(label);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);


        gridLayout->addLayout(horizontalLayout_2, 0, 0, 1, 1);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_5);

        ROM_usage = new QProgressBar(About);
        ROM_usage->setObjectName(QStringLiteral("ROM_usage"));
        ROM_usage->setMinimumSize(QSize(151, 31));
        ROM_usage->setMaximumSize(QSize(151, 31));
        ROM_usage->setStyleSheet(QLatin1String("border: 2px solid gray;\n"
"border-radius: 5px;\n"
"background-color: #FFFFFF;\n"
"   border-radius: 5px;\n"
"   text-align: center;\n"
"chunk {\n"
"   background-color: #05B8CC;\n"
"   width: 20px;\n"
"}\n"
"color:rgb(255,255,255)"));
        ROM_usage->setValue(24);

        horizontalLayout_3->addWidget(ROM_usage);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_6);


        gridLayout->addLayout(horizontalLayout_3, 1, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_3);

        formatButton = new QPushButton(About);
        formatButton->setObjectName(QStringLiteral("formatButton"));
        formatButton->setMinimumSize(QSize(91, 41));
        formatButton->setMaximumSize(QSize(91, 41));
        formatButton->setStyleSheet(QLatin1String("QPushButton{\n"
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
        formatButton->setFlat(false);

        horizontalLayout->addWidget(formatButton);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_4);


        gridLayout->addLayout(horizontalLayout, 2, 0, 1, 1);


        gridLayout_3->addLayout(gridLayout, 1, 0, 1, 1);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        logLevelLabel = new QLabel(About);
        logLevelLabel->setObjectName(QStringLiteral("logLevelLabel"));
        logLevelLabel->setMinimumSize(QSize(135, 35));
        logLevelLabel->setMaximumSize(QSize(135, 35));
        logLevelLabel->setStyleSheet(QLatin1String("color:rgb(255,255,255);\n"
"background-color:rgb(86, 76, 75,0);"));
        logLevelLabel->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(logLevelLabel, 0, 0, 1, 1);

        logLevelComboBox = new QComboBox(About);
        logLevelComboBox->setObjectName(QStringLiteral("logLevelComboBox"));
        logLevelComboBox->setMinimumSize(QSize(135, 35));
        logLevelComboBox->setMaximumSize(QSize(135, 35));
        logLevelComboBox->setLayoutDirection(Qt::LeftToRight);
        logLevelComboBox->setStyleSheet(QLatin1String("QComboBox::drop-down {\n"
"	subcontrol-origin: padding;\n"
"	subcontrol-position: top right;\n"
"	width: 15px; \n"
"	border-left-width: 1px;\n"
"	border-left-style: solid;\n"
"	border-top-right-radius: 3px;\n"
"	border-bottom-right-radius: 3px;\n"
"	border-left-color: #50A3F0;\n"
"}\n"
"\n"
"QComboBox::down-arrow {\n"
"	border-image: url(:/icon/arrowdown.png); \n"
"}\n"
"QComboBox{border: 2px solid rgb(100, 100, 100);\n"
"text-align: center;\n"
"text-color:rgb(255,255,255); \n"
"border-radius: 4px; \n"
"color:rgb(255,255,255);\n"
"}\n"
""));

        gridLayout_2->addWidget(logLevelComboBox, 0, 1, 1, 1);


        gridLayout_3->addLayout(gridLayout_2, 2, 0, 1, 1);


        retranslateUi(About);

        QMetaObject::connectSlotsByName(About);
    } // setupUi

    void retranslateUi(QDialog *About)
    {
        About->setWindowTitle(QApplication::translate("About", "Dialog", 0));
        softerwareVersion->setText(QApplication::translate("About", "\350\275\257\344\273\266\347\211\210\346\234\254\357\274\232T3-linux-sdk-V1.3.5", 0));
        label->setText(QApplication::translate("About", "SD\345\215\241\345\256\271\351\207\217", 0));
        formatButton->setText(QApplication::translate("About", "\346\240\274\345\274\217\345\214\226", 0));
        logLevelLabel->setText(QApplication::translate("About", "LOG\346\211\223\345\215\260\347\255\211\347\272\247\350\256\276\347\275\256", 0));
        logLevelComboBox->clear();
        logLevelComboBox->insertItems(0, QStringList()
         << QApplication::translate("About", "0", 0)
         << QApplication::translate("About", "1", 0)
         << QApplication::translate("About", "2", 0)
         << QApplication::translate("About", "3", 0)
         << QApplication::translate("About", "4", 0)
         << QApplication::translate("About", "5", 0)
         << QApplication::translate("About", "6", 0)
        );
    } // retranslateUi

};

namespace Ui {
    class About: public Ui_About {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ABOUT_H
