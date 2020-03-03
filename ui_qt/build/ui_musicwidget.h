/********************************************************************************
** Form generated from reading UI file 'musicwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MUSICWIDGET_H
#define UI_MUSICWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QListWidget>

QT_BEGIN_NAMESPACE

class Ui_MusicWidget
{
public:
    QGridLayout *gridLayout;
    QListWidget *listWidget;

    void setupUi(QDialog *MusicWidget)
    {
        if (MusicWidget->objectName().isEmpty())
            MusicWidget->setObjectName(QStringLiteral("MusicWidget"));
        MusicWidget->resize(761, 551);
        gridLayout = new QGridLayout(MusicWidget);
        gridLayout->setSpacing(0);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        listWidget = new QListWidget(MusicWidget);
        listWidget->setObjectName(QStringLiteral("listWidget"));
        listWidget->setStyleSheet(QLatin1String("QListWidget::scrollBar:vertical\n"
"{\n"
"    width:8px;\n"
"    background:rgba(0,0,0,0%);\n"
"    margin:0px,0px,0px,0px;\n"
"    padding-top:9px; \n"
"    padding-bottom:9px;\n"
"};\n"
"QListWidget::scrollBar::handle:vertical\n"
"{\n"
"    width:8px;\n"
"    background:rgba(0,0,0,25%);\n"
"    border-radius:4px;\n"
"    min-height:20;\n"
"};\n"
"QListWidget::scrollBar::handle:vertical:hover\n"
"{\n"
"    width:8px;\n"
"    background:rgba(0,0,0,50%);\n"
"    border-radius:4px;\n"
"    min-height:20;\n"
"};\n"
"QListWidget::scrollBar::add-line:vertical \n"
"{\n"
"    height:9px;width:8px;\n"
"    border-image:url(:/images/a/3.png);\n"
"    subcontrol-position:bottom;\n"
"};\n"
"QListWidget::scrollBar::sub-line:vertical \n"
"{\n"
"    height:9px;width:8px;\n"
"    border-image:url(:/images/a/1.png);\n"
"    subcontrol-position:top;\n"
"};\n"
"QListWidget::scrollBar::add-line:vertical:hover\n"
"{\n"
"    height:9px;width:8px;\n"
"    border-image:url(:/images/a/4.png);\n"
"    subcontrol-position:bottom;\n"
"}"
                        ";\n"
"QListWidget::scrollBar::sub-line:vertical:hover \n"
"{\n"
"    height:9px;width:8px;\n"
"    border-image:url(:/images/a/2.png);\n"
"    subcontrol-position:top;\n"
"};\n"
"QListWidget::scrollBar::sub-page:vertical   \n"
"{\n"
"    background:rgba(0,0,0,10%);\n"
"    border-radius:4px;\n"
"};\n"
"QListWidget::scrollBar::add-page:vertical \n"
"{\n"
"	background:rgba(0,0,0,10%);\n"
"    border-radius:4px;\n"
"}"));

        gridLayout->addWidget(listWidget, 0, 0, 1, 1);


        retranslateUi(MusicWidget);

        QMetaObject::connectSlotsByName(MusicWidget);
    } // setupUi

    void retranslateUi(QDialog *MusicWidget)
    {
        MusicWidget->setWindowTitle(QApplication::translate("MusicWidget", "Dialog", 0));
    } // retranslateUi

};

namespace Ui {
    class MusicWidget: public Ui_MusicWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MUSICWIDGET_H
