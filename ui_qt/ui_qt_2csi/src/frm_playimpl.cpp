#include "frm_playimpl.h"
#include "ui_frm_play.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QTimer>
#include <QGraphicsScene>
#include <QGraphicsView>
#include "mainwindowimpl.h"
#include "CameraFileCfg.h"
//#define USE_AW_360
//#define AAA
//#define SUPPORT_RTSP 1
#define HAVA_TWO_CAMERA 1  //While move to cfg
#define CAMERA_FONT 0  //While move to cfg
#define CAMERA_BACK 1 //While move to cfg

Play *pstaticthis=NULL;//fucking bad impl for single object
extern MainWindowImpl *pMainWindowImpl;
#if defined(Q_OS_LINUX)
using namespace android;
void usernotifyCallback(int32_t msgType, int32_t ext1,
                        int32_t ext2, void* user)
{
//printf("cb------------------%p",user);
    if ((msgType&CAMERA_MSG_ERROR) ==CAMERA_MSG_ERROR)
    {
        ALOGE("(msgType =CAMERA_MSG_ERROR)");

    }

    if ((msgType&CAMERA_MSG_DVR_STORE_ERR) ==CAMERA_MSG_DVR_STORE_ERR)
    {
        ALOGE("msgType =CAMERA_MSG_DVR_STORE_ERR)");

        dvr_factory *p_dvr=(dvr_factory *)user;
        p_dvr->storage_state=0;//tmp
        p_dvr->stopRecord();
        if(pstaticthis!=NULL)
        {
            if(p_dvr->mCameraId<SUPPORT_CAMERA_NUM)
            {
                if(pstaticthis->dvrCamera[p_dvr->mCameraId].getRecord())
                    pstaticthis->dvrCamera[p_dvr->mCameraId].setRecord(false);//this is needed ,usrs must sync this ctrl status by self
            }else
            {
                int i;
                for(i=0;i<SUPPORT_CAMERA_NUM;i++){
                    if(p_dvr->mCameraId==pstaticthis->dvrCamera[i].dvr->mCameraId){
                        if(pstaticthis->dvrCamera[i].getRecord())
                            pstaticthis->dvrCamera[i].setRecord(false);//this is needed ,usrs must sync this ctrl status by self
                    }
                }
            }
        }

    }

//return NO_ERROR;

}
void userdataCallback(int32_t msgType,
                      char* dataPtr, camera_frame_metadata_t *metadata, void* user)
{
    //return NO_ERROR;
}

void userdataCallbackTimestamp(nsecs_t timestamp,
                               int32_t msgType, char* dataPtr, void* user) {}


status_t usr_h264datacb(int32_t msgType,
                        char *dataPtr,int dalen,
                        void *user)
{


}
#endif

#define LOG_BUF_SIZE	1024
#define VIEW_WEITH 720
#define VIEW_HEIGHT 480

Play::Play(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Play)
{
    ui->setupUi(this);
    //this->setAttribute(Qt::WA_TranslucentBackground,true);
#ifdef QT_ROTATE_APP
    QGraphicsScene *scene = new QGraphicsScene();
    QGraphicsView *view = new QGraphicsView(parent);
    view->setGeometry(0,0,800,480); // actual Display size
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    QGraphicsProxyWidget *proxy = scene->addWidget(this);
    view->setScene(scene);
    view->show();
    view->rotate(180);
#endif
    setProperty("noinput",true);
    isaf=false;
#ifdef AAA
    setStyleSheet(QStringLiteral("background-color: rgb(112, 200, 11);"));
#endif
    #if defined(Q_OS_LINUX)

    HwDisplay* mcd=NULL;
    //test screen mode
    //0 disable
    int tvmode=0;
    //config_set_tvout(0,tvmode);
    mcd=HwDisplay::getInstance();
    tvmode=config_get_tvout(0);
    printf("--------------tvmode =%d \r\n",tvmode);
    mcd->hwd_screen1_mode(tvmode);
    //mcd->hwd_tvout();
    cur_camera = 0 ;
    pgEmulatedCameraFactory=new HALCameraFactory();
    //sleep(4);
    int i;
    for(i=0;i<SUPPORT_CAMERA_NUM;i++)
        dvrCamera[i].dvr=new dvr_factory();

#endif
    printf("Play----------%p----\r\n",this);
    pstaticthis=this;//fucking bad,single obj only
}
void Play::startRecord()
{
#if defined(Q_OS_LINUX)
    //int cycltime;
    char bufname[512];
   // int cx,cy;
    printf("startRecord--------------\r\n");
    Mutex::Autolock locker(&mObjectLock);

    dvr_factory * p_dvr;//=dvrCamera[i].dvr;
    int i;
    for(i=0;i<SUPPORT_CAMERA_NUM;i++)
    {
        p_dvr=dvrCamera[i].dvr;
        printf("get camera startup flag=%d \r\n",config_get_startup(i));
        if(1==config_get_startup(i))
        {
            if(dvrCamera[i].getPreview()){
                p_dvr->startRecord();
                isaf=true;
                dvrCamera[i].setRecord(true);
            }else{
                int rt ;
                if(dvrCamera[i].getRecord())
                 {continue ;}
            #ifdef USE_AW_360
                //config_set_heigth(360,960);
                //config_set_weith(360,1440);
                rt= p_dvr->recordInit("360");
            #else
                //config_set_heigth(0,720);
                //config_set_weith(0,1280);
                sprintf(bufname,"%d",i);
                rt= p_dvr->recordInit(bufname);
            #endif
                if(rt <0){
                    printf("init record fail camera[%s] ret =%d \r\n",bufname,rt);
                    continue;
                }
                p_dvr->SetDataCB(usr_h264datacb,p_dvr);
                p_dvr->setCallbacks(usernotifyCallback,userdataCallback,userdataCallbackTimestamp,p_dvr/* must dvr obj*/ /*dump*/);
                //dvr->prepare();
                p_dvr->start();
                p_dvr->enableWaterMark();
                sprintf(bufname,"64,64,0,64,250,T3L SDK,64,450,ASTEROID V1 alpha");
                p_dvr->setWaterMarkMultiple(bufname);
        #ifdef SUPPORT_RTSP
                p_dvr->set_rtsp_to_file(0);//test !!!! if 1 wirite to encode stream to file . must start before start_rtsp

                p_dvr->start_rtsp();
        #endif
                p_dvr->startRecord();
                isaf = true;
                dvrCamera[i].setRecord(true);
            }
        }
    }

#endif
}

