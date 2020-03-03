#ifndef FRM_RECORDSETTINGSIMPL_H
#define FRM_RECORDSETTINGSIMPL_H
//
#include <QWidget>
#include "ui_frm_recordsettings.h"
//
class frm_recordsettingsImpl : public QWidget, public Ui::frm_recordsettings
{
Q_OBJECT
public:
	frm_recordsettingsImpl( QWidget * parent = 0, Qt::WindowFlags f = 0 );
private slots:
};
#endif






