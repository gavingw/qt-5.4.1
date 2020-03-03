#ifndef FRM_SYSTIMEIMPL_H
#define FRM_SYSTIMEIMPL_H
//
#include <QWidget>
#include "ui_frm_systime.h"
//
class frm_systimeImpl : public QWidget, public Ui::frm_systime
{
Q_OBJECT
public:
	frm_systimeImpl( QWidget * parent = 0, Qt::WindowFlags f = 0 );
private slots:
};
#endif





