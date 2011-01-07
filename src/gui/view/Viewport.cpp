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


#include "Viewport.hpp"

#include <QtGui/QGraphicsSceneResizeEvent>
#include <QtGui/QPainter>
#include <QtGui/QStyleOptionGraphicsItem>
#include <QtGui/QGraphicsLineItem>
#include <QtGui/QGraphicsScene>
#include <QtGui/QGraphicsSceneMouseEvent>
#include <QtCore/QRectF>
#include <QtGui/QImage>

#include <boost/fusion/container/vector.hpp>
#include <boost/fusion/sequence/intrinsic/at_c.hpp>

#include <unistd.h>

#include <iostream>
using namespace std;


Viewport::Viewport(){

  ready_ = false;
  abortRendering_ = false;
  showResizeHandle_ = false;
  showImage_ = true;
  setMinimumSize(128, 128);
  initViewport(512.0, 512.0);
  initCamera();
}

Viewport::Viewport(qreal w, qreal h){

  ready_ = false;
  abortRendering_ = false;
  showResizeHandle_ = false;
  showImage_ = true;
  setMinimumSize(128, 128);
  initViewport(w, h);
  initCamera();
}

void Viewport::configure(const ViewportConfig& config){

  wcs_ = config.workingColourSpace_;
  scs_ = config.systemColourSpace_;
  setRW(config.referenceWhite_);
  setAM(config.adaptationMethod_);
  // = config.precision_;
  // = config.accuracy_;
  // = config.camera_;
  ready_ = true;
  restartRendering();
}

void Viewport::setWCS(int wcs){

  setAbortRendering(true);
  if(thread_){ thread_->join(); }
  wcs_ = wcs;
  setAbortRendering(false);
  if(ready_){ restartRendering(); }
}

void Viewport::setSCS(int scs){

  setAbortRendering(true);
  if(thread_){ thread_->join(); }
  scs_ = scs;
  setAbortRendering(false);
  if(ready_){ restartRendering(); }
}

void Viewport::setRW(int rw){

  setAbortRendering(true);
  if(thread_){ thread_->join(); }

  switch(rw){
  case 0:  rw_.setIlluminant(IlluminantA<Real>());    break;
  case 1:  rw_.setIlluminant(IlluminantB<Real>());    break;
  case 2:  rw_.setIlluminant(IlluminantC<Real>());    break;
  case 3:  rw_.setIlluminant(IlluminantD50<Real>());  break;
  case 4:  rw_.setIlluminant(IlluminantD55<Real>());  break;
  case 5:  rw_.setIlluminant(IlluminantD65<Real>());  break;
  case 6:  rw_.setIlluminant(IlluminantD75<Real>());  break;
  case 7:  rw_.setIlluminant(IlluminantE<Real>());    break;
  case 8:  rw_.setIlluminant(IlluminantF2<Real>());   break;
  case 9:  rw_.setIlluminant(IlluminantF7<Real>());   break;
  case 10: rw_.setIlluminant(IlluminantF11<Real>());  break;
  default:
    return;
  }
  GRW::value_ = rw_;
  setAbortRendering(false);
  if(ready_){ restartRendering(); }
}

void Viewport::setAM(int am){

  setAbortRendering(true);
  if(thread_){ thread_->join(); }

  switch(am){
  case 0: am_ = AdaptationMethod<Real>::XYZScaling_; break;
  case 1: am_ = AdaptationMethod<Real>::VonKries_;   break;
  case 2: am_ = AdaptationMethod<Real>::Bradford_;   break;
  default:
    return;
  }
  setAbortRendering(false);
  if(ready_){ restartRendering(); }
}

void Viewport::setPrecision(int prec){

}

void Viewport::setAccuracy(double acc){

}

void Viewport::setCamera(int camera){

}

void Viewport::paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
		   QWidget* widget){

  painter->fillRect(0, 0, size().width(), size().height(), Qt::black);

  if( showResizeHandle_ )
    paintResizeHandle(resizeHandleLength_, resizeHandleGap_,
		      resizeHandleWidth_, painter);
  if( showImage_ ){
    QImage image(buffer_, imageWidth_, imageHeight_, QImage::Format_RGB32);
    painter->drawImage(QPoint(resizeHandleWidth_, resizeHandleWidth_),
		       image.mirrored());
  }
}

