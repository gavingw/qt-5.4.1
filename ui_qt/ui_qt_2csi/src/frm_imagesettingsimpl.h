#ifndef FRM_IMAGESETTINGSIMPL_H
#define FRM_IMAGESETTINGSIMPL_H
//
#include <QWidget>
#include "ui_frm_imageSettings.h"
//
class frm_imageSettingsImpl : public QWidget, public Ui::frm_imageSettings
{
Q_OBJECT
public:
	frm_imageSettingsImpl( QWidget * parent = 0, Qt::WindowFlags f = 0 );
private slots:
};
#endif






