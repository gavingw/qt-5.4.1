#ifndef FRM_MODIFYPASSIMPL_H
#define FRM_MODIFYPASSIMPL_H
//
#include <QWidget>
#include "ui_frm_ModifyPass.h"
//
class frm_ModifyPassImpl : public QWidget, public Ui::frm_ModifyPass
{
Q_OBJECT
public:
	frm_ModifyPassImpl( QWidget * parent = 0, Qt::WindowFlags f = 0 );
private slots:
};
#endif





