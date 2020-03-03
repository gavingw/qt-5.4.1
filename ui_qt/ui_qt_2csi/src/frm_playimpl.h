#ifndef FRM_PLAYIMPL_H
#define FRM_PLAYIMPL_H

#include <QWidget>

#if defined(Q_OS_LINUX)
#include "CameraDebug.h"

#include <sdklog.h>

#include "V4L2CameraDevice2.h"
#include "CallbackNotifier.h"
#include "PreviewWindow.h"
#include "CameraHardware2.h"
#include "HALCameraFactory.h"
#include "CameraHardwareInterface.h"
#include "DvrFactory.h"

using namespace android;
#endif
namespace Ui {
class Play;
}
#if defined(Q_OS_LINUX)
#define SUPPORT_CAMERA_NUM 2
//char *cameraname[SUPPORT_CAMERA_NUM]={"0","1"};
class DvrCamera
{
public:
    dvr_factory *dvr;

    int setRecord(bool flag ){isRecord=flag;return 0;}
    int setPaused(bool flag){isPaused=flag;return 0;}
    int setPreview(bool flag){isPreview=flag;return 0;}

    bool getRecord(){return isRecord;}
    bool getPaused(){return isPaused;}
    bool getPreview(){return isPreview;}
    DvrCamera()
    {
        isRecord=false;
        isPaused=false;
        isPreview=false;
    }
 private:
    bool isRecord;
    bool isPaused;
    bool isPreview;
};
#endif
class Play : public QWidget
{
    Q_OBJECT

public:
    explicit Play(QWidget *parent = 0);
    ~Play();
    #if defined(Q_OS_LINUX)
    //sp<CameraHardwareInterface>     mHardware;
    //dvr_factory *dvr;
    //dvr_factory *dvr1;
    struct view_info mvv;
    DvrCamera dvrCamera[SUPPORT_CAMERA_NUM];
    int setCameraDispRect(){
        //mvv.x=
        //mvv.y=
        //mvv.w=
        //mvv.h=


    }
    #endif
    int  startAllCameraWithPreview(int camera_no /* nouse now*/);
    int cur_camera;
    void videoshow()
    {
        //on_pushButton_play_clicked();
        startAllCameraWithPreview(0);
    }
    void startCurCamera()
    {
        on_pushButton_play_clicked();
    }
    bool isaf;

    #if defined(Q_OS_LINUX)
    Mutex                           mObjectLock;
    #endif
    void startRecord();
    void stopRecord(){
        on_pushButton_stop_clicked();
    }
private slots:
    void on_pushButton_play_clicked();

    void on_pushButton_openfile_clicked();

    void on_pushButton_pause_clicked();

    void on_pushButton_stop_clicked();

    void on_pushButton_home_clicked();

    void on_pushButton_tackpic_clicked();

    void on_horizontalSlider_4_valueChanged(int value);


    void on_pushButton_switchC_clicked();

private:
    Ui::Play *ui;
};

#endif // FRM_PLAYIMPL_H