Play::~Play()
{
    printf("~Play--------------\r\n");
    delete ui;
     #if defined(Q_OS_LINUX)
    //delete dvr;
    int i;
    for(i=0;i<SUPPORT_CAMERA_NUM;i++)
        delete dvrCamera[i].dvr;
    delete pgEmulatedCameraFactory;
#endif
}

void Play::on_pushButton_play_clicked()
{
    //sp<CameraHardwareInterface>     mHardware;
    printf("camera play %p\r\n",this);

#if defined(Q_OS_LINUX)
    Mutex::Autolock locker(&mObjectLock);
    //int cycltime;
    char bufname[512];
    //int cx,cy;
    bool rt;
    dvr_factory * p_dvr=dvrCamera[cur_camera].dvr;
    if(!dvrCamera[cur_camera].getRecord())
    {
        if(dvrCamera[cur_camera].getPreview()){
            p_dvr->startRecord();
            isaf=true;
            dvrCamera[cur_camera].setRecord(true);
        }else{
            int rt ;
        #ifdef USE_AW_360
            config_set_heigth(360,960);
            config_set_weith(360,1440);
            rt= p_dvr->recordInit("360");
        #else
            //config_set_heigth(0,720);
            //config_set_weith(0,1280);
            sprintf(bufname,"%d",cur_camera);
            rt= p_dvr->recordInit(bufname);
        #endif
            if(rt <0)
                return;
            p_dvr->SetDataCB(usr_h264datacb,p_dvr);

            //dvr->prepare();
            p_dvr->start();
            p_dvr->enableWaterMark();
            sprintf(bufname,"64,64,0,64,250,T2L SDK,64,450,RAINBOW V0.3a");
            p_dvr->setWaterMarkMultiple(bufname);
    #ifdef AAA
            setStyleSheet(QStringLiteral("background-color: rgba(112, 200, 11, 0);"));
    #endif
    #ifdef SUPPORT_RTSP
            p_dvr->set_rtsp_to_file(0);//test !!!! if 1 wirite to encode stream to file . must start before start_rtsp
            p_dvr->start_rtsp();
    #endif
            p_dvr->startRecord();
            isaf=true;
            dvrCamera[cur_camera].setRecord(true);
        }
    }
    else
    {
        //Play->setStyleSheet(QStringLiteral("background-color: rgba(112, 200, 11, 0);"));
        //setStyleSheet(QStringLiteral("background-color: rgb(112, 200, 11);"));
    }

    if(!dvrCamera[cur_camera].getPreview())
    {
        struct view_info vv= {0,0,VIEW_WEITH,VIEW_HEIGHT};
        //ALOGV("vx=%d vy=%d sx=%d sy%d",mcd->lcdxres,mcd->lcdyres,dvr->recordwith,dvr->recordheith);
        p_dvr->startPriview(vv);
#ifdef AAA
        setStyleSheet(QStringLiteral("background-color: rgba(112, 200, 11, 0);"));
#endif
        //isPreview=true;
        dvrCamera[cur_camera].setPreview(true);
    }

#endif
    //isRecord=true;
}

