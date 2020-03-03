#ifndef FRM_EXCEPTHANDLEIMPL_H
#define FRM_EXCEPTHANDLEIMPL_H
//
#include <QWidget>
#include "ui_frm_excepthandle.h"
//
class frm_excepthandleImpl : public QWidget, public Ui::frm_excepthandle
{
Q_OBJECT
public:
	frm_excepthandleImpl( QWidget * parent = 0, Qt::WindowFlags f = 0 );
private slots:
};
#endif





