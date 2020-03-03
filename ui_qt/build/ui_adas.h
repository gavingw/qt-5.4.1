/********************************************************************************
** Form generated from reading UI file 'adas.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ADAS_H
#define UI_ADAS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ADAS
{
public:
    QGridLayout *gridLayout_3;
    QGridLayout *gridLayout_2;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_4;
    QWidget *widget;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_5;
    QWidget *widget_2;
    QLabel *label;
    QLabel *label_2;
    QGridLayout *gridLayout;
    QLabel *pictureLabel2;
    QSpacerItem *horizontalSpacer;

    void setupUi(QDialog *ADAS)
    {
        if (ADAS->objectName().isEmpty())
            ADAS->setObjectName(QStringLiteral("ADAS"));
        ADAS->resize(660, 430);
        gridLayout_3 = new QGridLayout(ADAS);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        gridLayout_3->setContentsMargins(15, 50, 15, 50);
        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        groupBox = new QGroupBox(ADAS);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setStyleSheet(QString::fromUtf8("QGroupBox{\n"
"border-width:2px;\n"
"border-style:solid;\n"
"border-radius: 10px;\n"
"border-color:blue;\n"
"margin-top:0.5ex;\n"
"}\n"
"font: 75 10pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
        gridLayout_4 = new QGridLayout(groupBox);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        widget = new QWidget(groupBox);
        widget->setObjectName(QStringLiteral("widget"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
        widget->setSizePolicy(sizePolicy);
        widget->setMinimumSize(QSize(289, 194));
        widget->setMaximumSize(QSize(289, 194));

        gridLayout_4->addWidget(widget, 0, 0, 1, 1);


        gridLayout_2->addWidget(groupBox, 1, 0, 1, 1);

        groupBox_2 = new QGroupBox(ADAS);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setStyleSheet(QString::fromUtf8("QGroupBox{\n"
"border-width:2px;\n"
"border-style:solid;\n"
"border-radius: 10px;\n"
"border-color:blue;\n"
"margin-top:0.5ex;\n"
"}\n"
"font: 75 10pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
        gridLayout_5 = new QGridLayout(groupBox_2);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        widget_2 = new QWidget(groupBox_2);
        widget_2->setObjectName(QStringLiteral("widget_2"));
        sizePolicy.setHeightForWidth(widget_2->sizePolicy().hasHeightForWidth());
        widget_2->setSizePolicy(sizePolicy);
        widget_2->setMinimumSize(QSize(289, 194));
        widget_2->setMaximumSize(QSize(289, 194));

        gridLayout_5->addWidget(widget_2, 0, 0, 1, 1);


        gridLayout_2->addWidget(groupBox_2, 1, 1, 1, 1);

        label = new QLabel(ADAS);
        label->setObjectName(QStringLiteral("label"));
        label->setStyleSheet(QLatin1String("font: 63 11pt \"Ubuntu\";\n"
"color:rgb(255,255,255);"));
        label->setAlignment(Qt::AlignBottom|Qt::AlignHCenter);

        gridLayout_2->addWidget(label, 0, 0, 1, 1);

        label_2 = new QLabel(ADAS);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setStyleSheet(QLatin1String("font: 63 11pt \"Ubuntu\";\n"
"color:rgb(255,255,255);"));
        label_2->setAlignment(Qt::AlignBottom|Qt::AlignHCenter);

        gridLayout_2->addWidget(label_2, 0, 1, 1, 1);

        gridLayout_2->setRowStretch(0, 1);
        gridLayout_2->setRowStretch(1, 5);

        gridLayout_3->addLayout(gridLayout_2, 0, 0, 1, 1);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        pictureLabel2 = new QLabel(ADAS);
        pictureLabel2->setObjectName(QStringLiteral("pictureLabel2"));
        pictureLabel2->setMinimumSize(QSize(161, 31));
        pictureLabel2->setMaximumSize(QSize(161, 31));
        pictureLabel2->setStyleSheet(QStringLiteral("color:rgb(255,255,255);"));

        gridLayout->addWidget(pictureLabel2, 0, 0, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 0, 1, 1, 1);


        gridLayout_3->addLayout(gridLayout, 1, 0, 1, 1);

        gridLayout_3->setRowStretch(0, 5);
        gridLayout_3->setRowStretch(1, 1);

        retranslateUi(ADAS);

        QMetaObject::connectSlotsByName(ADAS);
    } // setupUi

    void retranslateUi(QDialog *ADAS)
    {
        ADAS->setWindowTitle(QApplication::translate("ADAS", "Dialog", 0));
        groupBox->setTitle(QString());
        groupBox_2->setTitle(QString());
        label->setText(QApplication::translate("ADAS", "\345\211\215\346\221\204\345\203\217ADAS\350\256\276\347\275\256", 0));
        label_2->setText(QApplication::translate("ADAS", "\345\220\216\346\221\204\345\203\217\345\244\264ADAS\350\256\276\347\275\256", 0));
        pictureLabel2->setText(QApplication::translate("ADAS", " \346\221\204\345\203\217\345\244\264\345\210\266\345\274\217\357\274\232", 0));
    } // retranslateUi

};

namespace Ui {
    class ADAS: public Ui_ADAS {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ADAS_H
