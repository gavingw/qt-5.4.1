#ifndef FRM_QUERYLOGIMPL_H
#define FRM_QUERYLOGIMPL_H
//
#include <QWidget>
#include "ui_frm_queryLog.h"
//
class frm_queryLogImpl : public QWidget, public Ui::frm_queryLog
{
Q_OBJECT
public:
	frm_queryLogImpl( QWidget * parent = 0, Qt::WindowFlags f = 0 );
private slots:
};
#endif





