#ifndef FRM_ONLINEUSERIMPL_H
#define FRM_ONLINEUSERIMPL_H
//
#include <QWidget>
#include "ui_frm_onlineUser.h"
//
class frm_onlineUserImpl : public QWidget, public Ui::frm_onlineUser
{
Q_OBJECT
public:
	frm_onlineUserImpl( QWidget * parent = 0, Qt::WindowFlags f = 0 );
private slots:
};
#endif





