#ifndef FRM_SETTINGIMPL_H
#define FRM_SETTINGIMPL_H

#include <QWidget>
#include "ui_frm_Settings.h"
class frm_settingimpl : public QWidget,public Ui::frm_imageSettings
{
    Q_OBJECT
public:
    explicit frm_settingimpl(QWidget *parent = 0);
    ~frm_settingimpl();

signals:
private:
    int m_startup;
    int m_horizontal;
    int m_vertical;
    int m_cycletime;
    int m_adas_en;
    char m_filedir[128];
    int m_tvout_type;


public slots:
private slots:
    void on_comboBox_5_currentIndexChanged(int index);
    void on_comboBox_3_currentIndexChanged(int index);
    void on_comboBox_4_currentIndexChanged(int index);
    void on_comboBox_6_currentIndexChanged(int index);
    void on_buttonBox_clicked(QAbstractButton *button);
    void on_pushButton_clicked();
    void on_comboBox_7_currentIndexChanged(int index);
};

#endif // FRM_SETTINGIMPL_H
