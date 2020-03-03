/********************************************************************************
** Form generated from reading UI file 'slave_picturewidget.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SLAVE_PICTUREWIDGET_H
#define UI_SLAVE_PICTUREWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QListWidget>

QT_BEGIN_NAMESPACE

class Ui_slave_PictureWidget
{
public:
    QGridLayout *gridLayout;
    QListWidget *listWidget_file;

    void setupUi(QDialog *slave_PictureWidget)
    {
        if (slave_PictureWidget->objectName().isEmpty())
            slave_PictureWidget->setObjectName(QStringLiteral("slave_PictureWidget"));
        slave_PictureWidget->resize(656, 532);
        gridLayout = new QGridLayout(slave_PictureWidget);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        listWidget_file = new QListWidget(slave_PictureWidget);
        listWidget_file->setObjectName(QStringLiteral("listWidget_file"));

        gridLayout->addWidget(listWidget_file, 0, 0, 1, 1);


        retranslateUi(slave_PictureWidget);

        QMetaObject::connectSlotsByName(slave_PictureWidget);
    } // setupUi

    void retranslateUi(QDialog *slave_PictureWidget)
    {
        slave_PictureWidget->setWindowTitle(QApplication::translate("slave_PictureWidget", "Dialog", 0));
    } // retranslateUi

};

namespace Ui {
    class slave_PictureWidget: public Ui_slave_PictureWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SLAVE_PICTUREWIDGET_H
