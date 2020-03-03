#ifndef QAUTPLAYER_H
#define QAUTPLAYER_H

#include <QWidget>
#include "ui_frm_autplayer.h"

#include <QFileDialog>
#if defined(Q_OS_LINUX)
#define USE_AUTPLAYER 1
#endif
#if defined(USE_AUTPLAYER)
#include "AutPlayer.h"
using namespace android;
#endif
static const int ASTATUS_STOPPED   = 0;
static const int ASTATUS_PREPARING = 1;
static const int ASTATUS_PREPARED  = 2;
static const int ASTATUS_PLAYING   = 3;
static const int ASTATUS_PAUSED    = 4;
static const int ASTATUS_SEEKING   = 5;

class QAutplayer : public QWidget,Ui::AutPlayer
{
    Q_OBJECT
public:
    explicit QAutplayer(QWidget *parent = 0);
    ~QAutplayer();
#if defined(USE_AUTPLAYER)
    AUTPlayer *ap;
#endif
#if defined(USE_AUTPLAYER)
    static int end(int32_t msgType, void *user);
#endif
//todo:warning no protected
    int astatus;
signals:

public slots:
private slots:
    void on_pushButton_openfile_clicked();
    void on_pushButton_play_clicked();
    void on_pushButton_stop_clicked();
    void on_pushButton_home_clicked();
private:
    Ui::AutPlayer *ui;
};

#endif // QAUTPLAYER_H
