#ifndef FRM_NETSETTINGSIMPL_H
#define FRM_NETSETTINGSIMPL_H
//
#include <QWidget>
#include "ui_frm_netSettings.h"
//
class frm_netSettingsImpl : public QWidget, public Ui::frm_netSettings
{
Q_OBJECT
public:
	frm_netSettingsImpl( QWidget * parent = 0, Qt::WindowFlags f = 0 );
private slots:
    void on_buttonBox_clicked(QAbstractButton *button);
};
#endif







