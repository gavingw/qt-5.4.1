#ifndef FRM_INOUTIMPL_H
#define FRM_INOUTIMPL_H
//
#include <QWidget>
#include "ui_frm_inOut.h"
//
class frm_inOutImpl : public QWidget, public Ui::frm_inOut
{
Q_OBJECT
public:
	frm_inOutImpl( QWidget * parent = 0, Qt::WindowFlags f = 0 );
private slots:
};
#endif





