#ifndef FRM_YUNTAISETTINGSIMPL_H
#define FRM_YUNTAISETTINGSIMPL_H
//
#include <QWidget>
#include "ui_frm_yuntaisettings.h"
//
class frm_yuntaisettingsImpl : public QWidget, public Ui::frm_yuntaisettings
{
Q_OBJECT
public:
	frm_yuntaisettingsImpl( QWidget * parent = 0, Qt::WindowFlags f = 0 );
private slots:
    void on_buttonBox_clicked(QAbstractButton *button);
};
#endif