bool Viewport::abortRendering() const{ return abortRendering_; }

int Viewport::threadCount() const{ return threadCount_; }

void Viewport::mousePressEvent(QGraphicsSceneMouseEvent* event){

  if(event->button() & Qt::LeftButton){

    resizeFlags_ = 0;
    qreal s = resizeHandleWidth_ * 3.0;
    if(!rect().adjusted(s, s, -s, -s).contains(event->pos())){

      showImage_ = false;
      qreal ext = resizeHandleGap_ + resizeHandleLength_;
      qreal hext = (rect().width() - (ext * 2.0)) + resizeHandleGap_;
      qreal vext = (rect().height() - (ext * 2.0)) + resizeHandleGap_;
      QRectF r11 = rect().adjusted(0, 0, -(hext + ext), -(vext + ext));
      QRectF r12 = rect().adjusted(ext, 0, -ext, -(vext + ext));
      QRectF r13 = rect().adjusted(hext + ext, 0, 0, -(vext + ext));
      QRectF r21 = rect().adjusted(0, ext, -(hext + ext), -ext);
      QRectF r23 = rect().adjusted(hext + ext, ext, 0, -ext);
      QRectF r31 = rect().adjusted(0, ext + vext, -(ext + hext), 0);
      QRectF r32 = rect().adjusted(ext, ext + vext, -ext, 0);
      QRectF r33 = rect().adjusted(ext + hext, ext + vext, 0, 0);

      if(r11.contains(event->pos()))
	resizeFlags_ = Top | Left;
      else if(r12.contains(event->pos()))
	resizeFlags_ = Top;
      else if(r13.contains(event->pos()))
	resizeFlags_ = Top | Right;
      else if(r21.contains(event->pos()))
	resizeFlags_ = Left;
      else if(r23.contains(event->pos()))
	resizeFlags_ = Right;
      else if(r31.contains(event->pos()))
	resizeFlags_ = Bottom | Left;
      else if(r32.contains(event->pos()))
	resizeFlags_ = Bottom;
      else if(r33.contains(event->pos()))
	resizeFlags_ = Bottom | Right;
      else
	return;

      setAbortRendering(true);
      if(thread_)
	thread_->join();
    }
  }
  QGraphicsWidget::mousePressEvent(event);
}

void Viewport::mouseMoveEvent(QGraphicsSceneMouseEvent* event){

  if(event->buttons() & Qt::LeftButton){
    if(resizeFlags_){
      setFlag(QGraphicsItem::ItemIsMovable, false);
      QRectF rec = rect();
      QPointF p = event->pos();
      QPointF d = event->pos() - event->lastPos();
      qreal max = qMax(abs(d.x()), abs(d.y()));
      max *= (d.x() < 0 || d.y() < 0) ? -1.0 : 1.0;

      QPointF nonUniform = event->pos();
      QPointF uniform = QPointF(max, max);

      if(resizeFlags_ == (Top    | Left ))
	rec.setTopLeft(rec.topLeft() + uniform);
      else if(resizeFlags_ == (Top           )) rec.setTop(p.y());
      else if(resizeFlags_ == (Top    | Right)) rec.setTopRight(nonUniform);
      else if(resizeFlags_ == (         Left )) rec.setLeft(p.x());
      else if(resizeFlags_ == (         Right)) rec.setRight(p.x());
      else if(resizeFlags_ == (Bottom | Left )) rec.setBottomLeft(nonUniform);
      else if(resizeFlags_ == (Bottom        )) rec.setBottom(p.y());
      else if(resizeFlags_ == (Bottom | Right))
	rec.setBottomRight(rec.bottomRight() + uniform);

      setGeometry(mapToScene(rec).boundingRect());
    }
  }
  QGraphicsWidget::mouseMoveEvent(event);
  if( !(flags() & QGraphicsItem::ItemIsMovable) )
    setFlag(QGraphicsItem::ItemIsMovable, true);
}

