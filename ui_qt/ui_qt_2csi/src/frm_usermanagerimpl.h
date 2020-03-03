#ifndef FRM_USERMANAGERIMPL_H
#define FRM_USERMANAGERIMPL_H
//
#include <QWidget>
#include "ui_frm_userManager.h"
//
class frm_userManagerImpl : public QWidget, public Ui::frm_userManager
{
Q_OBJECT
public:
	frm_userManagerImpl( QWidget * parent = 0, Qt::WindowFlags f = 0 );
private slots:
};
#endif





