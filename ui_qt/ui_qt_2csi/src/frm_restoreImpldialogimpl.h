#ifndef FRM_RESTOREIMPLDIALOGIMPL_H
#define FRM_RESTOREIMPLDIALOGIMPL_H
//
#include <QDialog>
#include "ui_frm_restore.h"
//
class frm_restoreImpl : public QDialog, public Ui::Dialog
{
Q_OBJECT
public:
	frm_restoreImpl( QWidget * parent = 0, Qt::WindowFlags f = 0 );
private slots:
    void on_buttonBox_clicked(QAbstractButton *button);
};
#endif