void Viewport::mouseReleaseEvent(QGraphicsSceneMouseEvent* event){

  if(event->button() == Qt::LeftButton && resizeFlags_){

    QRectF rec = rect();
    Real offset = resizeHandleWidth_ * 2.0;
    Real newWidthF = rec.width() - offset;
    Real newHeightF = rec.height() - offset;
    Real oldWidthF = (Real)imageWidth_;
    Real oldHeightF = (Real)imageHeight_;

    Real newTop = (camera_.top_ * newHeightF) / oldHeightF;
    Real newBottom = (camera_.bottom_ * newHeightF) / oldHeightF;
    Real newRight = (camera_.right_ * newWidthF) / oldWidthF;
    Real newLeft  = (camera_.left_ * newWidthF) / oldWidthF;
    Real newHDist = newRight - newLeft;
    Real oldHDist = camera_.right_ - camera_.left_;
    Real hOffset = newHDist - oldHDist;
    Real newVDist = newTop - newBottom;
    Real oldVDist = camera_.top_ - camera_.bottom_;
    Real vOffset = newVDist - oldVDist;


    if(resizeFlags_ == (Top    | Left )){
      camera_.top_ = camera_.top_ + vOffset;
      camera_.left_ = camera_.left_ - hOffset;

    }else if(resizeFlags_ == (Top           )){
      camera_.top_ = camera_.top_ + vOffset;

    }else if(resizeFlags_ == (Top    | Right)){
      camera_.top_ = camera_.top_ + vOffset;
      camera_.right_ = camera_.right_ + hOffset;

    }else if(resizeFlags_ == (         Left )){
      camera_.left_ = camera_.left_ - hOffset;

    }else if(resizeFlags_ == (         Right)){
      camera_.right_ = camera_.right_ + hOffset;

    }else if(resizeFlags_ == (Bottom | Left )){
      camera_.bottom_ = camera_.bottom_ - vOffset;
      camera_.left_ = camera_.left_ - hOffset;

    }else if(resizeFlags_ == (Bottom        )){
      camera_.bottom_ = camera_.bottom_ - vOffset;
 
    }else if(resizeFlags_ == (Bottom | Right)){
      camera_.bottom_ = camera_.bottom_ - vOffset;
      camera_.right_ = camera_.right_ + hOffset;
    }

    imageWidth_ = (unsigned)newWidthF;
    imageHeight_ = (unsigned)newHeightF;

    delete[] buffer_;
    buffer_ = new unsigned char[4 * imageWidth_ * imageHeight_];
    setAbortRendering(false);
    if(ready_){ restartRendering(); }
    showImage_ = true;
  }
  QGraphicsWidget::mouseReleaseEvent(event);
}

void Viewport::hoverEnterEvent(QGraphicsSceneHoverEvent* event){

  QGraphicsWidget::hoverEnterEvent(event);
}

void Viewport::hoverLeaveEvent(QGraphicsSceneHoverEvent* event){

  if( showResizeHandle_ ){
    showResizeHandle_ = false;
    update();
  }
}

void Viewport::hoverMoveEvent(QGraphicsSceneHoverEvent* event){

  qreal s = resizeHandleWidth_ * 3.0;
  if(!rect().adjusted(s, s, -s, -s).contains(event->pos())){
    if( !showResizeHandle_ )
      showResizeHandle_ = true;
  }else{
    if( showResizeHandle_ )
      showResizeHandle_ = false;
  }
  update();
  QGraphicsWidget::hoverMoveEvent(event);
}


/* private */
void Viewport::initViewport(qreal w, qreal h){

  resizeHandleWidth_ = 2.0;
  resizeHandleLength_ = 32.0;
  resizeHandleGap_ = 8.0;
  resizeFlags_ = 0;
  showResizeHandle_ = false;
  setFlag(QGraphicsItem::ItemIsMovable, true);
  setFlag(QGraphicsItem::ItemIsSelectable, true);
  setAcceptHoverEvents(true);

  qreal offset = resizeHandleWidth_ * 2.0;
  resize(w + offset, h + offset);

  imageWidth_ = (unsigned)qRound(w);
  imageHeight_ = (unsigned)qRound(h);
  buffer_ = new unsigned char[4 * imageWidth_ * imageHeight_];

  thread_ = 0;

#if defined(Q_OS_MAC)
  threadCount_ = (unsigned)MPProcessorsScheduled();
#else
  threadCount_ = (unsigned)sysconf(_SC_NPROCESSORS_ONLN);
#endif
}