int Play::startAllCameraWithPreview(int camera_no /* nouse now*/)
{
    //sp<CameraHardwareInterface>     mHardware;
    printf("startAllCameraWithPreview play %p\r\n",this);
    camera_no=camera_no;

#if defined(Q_OS_LINUX)
   startRecord();
   printf("startAllCameraWithPreview play %d\r\n",cur_camera);
   dvr_factory *p_dvr=dvrCamera[cur_camera].dvr;

    if((!dvrCamera[cur_camera].getPreview())&&(dvrCamera[cur_camera].getRecord()))
    {
        struct view_info vv= {0,0,VIEW_WEITH,VIEW_HEIGHT};
        //ALOGV("vx=%d vy=%d sx=%d sy%d",mcd->lcdxres,mcd->lcdyres,dvr->recordwith,dvr->recordheith);
        p_dvr->startPriview(vv);
        #ifdef AAA
        setStyleSheet(QStringLiteral("background-color: rgba(112, 200, 11, 0);"));
#endif
        dvrCamera[cur_camera].setPreview(true);
    }

#endif
    return 0;
}
void Play::on_pushButton_openfile_clicked()
{
    QString fileName;
    QString m_currentPath;
    printf("camera openfile \r\n");
    return;
    bool isafb=isaf;
    if(isafb)
    {
#ifdef AAA
        setStyleSheet(QStringLiteral("background-color: rgb(112, 200, 11);"));
#endif
        isaf=false;
    }

#if defined(Q_OS_LINUX)
    Mutex::Autolock locker(&mObjectLock);
    dvr_factory *p_dvr=dvrCamera[cur_camera].dvr;
    bool preback=dvrCamera[cur_camera].getPreview();
    if(dvrCamera[cur_camera].getPreview())
    {
        p_dvr->stopPriview();
        dvrCamera[cur_camera].setPreview(false);
    }
#endif

    QFileDialog *fileDialog = new QFileDialog(this);
    //fileDialog->setProperty("noinput",true);
    fileDialog->setWindowTitle(tr("Open Video"));
    fileDialog->setDirectory(".");
    fileDialog->setNameFilter(tr("video files (*.mp4)"));
    if(fileDialog->exec() == QDialog::Accepted)
    {
        fileName = fileDialog->selectedFiles()[0];
        // QMessageBox::information(NULL, tr("Path"), tr("You selected ") + path);
    }
    else
    {
        // QMessageBox::information(NULL, tr("Path"), tr("You didn't select any files."));
    }


    if (!fileName.isEmpty())
    {
        QFile file(fileName);
        if (!file.exists())
        {
            QMessageBox::critical(this, tr("Open video File"),
                                  QString("Could not open file '%1'.").arg(fileName));

            return;
        }

        if (!fileName.startsWith(":/"))
        {
            m_currentPath = fileName;
        }

    }
    else
    {
#if defined(Q_OS_LINUX)
        if(preback)
        {
            struct view_info vv= {0,0,VIEW_WEITH,VIEW_HEIGHT};
            p_dvr->startPriview(vv);
            dvrCamera[cur_camera].setPreview(true);

        }
#endif
    }
    if(isafb)
    {
#ifdef AAA
        setStyleSheet(QStringLiteral("background-color: rgba(112, 200, 11, 0);"));
#endif
        isaf=true;
    }
}

void Play::on_pushButton_pause_clicked()
{
#if defined(Q_OS_LINUX)

#endif
}

void Play::on_pushButton_stop_clicked()
{
    printf("camera stop \r\n");
    if(isaf)
    {
#ifdef AAA
        setStyleSheet(QStringLiteral("background-color: rgb(112, 200, 11);"));
#endif
        isaf=false;
    }
#if defined(Q_OS_LINUX)
    Mutex::Autolock locker(&mObjectLock);
    dvr_factory * p_dvr;
    int i;
    for(i=0;i<SUPPORT_CAMERA_NUM;i++)
    {
        p_dvr=dvrCamera[i].dvr;
        if(dvrCamera[i].getRecord()||dvrCamera[i].getPreview())
        {
            printf("camera stop ----\r\n");
    #ifdef SUPPORT_RTSP
            p_dvr->stop_rtsp();
    #endif
            if(dvrCamera[i].getPreview())
            {
                p_dvr->stopPriview();
                //isPreview=false;
                dvrCamera[i].setPreview(false);
            }

            if(dvrCamera[i].getRecord())
            {
                p_dvr->stopRecord();
                //isRecord=false;
                dvrCamera[i].setRecord(false);
            }

            p_dvr->stop();
            p_dvr->enc_de_init();
            p_dvr->uninitializeDev();

            //isRecord=false;
            //isPreview=false;
            dvrCamera[i].setPreview(false);
            dvrCamera[i].setRecord(false);
        }
    }
#endif
}

