#ifndef FRM_SEARCHFILEIMPL_H
#define FRM_SEARCHFILEIMPL_H
//
#include <QWidget>
#include "ui_frm_searchfile.h"
//
class frm_searchfileImpl : public QWidget, public Ui::frm_searchfile
{
Q_OBJECT
public:
	frm_searchfileImpl( QWidget * parent = 0, Qt::WindowFlags f = 0 );
private slots:
};
#endif





