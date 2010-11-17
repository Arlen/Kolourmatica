#include "Viewer.hpp"

#include <QtGui/QPainter>
#include <QtGui/QImage>
#include <QtGui/QMouseEvent>
#include <QtCore/QTime>
#include <QtGui/QApplication>

using namespace Eigen;


RenderThread::RenderThread(Viewer* pWidget, int id) : 
  id_(id), pWidget_(pWidget){

  setTerminationEnabled(true);
}						      

void RenderThread::run(){

  int w = pWidget_->width() / pWidget_->proxy_;
  int h = pWidget_->height() / pWidget_->proxy_;
  RenderSpace(w, h);
}

void RenderThread::Init(){

  if( isRunning() ){ quit(); }
  in_ = pWidget_->in_;
  if( pWidget_->convert_ ){  ////
    convert_ = pWidget_->convert_;
  }
}

void RenderThread::RenderSpace(int w, int h){

  unsigned char* const buffer = pWidget_->buffer_;
  const int threadCount = pWidget_->threadCount_;
  Vector3f boxMin(Vector3f::Zero());
  Vector3f boxMax(Vector3f::Ones());
  Vector3f out;
  float x, y, Y;
  float delta = 0.01;

  for(int i = id_; i < h; i += threadCount){
    int si = i * w;
    y = static_cast<float>(i) / static_cast<float>(h);
    Y = 1.0;

    for(int j = 0; j < w; ++j){
      x = static_cast<float>(j) / static_cast<float>(w);
      Y = 1.0;

      do{
	in_.from_ << x, y, Y;
	out = convert_(in_);

	if( (boxMax.cwise() > out).all() ){
	  break;
	}



      	Y -= delta;
      	in_.from_ << x, y, Y;

      }while(Y > 0.0);
      if( (boxMin.cwise() > out).any() ){ out << 0, 0, 0; }


      buffer[4 * (si + j)    ] = static_cast<unsigned char>(255.0 * out(2, 0));
      buffer[4 * (si + j) + 1] = static_cast<unsigned char>(255.0 * out(1, 0));
      buffer[4 * (si + j) + 2] = static_cast<unsigned char>(255.0 * out(0, 0));
    }
  }
}

Viewer::Viewer() : QWidget(), buffer_(0), size_(0), proxy_(16){

  convert_ = NULL;
  wcs_ = CSxyY;
  scs_ = CSsRGB;
  threadCount_ = QThread::idealThreadCount();
  threads_ = new RenderThread* [threadCount_];
  for(int i = 0; i < threadCount_; ++i){
    threads_[i] = new RenderThread(this, i);
  }
  in_.white_ = Convert::IlluminantD65_;
  SRGB srgb;
  in_.targetM_1_ = srgb.GetM_1_Adapted(in_.white_, Convert::Bradford_);
  in_.targetGamma_ = 2.4f;
}

Viewer::~Viewer(){

  if(buffer_){
    delete[] buffer_;
  }

  for(int i = 0; i < threadCount_; ++i){
    delete threads_[i];
  }
  delete[] threads_;
}

void Viewer::SetWorkingColorSpace(int cs){

  StopAllThreads();
  wcs_ = cs;

  /*
    C is the conversion function index.
    F is the From index.
    T is the To index.
  */
  enum{ C = 0, F = 1, T = 2 };

  Conversions con;
  convert_ = con.GetTable()[wcs_][scs_].get<C>();
  if(wcs_ > CSLCHuv){
    in_.sourceM_ = con.GetTable()[wcs_][scs_].
      get<F>()->GetM_Adapted(rw_, am_);
    in_.sourceGamma_ = con.GetTable()[wcs_][scs_].
      get<F>()->GetGamma();
  }
  UpdateAllThreads();
  proxy_ = 16;
  update();
}

void Viewer::SetSystemColorSpace(int cs){

  StopAllThreads();
  scs_ = cs;

  /*
    C is the conversion function index.
    F is the From index.
    T is the To index.
  */
  enum{ C = 0, F = 1, T = 2 };

  Conversions con;
  convert_ = con.GetTable()[wcs_][scs_].get<C>();
  if(scs_ > CSLCHuv){
    in_.targetM_1_ = con.GetTable()[wcs_][scs_].
      get<T>()->GetM_1_Adapted(rw_, am_);
    in_.targetGamma_ = con.GetTable()[wcs_][scs_].
      get<T>()->GetGamma();
  }
  UpdateAllThreads();
  proxy_ = 16;
  update();
}

void Viewer::SetReferenceWhite(const Vector3f& rRw){

  rw_ = rRw;
  StopAllThreads();
  UpdateAllThreads();
}

void Viewer::SetAdaptationMethod(const Matrix3f& rAd){

  am_ = rAd;
  StopAllThreads();
  UpdateAllThreads();
}

void Viewer::resizeEvent(QResizeEvent* pEvent){

  if(size_ < width() * height()){

    proxy_ = 16;
    for(int i = 0; i < threadCount_; ++i){
      threads_[i]->quit();
    }
    size_ = width() * height();
    if(buffer_){ delete[] buffer_; }
    buffer_ = new unsigned char[4 * size_];
  }
  proxy_ = 16;
  for(int i = 0; i < threadCount_; ++i){
    threads_[i]->quit();
  }
}

void Viewer::paintEvent(QPaintEvent* pEvent){

  QTime time;
  time.start();

  if(convert_){
  for(int i = 0; i < threadCount_; ++i){
    threads_[i]->start(QThread::LowestPriority);
  }
 
  for(int i = 0; i < threadCount_; ++i){
    threads_[i]->wait();
  }
  }
  QPainter painter(this);

  QImage image(buffer_, width() / proxy_, height() / proxy_, QImage::Format_RGB32);
  if(image.isNull()){
    image = QImage(1, 1, QImage::Format_RGB32);
  }
  painter.drawImage(QPoint(0, 0), image.scaled(width(), height()).mirrored());
  painter.setPen(Qt::white);
  painter.drawText(10, 10, QString::number(proxy_) + QString("/16   ") +
		   QString::number(time.elapsed()));
  if(proxy_ > 1){
    proxy_ /= 2;
    update();
  }
}

void Viewer::mousePressEvent(QMouseEvent* pEvent){

  if(pEvent->buttons() & Qt::LeftButton){
    proxy_ = 16;
    for(int i = 0; i < threadCount_; ++i){
      threads_[i]->quit();
    }
    update();
  }
}

void Viewer::StopAllThreads(){

  for(int i = 0; i < threadCount_; ++i){
    threads_[i]->quit();
  }
}

void Viewer::UpdateAllThreads(){

  for(int i = 0; i < threadCount_; ++i){
    threads_[i]->Init();
  }
}
