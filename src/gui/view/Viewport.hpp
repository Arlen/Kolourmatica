/***********************************************************************
|*  Copyright (C) 2010, 2011 Arlen Avakian
|*
|*  This file is part of Kolourmatica.
|*
|*  Kolourmatica is free software: you can redistribute it and/or modify
|*  it under the terms of the GNU General Public License as published by
|*  the Free Software Foundation, either version 3 of the License, or
|*  (at your option) any later version.
|*
|*  Kolourmatica is distributed in the hope that it will be useful,
|*  but WITHOUT ANY WARRANTY; without even the implied warranty of
|*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
|*  GNU General Public License for more details.
|*
|*  You should have received a copy of the GNU General Public License
|*  along with Kolourmatica.  If not, see <http://www.gnu.org/licenses/>.
|*
|************************************************************************/


#ifndef VIEWPORT_HPP
#define VIEWPORT_HPP

#include "../../core/ReferenceWhite.hpp"

#include "../../../../eigen/Eigen/Dense"

#include <QtGui/QGraphicsWidget>

#include <boost/thread.hpp>

using boost::thread;
using namespace Eigen;

struct ViewportConfig;

class QImage;

typedef double Real;


template <typename Real>
struct Camera{

  Real left_;
  Real right_;
  Real top_;
  Real bottom_;
  Real front_;
  Real back_;
  Real zoom_;
  Real rayDir_;
  Real rayDelta_;
  unsigned horIndex_;
  unsigned verIndex_;
  unsigned rayIndex_;
};


class Viewport : public QGraphicsWidget{

public:
  Viewport();
  Viewport(qreal w, qreal h);
  void configure(const ViewportConfig& config);
  void setWCS(int wcs);
  void setSCS(int scs);
  void setRW(int rw);
  void setAM(int am);
  void setAccuracy(double acc);
  void setCamera(int c);
  void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
  bool abortRendering() const;
  int threadCount() const;

protected:
  void mousePressEvent(QGraphicsSceneMouseEvent* event);
  void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
  void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);

  void hoverEnterEvent(QGraphicsSceneHoverEvent* event);
  void hoverLeaveEvent(QGraphicsSceneHoverEvent* event);
  void hoverMoveEvent (QGraphicsSceneHoverEvent* event);

private:
  typedef GlobalReferenceWhite<Real> GRW;

  friend class RenderThread;
  template <class From, class To> friend struct Renderer;

  void initViewport(qreal w, qreal h);
  void initCamera();
  void paintResizeHandle(qreal len, qreal gap, qreal width, QPainter* painter);
  void restartRendering();
  void setAbortRendering(bool abort);

  qreal resizeHandleWidth_;
  qreal resizeHandleLength_;
  qreal resizeHandleGap_;
  unsigned resizeFlags_;
  unsigned imageWidth_;
  unsigned imageHeight_;
  bool ready_;
  bool showResizeHandle_;
  bool showImage_;

  enum {Left = 0x1, Top = 0x2, Right = 0x4, Bottom = 0x8};

  int wcs_;
  int scs_;
  ReferenceWhite<Real> rw_;
  Matrix<Real, 3, 3> am_;
  Camera<Real> camera_;

  int threadCount_;
  unsigned char* buffer_;
  QImage image_;
  boost::thread* thread_;

  bool abortRendering_;
  boost::mutex abortRenderingMutex_;
};


struct ViewportConfig{

  int workingColourSpace_;
  int systemColourSpace_;
  int referenceWhite_;
  int adaptationMethod_;
  double accuracy_;
  int camera_;
};


template <class From, class To>
struct Renderer{

  Renderer(Viewport* pViewport,
	   From* from,
	   To* to,
	   Camera<Real>* camera,
	   unsigned width,
	   unsigned height,
	   unsigned segment) :
    pViewport_(pViewport),
    from_(from),
    to_(to),
    camera_(camera),
    width_(width),
    height_(height),
    segment_(segment){ }

  ~Renderer(){
    if(thread_){
      join();
      delete thread_;
    }
  }

  void operator()(){

    unsigned char* const buffer = pViewport_->buffer_;
    Vector3d out;
    Vector3d in;
    double delta = camera_->rayDelta_ * camera_->rayDir_;
    unsigned si = segment_ * width_;

    double segmentF = (double)segment_;
    double vDist = camera_->top_ - camera_->bottom_;
    double hDist = camera_->right_ - camera_->left_;

    in(camera_->verIndex_) = ((segmentF * vDist) / height_) + camera_->bottom_;

    for(unsigned j = 0; j < width_; ++j){
      in(camera_->horIndex_) = ((static_cast<double>(j) * hDist) / width_) + 
	camera_->left_;
      in(camera_->rayIndex_) = camera_->front_;

      do{
	out = to_->operator()(from_->operator()(in)).position();
	if( (out.array() < 1).all() )
	  break;

	in(camera_->rayIndex_) += delta;
      }while(in(camera_->rayIndex_) > camera_->back_);

      if( (out.array() < 0).any() )
	out << 0, 0, 0;

      buffer[4 * (si + j)    ] = static_cast<unsigned char>(255.0 * out(2));
      buffer[4 * (si + j) + 1] = static_cast<unsigned char>(255.0 * out(1));
      buffer[4 * (si + j) + 2] = static_cast<unsigned char>(255.0 * out(0));
    }
  }

  void start(){
    thread_ = new boost::thread(boost::ref(*this));
  }

  void join() const{
    thread_->join();
  }

  Viewport* pViewport_;
  From* from_;
  To* to_;
  Camera<Real>* camera_;
  double width_;
  double height_;
  unsigned segment_;
  boost::thread* thread_;
};


template <class From, class To>
struct ThreadCreator{

  ThreadCreator(Viewport* pViewport,
		From from,
		To to,
		Camera<Real> camera,
		unsigned imageWidth,
		unsigned imageHeight) :
    pViewport_(pViewport),
    from_(from),
    to_(to),
    camera_(camera),
    imageWidth_(imageWidth),
    imageHeight_(imageHeight){
    threadCount_ = pViewport_->threadCount();
  }

  void operator()(){

    if(imageHeight_ < threadCount_){ threadCount_ = imageHeight_; }

    for(unsigned block = 0; block < imageHeight_; block += threadCount_){

      if(pViewport_->abortRendering()){ return; }

      unsigned blockSize =
	(block + threadCount_) > imageHeight_ ?
	imageHeight_ % block :
	threadCount_;

      Renderer<From, To>** jobs;
      jobs = new Renderer<From, To>* [blockSize];

      for(unsigned work = 0; work < blockSize; ++work){
	jobs[work] = new Renderer<From, To>
	  (pViewport_, &from_, &to_, &camera_,
	   (double)imageWidth_, (double)imageHeight_, work + block);
	jobs[work]->start();
      }

      for(unsigned work = 0; work < blockSize; ++work){
	jobs[work]->join();
	delete jobs[work];
      }
      delete[] jobs;
      pViewport_->update();
    }
  }

  Viewport* pViewport_;
  From from_;
  To to_;
  Camera<Real> camera_;
  unsigned imageWidth_;
  unsigned imageHeight_;
  unsigned threadCount_;
};


#endif
