#include "qautplayerimpl.h"
#include <QGraphicsScene>
#include <QGraphicsView>
#include "mainwindowimpl.h"
extern MainWindowImpl *pMainWindowImpl;//temp here
QAutplayer::QAutplayer(QWidget *parent): QWidget(parent), ui(new Ui::AutPlayer)
{
  ui->setupUi(this);
  #ifdef QT_ROTATE_APP
    QGraphicsScene *scene = new QGraphicsScene();
    QGraphicsView *view = new QGraphicsView(parent);
    view->setGeometry(0, 0, 800, 480); // actual Display size
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    QGraphicsProxyWidget *proxy = scene->addWidget(this);
    view->setScene(scene);
    view->show();
    view->rotate(180);
  #endif
  setProperty("noinput", true);
  #if defined(USE_AUTPLAYER)
    ap = new AUTPlayer();
  #endif
  setStyleSheet(QStringLiteral("background-color: rgb(112, 200, 11);"));
  astatus = ASTATUS_STOPPED;
}

QAutplayer::~QAutplayer()
{
  delete ui;
  #if defined(USE_AUTPLAYER)
    delete ap;
  #endif
  astatus = ASTATUS_STOPPED;
  setStyleSheet(QStringLiteral("background-color: rgb(112, 200, 11);"));
}


void QAutplayer::on_pushButton_openfile_clicked()
{
  int ret;
  if ((astatus == ASTATUS_PAUSED) || (astatus == ASTATUS_PLAYING))
  {
    #if defined(USE_AUTPLAYER)
      ap->stop();
    #endif
    astatus = ASTATUS_STOPPED;
    //setStyleSheet(QStringLiteral("background-color: rgb(112, 200, 11);"));
  }
  setStyleSheet(QStringLiteral("background-color: rgb(112, 200, 11);"));
  QString fileName;
    // = QFileDialog::getOpenFileName(this,				//获得要打开的文件的信息
  //               tr("Open video"), ".",
  //              tr("video files (*.mp4)"));

  QFileDialog *fileDialog = new QFileDialog(this);
  fileDialog->setProperty("noinput", true);
  fileDialog->setWindowTitle(tr("Open Video"));
  fileDialog->setDirectory("/mnt/UDISK/av");
  fileDialog->setNameFilter(tr("media files (*.mp4 *.3gp *.mkv *.mp3 *.wav *.mid *.amr)"));
  if (fileDialog->exec() == QDialog::Accepted)
  {
    fileName = fileDialog->selectedFiles()[0];
    // QMessageBox::information(NULL, tr("Path"), tr("You selected ") + path);
  }
  else
  {
    // QMessageBox::information(NULL, tr("Path"), tr("You didn't select any files."));
  }


  if (!fileName.isEmpty())
  //如果文件名不为空的话
  {
    #if defined(USE_AUTPLAYER)
      ret = ap->setUrl((char*)fileName.toStdString().c_str());
      if (ret < 0)
        return ;
      ap->setUserCb(end, this);
      //ap->setViewWindow(0,0,ap->getDispwidth(),ap->getDispheigth()-60);
      ap->setViewWindow(0, 0, 800, 480-60);
      //sleep(1);
      ap->play();

    #endif
    astatus = ASTATUS_PLAYING;
    setStyleSheet(QStringLiteral("background-color: rgba(112, 200, 11, 0);"));

  }
  else
  {


  }
}

void QAutplayer::on_pushButton_play_clicked()
{
  #if defined(USE_AUTPLAYER)
    ap->pause();
  #endif
  if (astatus == ASTATUS_PAUSED)
  {
    astatus = ASTATUS_PLAYING;
  }
  else
  {
    astatus = ASTATUS_PAUSED;

  }

}

#if defined(USE_AUTPLAYER)
  int QAutplayer::end(int32_t msgType, void *user) //结束MPlayer
  {
    QAutplayer *p = (QAutplayer*)user;


    //if (msgType == NOTIFY_PLAYBACK_COMPLETE)
    {
      printf("call end \r\n");
      //p->setStyleSheet(QStringLiteral("background-color: rgb(112, 200, 11);"));

    }
  }
#endif
void QAutplayer::on_pushButton_stop_clicked()
{
  #if defined(USE_AUTPLAYER)
    ap->stop();
  #endif
  astatus = ASTATUS_STOPPED;
  setStyleSheet(QStringLiteral("background-color: rgb(112, 200, 11);"));
}

void QAutplayer::on_pushButton_home_clicked()
{
  #if defined(USE_AUTPLAYER)
    ap->stop();
  #endif
  astatus = ASTATUS_STOPPED;
  setStyleSheet(QStringLiteral("background-color: rgb(112, 200, 11);"));
   pMainWindowImpl->setHidden(false);//temp here
  this->close();
}
