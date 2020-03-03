/********************************************************************************
** Form generated from reading UI file 'slave_videowidget.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SLAVE_VIDEOWIDGET_H
#define UI_SLAVE_VIDEOWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QListWidget>

QT_BEGIN_NAMESPACE

class Ui_slave_VideoWidget
{
public:
    QGridLayout *gridLayout;
    QListWidget *listWidget;

    void setupUi(QDialog *slave_VideoWidget)
    {
        if (slave_VideoWidget->objectName().isEmpty())
            slave_VideoWidget->setObjectName(QStringLiteral("slave_VideoWidget"));
        slave_VideoWidget->resize(656, 532);
        gridLayout = new QGridLayout(slave_VideoWidget);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        listWidget = new QListWidget(slave_VideoWidget);
        listWidget->setObjectName(QStringLiteral("listWidget"));

        gridLayout->addWidget(listWidget, 0, 0, 1, 1);


        retranslateUi(slave_VideoWidget);

        QMetaObject::connectSlotsByName(slave_VideoWidget);
    } // setupUi

    void retranslateUi(QDialog *slave_VideoWidget)
    {
        slave_VideoWidget->setWindowTitle(QApplication::translate("slave_VideoWidget", "Dialog", 0));
    } // retranslateUi

};

namespace Ui {
    class slave_VideoWidget: public Ui_slave_VideoWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SLAVE_VIDEOWIDGET_H
