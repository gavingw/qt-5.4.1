#ifndef FRM_ALARMSETTINGSIMPL_H
#define FRM_ALARMSETTINGSIMPL_H
//
#include <QWidget>
#include "ui_frm_alarmsettings.h"
//
class frm_alarmsettingsImpl : public QWidget, public Ui::frm_alarmsettings
{
Q_OBJECT
public:
	frm_alarmsettingsImpl( QWidget * parent = 0, Qt::WindowFlags f = 0 );
private slots:
};
#endif





