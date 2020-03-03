#ifndef FRM_UPDATEIMPL_H
#define FRM_UPDATEIMPL_H
//
#include <QWidget>
#include "ui_frm_Update.h"
//
class frm_UpdateImpl : public QWidget, public Ui::frm_Update
{
Q_OBJECT
public:
	frm_UpdateImpl( QWidget * parent = 0, Qt::WindowFlags f = 0 );
private slots:
    void on_buttonBox_clicked(QAbstractButton *button);
    void on_comboBox_currentIndexChanged(int index);

    void on_comboBox_2_currentIndexChanged(int index);
};
#endif