void Play::on_pushButton_home_clicked()
{
    printf("camera home \r\n");
#if defined(Q_OS_LINUX)
    Mutex::Autolock locker(&mObjectLock);
    dvr_factory * p_dvr=dvrCamera[cur_camera].dvr;
    if(dvrCamera[cur_camera].getPreview())
    {
        p_dvr->stopPriview();
        dvrCamera[cur_camera].setPreview(false);
    }
#endif
    pMainWindowImpl->setHidden(false);
    this->close();
}

void Play::on_pushButton_tackpic_clicked()
{
    printf("camera take pic \r\n");
#if defined(Q_OS_LINUX)
    Mutex::Autolock locker(&mObjectLock);
    if(dvrCamera[cur_camera].getRecord())
    {
        status_t rt=dvrCamera[cur_camera].dvr->takePicture();
        if(NO_ERROR!=rt)
            QMessageBox::critical(this, tr("takepic"), tr("takepic fail"));
        else
        {
            QMessageBox* box = new QMessageBox;
            QTimer::singleShot(1000, box, SLOT(close()));

            box->setWindowTitle(tr("takepic"));
            box->setStyleSheet(QStringLiteral("background-color: rgba(0, 200, 11,0);"));
            //box->setIcon(QMessageBox::Warning);
            box->setIcon(QMessageBox::NoIcon);
            box->setText(tr("pic success"));
            box->move(300,240);
            box->show();

        }
    }
#else

    QMessageBox* box = new QMessageBox;
    QTimer::singleShot(1000, box, SLOT(close()));

    box->setWindowTitle(tr("takepic"));
    box->setStyleSheet(QStringLiteral("background-color: rgb(0, 200, 11);"));

    box->setIcon(QMessageBox::NoIcon);
    box->setText(tr("pic success"));
    box->move(10,10);
    box->show();

#endif
}

void Play::on_horizontalSlider_4_valueChanged(int value)
{
    printf("-----x1-----=%d\r\n",value);
    //ui->label->setText(QString("X")+QString::number(value));

}

//switch camera
void Play::on_pushButton_switchC_clicked()
{
#if defined(Q_OS_LINUX)
    if(SUPPORT_CAMERA_NUM<2)
        return;
    dvr_factory * p_dvr=dvrCamera[cur_camera].dvr;
    if(dvrCamera[cur_camera].getPreview()){
        p_dvr->stopPriview();
        dvrCamera[cur_camera].setPreview(false);
    }
    cur_camera = (cur_camera==0)?1:0;

    //int cycltime;
    char bufname[512];
    //int cx,cy;
    bool rt;
    p_dvr=dvrCamera[cur_camera].dvr;
    if(!dvrCamera[cur_camera].getRecord())
    {
           int rt ;
        #ifdef USE_AW_360
            config_set_heigth(360,960);
            config_set_weith(360,1440);
            rt= p_dvr->recordInit("360");
        #else
            //config_set_heigth(0,720);
            //config_set_weith(0,1280);
            sprintf(bufname,"%d",cur_camera);
            rt= p_dvr->recordInit(bufname);
        #endif
            if(rt <0)
                return;
            p_dvr->SetDataCB(usr_h264datacb,p_dvr);

            //dvr->prepare();
            p_dvr->start();
            p_dvr->enableWaterMark();
            sprintf(bufname,"64,64,0,64,250,T2L SDK,64,450,RAINBOW V0.3a");
            p_dvr->setWaterMarkMultiple(bufname);
    #ifdef AAA
            setStyleSheet(QStringLiteral("background-color: rgba(112, 200, 11, 0);"));
    #endif
    #ifdef SUPPORT_RTSP
            p_dvr->set_rtsp_to_file(0);//test !!!! if 1 wirite to encode stream to file . must start before start_rtsp
            p_dvr->start_rtsp();
    #endif
            p_dvr->startRecord();
            isaf=true;
            dvrCamera[cur_camera].setRecord(true);
    }


    if(!dvrCamera[cur_camera].getPreview())
    {
        struct view_info vv= {0,0,VIEW_WEITH,VIEW_HEIGHT};
        //ALOGV("vx=%d vy=%d sx=%d sy%d",mcd->lcdxres,mcd->lcdyres,dvr->recordwith,dvr->recordheith);
        p_dvr->startPriview(vv);
        #ifdef AAA
        setStyleSheet(QStringLiteral("background-color: rgba(112, 200, 11, 0);"));
#endif

        dvrCamera[cur_camera].setPreview(true);
    }
    sleep(1);//temp add here,must use timestamp for response gap,i will impl next version
#endif
}