void Viewport::initCamera(){

  camera_.left_   = 0.0;
  camera_.right_  = 1.0;
  camera_.top_    = 1.0;
  camera_.bottom_ = 0.0;
  camera_.front_  = 1.0;
  camera_.back_   = 0.0;
}


void Viewport::paintResizeHandle(qreal len, qreal gap, qreal width,
				 QPainter* painter){

  painter->save();
  painter->setRenderHint(QPainter::Antialiasing);

  QRectF rec = rect().adjusted(width / 2.0, width / 2.0,
			       -width / 2.0, -width / 2.0);

  QPen pen(QBrush(QColor(0, 200, 0, 255), Qt::SolidPattern), width,
	   Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);

  QPointF tl = rec.topLeft();
  QPointF tr = rec.topRight();
  QPointF bl = rec.bottomLeft();
  QPointF br = rec.bottomRight();

  qreal hp1 = tl.x() + len + gap;
  qreal hp2 = tr.x() - len;
  qreal vp1 = tl.y() + len + gap;
  qreal vp2 = bl.y() - len;

  painter->setPen(pen);
  /* top left corner */
  /* 1. top left end */
  painter->drawLine(tl.x(), tl.y(), tl.x() + len, tl.y());
  /* 2. left top end */
  painter->drawLine(tl.x(), tl.y(), tl.x(), tl.y() + len);

  /* bottom right corner */
  /* 1. bottom right end */
  painter->drawLine(hp2, br.y(), br.x(), br.y());
  /* 2. right bottom end */
  painter->drawLine(tr.x(), vp2, tr.x(), br.y());

  pen.setBrush(QBrush(QColor(0, 0, 200, 255), Qt::SolidPattern));
  painter->setPen(pen);
  /* bottom left corner*/
  /* 1. bottom left end*/
  painter->drawLine(bl.x(), bl.y(), bl.x() + len, bl.y());
  /* 2. left bottom end */
  painter->drawLine(tl.x(), vp2, tl.x(), bl.y());

  /* top right corner */
  /* 1. top right end */
  painter->drawLine(hp2, tr.y(), tr.x(), tr.y());
  /* 2. right top end */
  painter->drawLine(tr.x(), tr.y(), tr.x(), tr.y() + len);

  pen.setBrush(QBrush(QColor(200, 0, 0, 255), Qt::SolidPattern));
  painter->setPen(pen);
  /* top middle */
  painter->drawLine(hp1, tl.y(), hp2 - gap, tr.y());
  /* bottom middle */
  painter->drawLine(hp1, bl.y(), hp2 - gap, br.y());
  /* left middle */
  painter->drawLine(tl.x(), vp1, tl.x(), vp2 - gap);
  /* right middle */
  painter->drawLine(tr.x(), vp1, tr.x(), vp2 - gap);

  painter->restore();
}


void Viewport::restartRendering(){

  typedef Space_XYZ<Real> XYZ;
  typedef Space_xyY<Real> xyY;
  typedef Space_sRGB<Real> sRGB;
  XYZ xyz;
  xyY xyy;
  sRGB srgb;
  srgb.adapt(rw_, am_);

  if( thread_ ){ thread_->join(); delete thread_; }
  if( buffer_ ){
    memset(buffer_, 0, 4 * imageWidth_ * imageHeight_);
  }
  update();

  switch(wcs_){


  case 0:{
    switch(scs_){
    case 20:{
      thread_ = new boost::thread(ThreadCreator<XYZ, sRGB>
				  (this, xyz, srgb, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    }
    break;
  }

  case 1:{
    switch(scs_){
    case 20:{
      thread_ = new boost::thread(ThreadCreator<xyY, sRGB>
				  (this, xyy, srgb, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    }
    break;
  }


  }
}


void Viewport::setAbortRendering(bool abort){

  abortRenderingMutex_.lock();
  abortRendering_ = abort;
  abortRenderingMutex_.unlock();
}












