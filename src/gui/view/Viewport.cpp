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

#include "../../core/Utility.hpp"
#include "../../core/AdaptationMethod.hpp"
#include "../../core/ReferenceWhite.hpp"
#include "../../core/Space_XYZ.hpp"
#include "../../core/Space_xyY.hpp"
#include "../../core/Space_Lab.hpp"
#include "../../core/Space_LCHab.hpp"
#include "../../core/Space_Luv.hpp"
#include "../../core/Space_LCHuv.hpp"
#include "../../core/Space_AdobeRGB.hpp"
#include "../../core/Space_AppleRGB.hpp"
#include "../../core/Space_BestRGB.hpp"
#include "../../core/Space_BetaRGB.hpp"
#include "../../core/Space_BruceRGB.hpp"
#include "../../core/Space_CIERGB.hpp"
#include "../../core/Space_ColorMatchRGB.hpp"
#include "../../core/Space_DonRGB4.hpp"
#include "../../core/Space_ECIRGB.hpp"
#include "../../core/Space_EktaSpacePS5.hpp"
#include "../../core/Space_NTSCRGB.hpp"
#include "../../core/Space_PAL_SECAMRGB.hpp"
#include "../../core/Space_ProPhotoRGB.hpp"
#include "../../core/Space_SMPTE_CRGB.hpp"
#include "../../core/Space_sRGB.hpp"
#include "../../core/Space_WideGamutRGB.hpp"

#include <QtGui/QGraphicsSceneResizeEvent>
#include <QtGui/QPainter>
#include <QtGui/QStyleOptionGraphicsItem>
#include <QtGui/QGraphicsLineItem>
#include <QtGui/QGraphicsScene>
#include <QtGui/QGraphicsSceneMouseEvent>
#include <QtCore/QRectF>
#include <QtGui/QImage>

#include <unistd.h>


Viewport::Viewport(){

  ready_ = false;
  abortRendering_ = false;
  showResizeHandle_ = false;
  showImage_ = true;
  buffer_ = 0;
  setMinimumSize(128, 128);
  initViewport(512.0, 512.0);
  initCamera();
}

Viewport::Viewport(qreal w, qreal h){

  ready_ = false;
  abortRendering_ = false;
  showResizeHandle_ = false;
  showImage_ = true;
  buffer_ = 0;
  setMinimumSize(128, 128);
  initViewport(w, h);
  initCamera();
}

void Viewport::configure(const ViewportConfig& config){

  wcs_ = config.workingColourSpace_;
  scs_ = config.systemColourSpace_;
  setRW(config.referenceWhite_);
  setAM(config.adaptationMethod_);
  // = config.accuracy_;
  // = config.camera_;
  ready_ = true;
  restartRendering();
}

void Viewport::setWCS(int wcs){

  setAbortRendering(true);
  if(thread_){ thread_->join(); }
  wcs_ = wcs;
  initCamera();
  initViewport(512.0, 512.0);

  switch(wcs_){
  case CSXYZ:{

    camera_.rayDelta_ = 0.01;
    camera_.horIndex_ = 0;
    camera_.verIndex_ = 2;
    camera_.rayIndex_ = 1;
    break;
  }
  case CSxyY:{
    camera_.rayDelta_ = 0.01;
    camera_.horIndex_ = 0;
    camera_.verIndex_ = 1;
    camera_.rayIndex_ = 2;
    break;
  }
  case CSLab:
  case CSLCHab:
  case CSLuv:
  case CSLCHuv:{
    camera_.left_ = -200.0;
    camera_.right_ = 200.0;
    camera_.bottom_ = -200.0;
    camera_.top_ = 200.0;
    camera_.front_ = 100.0;
    camera_.back_ = 0.0;
    camera_.rayDelta_ = 1.0;
    camera_.horIndex_ = 1;
    camera_.verIndex_ = 2;
    camera_.rayIndex_ = 0;
    break;
  }
  }
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

/* protected */
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
  if(buffer_)
    delete[] buffer_;

  buffer_ = new unsigned char[4 * imageWidth_ * imageHeight_];

  thread_ = 0;

#if defined(Q_OS_MAC)
  threadCount_ = (unsigned)MPProcessorsScheduled();
#else
  threadCount_ = (unsigned)sysconf(_SC_NPROCESSORS_ONLN);
#endif
}

void Viewport::initCamera(){

  camera_.left_     = 0.0;
  camera_.right_    = 1.0;
  camera_.top_      = 1.0;
  camera_.bottom_   = 0.0;
  camera_.front_    = 1.0;
  camera_.back_     = 0.0;
  camera_.zoom_     = 1.0;
  camera_.rayDir_   = -1.0;
  camera_.rayDelta_ = 0.01;
  camera_.horIndex_ = 0;
  camera_.verIndex_ = 1;
  camera_.rayIndex_ = 2;
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
  typedef Space_Lab<Real> Lab;
  typedef Space_LCHab<Real> LCHab;
  typedef Space_Luv<Real> Luv;
  typedef Space_LCHuv<Real> LCHuv;
  typedef Space_AdobeRGB<Real> AdobeRGB;
  typedef Space_AppleRGB<Real> AppleRGB;
  typedef Space_BestRGB<Real> BestRGB;
  typedef Space_BetaRGB<Real> BetaRGB;
  typedef Space_BruceRGB<Real> BruceRGB;
  typedef Space_CIERGB<Real> CIERGB;
  typedef Space_ColorMatchRGB<Real> ColorMatchRGB;
  typedef Space_DonRGB4<Real> DonRGB4;
  typedef Space_ECIRGB<Real> ECIRGB;
  typedef Space_EktaSpacePS5<Real> EktaSpacePS5;
  typedef Space_NTSCRGB<Real> NTSCRGB;
  typedef Space_PAL_SECAMRGB<Real> PAL_SECAMRGB;
  typedef Space_ProPhotoRGB<Real> ProPhotoRGB;
  typedef Space_SMPTE_CRGB<Real> SMPTE_CRGB;
  typedef Space_sRGB<Real> sRGB;
  typedef Space_WideGamutRGB<Real> WideGamutRGB;

  XYZ xyz;
  xyY xyy;
  Lab lab;
  LCHab lchab;
  Luv luv;
  LCHuv lchuv;
  AdobeRGB adobe;
  AppleRGB apple;
  BestRGB best;
  BetaRGB beta;
  BruceRGB bruce;
  CIERGB cie;
  ColorMatchRGB colormatch;
  DonRGB4 don4;
  ECIRGB eci;
  EktaSpacePS5 ekta;
  NTSCRGB ntsc;
  PAL_SECAMRGB pal;
  ProPhotoRGB pro;
  SMPTE_CRGB smpt;
  sRGB srgb;
  WideGamutRGB widegamut;

  adobe.adapt(rw_, am_);
  apple.adapt(rw_, am_);
  best.adapt(rw_, am_);
  beta.adapt(rw_, am_);
  bruce.adapt(rw_, am_);
  cie.adapt(rw_, am_);
  colormatch.adapt(rw_, am_);
  don4.adapt(rw_, am_);
  eci.adapt(rw_, am_);
  ekta.adapt(rw_, am_);
  ntsc.adapt(rw_, am_);
  pal.adapt(rw_, am_);
  pro.adapt(rw_, am_);
  smpt.adapt(rw_, am_);
  srgb.adapt(rw_, am_);
  widegamut.adapt(rw_, am_);


  if( thread_ ){ thread_->join(); delete thread_; }
  if( buffer_ ){
    memset(buffer_, 0, 4 * imageWidth_ * imageHeight_);
  }

  switch(wcs_){

  case CSXYZ:{  // From XYZ to all other colour spaces. 
    switch(scs_){
    case CSXYZ:{
      thread_ = new boost::thread(ThreadCreator<XYZ, XYZ>
				  (this, xyz, xyz, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSxyY:{
      thread_ = new boost::thread(ThreadCreator<XYZ, xyY>
				  (this, xyz, xyy, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSLab:{
      thread_ = new boost::thread(ThreadCreator<XYZ, Lab>
				  (this, xyz, lab, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSLCHab:{
      thread_ = new boost::thread(ThreadCreator<XYZ, LCHab>
				  (this, xyz, lchab, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSLuv:{
      thread_ = new boost::thread(ThreadCreator<XYZ, Luv>
				  (this, xyz, luv, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSLCHuv:{
      thread_ = new boost::thread(ThreadCreator<XYZ, LCHuv>
				  (this, xyz, lchuv, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSAdobeRGB:{
      thread_ = new boost::thread(ThreadCreator<XYZ, AdobeRGB>
				  (this, xyz, adobe, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSAppleRGB:{
      thread_ = new boost::thread(ThreadCreator<XYZ, AppleRGB>
				  (this, xyz, apple, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSBestRGB:{
      thread_ = new boost::thread(ThreadCreator<XYZ, BestRGB>
				  (this, xyz, best, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSBetaRGB:{
      thread_ = new boost::thread(ThreadCreator<XYZ, BetaRGB>
				  (this, xyz, beta, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSBruceRGB:{
      thread_ = new boost::thread(ThreadCreator<XYZ, BruceRGB>
				  (this, xyz, bruce, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSCIERGB:{
      thread_ = new boost::thread(ThreadCreator<XYZ, CIERGB>
				  (this, xyz, cie, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSColorMatchRGB:{
      thread_ = new boost::thread(ThreadCreator<XYZ, ColorMatchRGB>
				  (this, xyz, colormatch, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSDonRGB4:{
      thread_ = new boost::thread(ThreadCreator<XYZ, DonRGB4>
				  (this, xyz, don4, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSECIRGB:{
      thread_ = new boost::thread(ThreadCreator<XYZ, ECIRGB>
				  (this, xyz, eci, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSEktaSpacePS5:{
      thread_ = new boost::thread(ThreadCreator<XYZ, EktaSpacePS5>
				  (this, xyz, ekta, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSNTSCRGB:{
      thread_ = new boost::thread(ThreadCreator<XYZ, NTSCRGB>
				  (this, xyz, ntsc, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSPALSECAMRGB:{
      thread_ = new boost::thread(ThreadCreator<XYZ, PAL_SECAMRGB>
				  (this, xyz, pal, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSProPhotoRGB:{
      thread_ = new boost::thread(ThreadCreator<XYZ, ProPhotoRGB>
				  (this, xyz, pro, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSSMPTECRGB:{
      thread_ = new boost::thread(ThreadCreator<XYZ, SMPTE_CRGB>
				  (this, xyz, smpt, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSsRGB:{
      thread_ = new boost::thread(ThreadCreator<XYZ, sRGB>
				  (this, xyz, srgb, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSWideGamutRGB:{
      thread_ = new boost::thread(ThreadCreator<XYZ, WideGamutRGB>
				  (this, xyz, widegamut, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    }
    break;
  }


  case CSxyY:{  // From xyY to all other colour spaces.
     switch(scs_){
    case CSXYZ:{
      thread_ = new boost::thread(ThreadCreator<xyY, XYZ>
				  (this, xyy, xyz, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSxyY:{
      thread_ = new boost::thread(ThreadCreator<xyY, xyY>
				  (this, xyy, xyy, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSLab:{
      thread_ = new boost::thread(ThreadCreator<xyY, Lab>
				  (this, xyy, lab, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSLCHab:{
      thread_ = new boost::thread(ThreadCreator<xyY, LCHab>
				  (this, xyy, lchab, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSLuv:{
      thread_ = new boost::thread(ThreadCreator<xyY, Luv>
				  (this, xyy, luv, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSLCHuv:{
      thread_ = new boost::thread(ThreadCreator<xyY, LCHuv>
				  (this, xyy, lchuv, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSAdobeRGB:{
      thread_ = new boost::thread(ThreadCreator<xyY, AdobeRGB>
				  (this, xyy, adobe, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSAppleRGB:{
      thread_ = new boost::thread(ThreadCreator<xyY, AppleRGB>
				  (this, xyy, apple, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSBestRGB:{
      thread_ = new boost::thread(ThreadCreator<xyY, BestRGB>
				  (this, xyy, best, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSBetaRGB:{
      thread_ = new boost::thread(ThreadCreator<xyY, BetaRGB>
				  (this, xyy, beta, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSBruceRGB:{
      thread_ = new boost::thread(ThreadCreator<xyY, BruceRGB>
				  (this, xyy, bruce, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSCIERGB:{
      thread_ = new boost::thread(ThreadCreator<xyY, CIERGB>
				  (this, xyy, cie, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSColorMatchRGB:{
      thread_ = new boost::thread(ThreadCreator<xyY, ColorMatchRGB>
				  (this, xyy, colormatch, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSDonRGB4:{
      thread_ = new boost::thread(ThreadCreator<xyY, DonRGB4>
				  (this, xyy, don4, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSECIRGB:{
      thread_ = new boost::thread(ThreadCreator<xyY, ECIRGB>
				  (this, xyy, eci, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSEktaSpacePS5:{
      thread_ = new boost::thread(ThreadCreator<xyY, EktaSpacePS5>
				  (this, xyy, ekta, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSNTSCRGB:{
      thread_ = new boost::thread(ThreadCreator<xyY, NTSCRGB>
				  (this, xyy, ntsc, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSPALSECAMRGB:{
      thread_ = new boost::thread(ThreadCreator<xyY, PAL_SECAMRGB>
				  (this, xyy, pal, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSProPhotoRGB:{
      thread_ = new boost::thread(ThreadCreator<xyY, ProPhotoRGB>
				  (this, xyy, pro, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSSMPTECRGB:{
      thread_ = new boost::thread(ThreadCreator<xyY, SMPTE_CRGB>
				  (this, xyy, smpt, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSsRGB:{
      thread_ = new boost::thread(ThreadCreator<xyY, sRGB>
				  (this, xyy, srgb, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSWideGamutRGB:{
      thread_ = new boost::thread(ThreadCreator<xyY, WideGamutRGB>
				  (this, xyy, widegamut, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    }
    break;
  }


  case CSLab:{  // From Lab to all other colour spaces.
    switch(scs_){
    case CSXYZ:{
      thread_ = new boost::thread(ThreadCreator<Lab, XYZ>
				  (this, lab, xyz, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSxyY:{
      thread_ = new boost::thread(ThreadCreator<Lab, xyY>
				  (this, lab, xyy, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSLab:{
      thread_ = new boost::thread(ThreadCreator<Lab, Lab>
				  (this, lab, lab, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSLCHab:{
      thread_ = new boost::thread(ThreadCreator<Lab, LCHab>
				  (this, lab, lchab, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSLuv:{
      thread_ = new boost::thread(ThreadCreator<Lab, Luv>
				  (this, lab, luv, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSLCHuv:{
      thread_ = new boost::thread(ThreadCreator<Lab, LCHuv>
				  (this, lab, lchuv, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSAdobeRGB:{
      thread_ = new boost::thread(ThreadCreator<Lab, AdobeRGB>
				  (this, lab, adobe, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSAppleRGB:{
      thread_ = new boost::thread(ThreadCreator<Lab, AppleRGB>
				  (this, lab, apple, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSBestRGB:{
      thread_ = new boost::thread(ThreadCreator<Lab, BestRGB>
				  (this, lab, best, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSBetaRGB:{
      thread_ = new boost::thread(ThreadCreator<Lab, BetaRGB>
				  (this, lab, beta, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSBruceRGB:{
      thread_ = new boost::thread(ThreadCreator<Lab, BruceRGB>
				  (this, lab, bruce, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSCIERGB:{
      thread_ = new boost::thread(ThreadCreator<Lab, CIERGB>
				  (this, lab, cie, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSColorMatchRGB:{
      thread_ = new boost::thread(ThreadCreator<Lab, ColorMatchRGB>
				  (this, lab, colormatch, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSDonRGB4:{
      thread_ = new boost::thread(ThreadCreator<Lab, DonRGB4>
				  (this, lab, don4, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSECIRGB:{
      thread_ = new boost::thread(ThreadCreator<Lab, ECIRGB>
				  (this, lab, eci, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSEktaSpacePS5:{
      thread_ = new boost::thread(ThreadCreator<Lab, EktaSpacePS5>
				  (this, lab, ekta, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSNTSCRGB:{
      thread_ = new boost::thread(ThreadCreator<Lab, NTSCRGB>
				  (this, lab, ntsc, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSPALSECAMRGB:{
      thread_ = new boost::thread(ThreadCreator<Lab, PAL_SECAMRGB>
				  (this, lab, pal, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSProPhotoRGB:{
      thread_ = new boost::thread(ThreadCreator<Lab, ProPhotoRGB>
				  (this, lab, pro, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSSMPTECRGB:{
      thread_ = new boost::thread(ThreadCreator<Lab, SMPTE_CRGB>
				  (this, lab, smpt, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSsRGB:{
      thread_ = new boost::thread(ThreadCreator<Lab, sRGB>
				  (this, lab, srgb, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSWideGamutRGB:{
      thread_ = new boost::thread(ThreadCreator<Lab, WideGamutRGB>
				  (this, lab, widegamut, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    }
    break;
  }


  case CSLCHab:{  // From LCHab to all other colour spaces.
    switch(scs_){
    case CSXYZ:{
      thread_ = new boost::thread(ThreadCreator<LCHab, XYZ>
				  (this, lchab, xyz, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSxyY:{
      thread_ = new boost::thread(ThreadCreator<LCHab, xyY>
				  (this, lchab, xyy, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSLab:{
      thread_ = new boost::thread(ThreadCreator<LCHab, Lab>
				  (this, lchab, lab, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSLCHab:{
      thread_ = new boost::thread(ThreadCreator<LCHab, LCHab>
				  (this, lchab, lchab, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSLuv:{
      thread_ = new boost::thread(ThreadCreator<LCHab, Luv>
				  (this, lchab, luv, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSLCHuv:{
      thread_ = new boost::thread(ThreadCreator<LCHab, LCHuv>
				  (this, lchab, lchuv, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSAdobeRGB:{
      thread_ = new boost::thread(ThreadCreator<LCHab, AdobeRGB>
				  (this, lchab, adobe, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSAppleRGB:{
      thread_ = new boost::thread(ThreadCreator<LCHab, AppleRGB>
				  (this, lchab, apple, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSBestRGB:{
      thread_ = new boost::thread(ThreadCreator<LCHab, BestRGB>
				  (this, lchab, best, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSBetaRGB:{
      thread_ = new boost::thread(ThreadCreator<LCHab, BetaRGB>
				  (this, lchab, beta, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSBruceRGB:{
      thread_ = new boost::thread(ThreadCreator<LCHab, BruceRGB>
				  (this, lchab, bruce, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSCIERGB:{
      thread_ = new boost::thread(ThreadCreator<LCHab, CIERGB>
				  (this, lchab, cie, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSColorMatchRGB:{
      thread_ = new boost::thread(ThreadCreator<LCHab, ColorMatchRGB>
				  (this, lchab, colormatch, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSDonRGB4:{
      thread_ = new boost::thread(ThreadCreator<LCHab, DonRGB4>
				  (this, lchab, don4, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSECIRGB:{
      thread_ = new boost::thread(ThreadCreator<LCHab, ECIRGB>
				  (this, lchab, eci, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSEktaSpacePS5:{
      thread_ = new boost::thread(ThreadCreator<LCHab, EktaSpacePS5>
				  (this, lchab, ekta, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSNTSCRGB:{
      thread_ = new boost::thread(ThreadCreator<LCHab, NTSCRGB>
				  (this, lchab, ntsc, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSPALSECAMRGB:{
      thread_ = new boost::thread(ThreadCreator<LCHab, PAL_SECAMRGB>
				  (this, lchab, pal, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSProPhotoRGB:{
      thread_ = new boost::thread(ThreadCreator<LCHab, ProPhotoRGB>
				  (this, lchab, pro, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSSMPTECRGB:{
      thread_ = new boost::thread(ThreadCreator<LCHab, SMPTE_CRGB>
				  (this, lchab, smpt, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSsRGB:{
      thread_ = new boost::thread(ThreadCreator<LCHab, sRGB>
				  (this, lchab, srgb, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSWideGamutRGB:{
      thread_ = new boost::thread(ThreadCreator<LCHab, WideGamutRGB>
				  (this, lchab, widegamut, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    }
    break;
  }


  case CSLuv:{  // From Luv to all other colour spaces.
    switch(scs_){
    case CSXYZ:{
      thread_ = new boost::thread(ThreadCreator<Luv, XYZ>
				  (this, luv, xyz, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSxyY:{
      thread_ = new boost::thread(ThreadCreator<Luv, xyY>
				  (this, luv, xyy, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSLab:{
      thread_ = new boost::thread(ThreadCreator<Luv, Lab>
				  (this, luv, lab, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSLCHab:{
      thread_ = new boost::thread(ThreadCreator<Luv, LCHab>
				  (this, luv, lchab, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSLuv:{
      thread_ = new boost::thread(ThreadCreator<Luv, Luv>
				  (this, luv, luv, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSLCHuv:{
      thread_ = new boost::thread(ThreadCreator<Luv, LCHuv>
				  (this, luv, lchuv, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSAdobeRGB:{
      thread_ = new boost::thread(ThreadCreator<Luv, AdobeRGB>
				  (this, luv, adobe, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSAppleRGB:{
      thread_ = new boost::thread(ThreadCreator<Luv, AppleRGB>
				  (this, luv, apple, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSBestRGB:{
      thread_ = new boost::thread(ThreadCreator<Luv, BestRGB>
				  (this, luv, best, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSBetaRGB:{
      thread_ = new boost::thread(ThreadCreator<Luv, BetaRGB>
				  (this, luv, beta, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSBruceRGB:{
      thread_ = new boost::thread(ThreadCreator<Luv, BruceRGB>
				  (this, luv, bruce, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSCIERGB:{
      thread_ = new boost::thread(ThreadCreator<Luv, CIERGB>
				  (this, luv, cie, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSColorMatchRGB:{
      thread_ = new boost::thread(ThreadCreator<Luv, ColorMatchRGB>
				  (this, luv, colormatch, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSDonRGB4:{
      thread_ = new boost::thread(ThreadCreator<Luv, DonRGB4>
				  (this, luv, don4, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSECIRGB:{
      thread_ = new boost::thread(ThreadCreator<Luv, ECIRGB>
				  (this, luv, eci, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSEktaSpacePS5:{
      thread_ = new boost::thread(ThreadCreator<Luv, EktaSpacePS5>
				  (this, luv, ekta, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSNTSCRGB:{
      thread_ = new boost::thread(ThreadCreator<Luv, NTSCRGB>
				  (this, luv, ntsc, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSPALSECAMRGB:{
      thread_ = new boost::thread(ThreadCreator<Luv, PAL_SECAMRGB>
				  (this, luv, pal, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSProPhotoRGB:{
      thread_ = new boost::thread(ThreadCreator<Luv, ProPhotoRGB>
				  (this, luv, pro, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSSMPTECRGB:{
      thread_ = new boost::thread(ThreadCreator<Luv, SMPTE_CRGB>
				  (this, luv, smpt, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSsRGB:{
      thread_ = new boost::thread(ThreadCreator<Luv, sRGB>
				  (this, luv, srgb, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSWideGamutRGB:{
      thread_ = new boost::thread(ThreadCreator<Luv, WideGamutRGB>
				  (this, luv, widegamut, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    }
    break;
  }


  case CSLCHuv:{  // From LCHuv to all other colour spaces.
    switch(scs_){
    case CSXYZ:{
      thread_ = new boost::thread(ThreadCreator<LCHuv, XYZ>
				  (this, lchuv, xyz, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSxyY:{
      thread_ = new boost::thread(ThreadCreator<LCHuv, xyY>
				  (this, lchuv, xyy, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSLab:{
      thread_ = new boost::thread(ThreadCreator<LCHuv, Lab>
				  (this, lchuv, lab, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSLCHab:{
      thread_ = new boost::thread(ThreadCreator<LCHuv, LCHab>
				  (this, lchuv, lchab, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSLuv:{
      thread_ = new boost::thread(ThreadCreator<LCHuv, Luv>
				  (this, lchuv, luv, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSLCHuv:{
      thread_ = new boost::thread(ThreadCreator<LCHuv, LCHuv>
				  (this, lchuv, lchuv, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSAdobeRGB:{
      thread_ = new boost::thread(ThreadCreator<LCHuv, AdobeRGB>
				  (this, lchuv, adobe, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSAppleRGB:{
      thread_ = new boost::thread(ThreadCreator<LCHuv, AppleRGB>
				  (this, lchuv, apple, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSBestRGB:{
      thread_ = new boost::thread(ThreadCreator<LCHuv, BestRGB>
				  (this, lchuv, best, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSBetaRGB:{
      thread_ = new boost::thread(ThreadCreator<LCHuv, BetaRGB>
				  (this, lchuv, beta, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSBruceRGB:{
      thread_ = new boost::thread(ThreadCreator<LCHuv, BruceRGB>
				  (this, lchuv, bruce, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSCIERGB:{
      thread_ = new boost::thread(ThreadCreator<LCHuv, CIERGB>
				  (this, lchuv, cie, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSColorMatchRGB:{
      thread_ = new boost::thread(ThreadCreator<LCHuv, ColorMatchRGB>
				  (this, lchuv, colormatch, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSDonRGB4:{
      thread_ = new boost::thread(ThreadCreator<LCHuv, DonRGB4>
				  (this, lchuv, don4, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSECIRGB:{
      thread_ = new boost::thread(ThreadCreator<LCHuv, ECIRGB>
				  (this, lchuv, eci, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSEktaSpacePS5:{
      thread_ = new boost::thread(ThreadCreator<LCHuv, EktaSpacePS5>
				  (this, lchuv, ekta, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSNTSCRGB:{
      thread_ = new boost::thread(ThreadCreator<LCHuv, NTSCRGB>
				  (this, lchuv, ntsc, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSPALSECAMRGB:{
      thread_ = new boost::thread(ThreadCreator<LCHuv, PAL_SECAMRGB>
				  (this, lchuv, pal, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSProPhotoRGB:{
      thread_ = new boost::thread(ThreadCreator<LCHuv, ProPhotoRGB>
				  (this, lchuv, pro, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSSMPTECRGB:{
      thread_ = new boost::thread(ThreadCreator<LCHuv, SMPTE_CRGB>
				  (this, lchuv, smpt, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSsRGB:{
      thread_ = new boost::thread(ThreadCreator<LCHuv, sRGB>
				  (this, lchuv, srgb, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSWideGamutRGB:{
      thread_ = new boost::thread(ThreadCreator<LCHuv, WideGamutRGB>
				  (this, lchuv, widegamut, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    }
    break;
  }


  case CSAdobeRGB:{  // From AdobeRGB to all other colour spaces.
    switch(scs_){
    case CSXYZ:{
      thread_ = new boost::thread(ThreadCreator<AdobeRGB, XYZ>
				  (this, adobe, xyz, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSxyY:{
      thread_ = new boost::thread(ThreadCreator<AdobeRGB, xyY>
				  (this, adobe, xyy, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSLab:{
      thread_ = new boost::thread(ThreadCreator<AdobeRGB, Lab>
				  (this, adobe, lab, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSLCHab:{
      thread_ = new boost::thread(ThreadCreator<AdobeRGB, LCHab>
				  (this, adobe, lchab, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSLuv:{
      thread_ = new boost::thread(ThreadCreator<AdobeRGB, Luv>
				  (this, adobe, luv, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSLCHuv:{
      thread_ = new boost::thread(ThreadCreator<AdobeRGB, LCHuv>
				  (this, adobe, lchuv, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSAdobeRGB:{
      thread_ = new boost::thread(ThreadCreator<AdobeRGB, AdobeRGB>
				  (this, adobe, adobe, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSAppleRGB:{
      thread_ = new boost::thread(ThreadCreator<AdobeRGB, AppleRGB>
				  (this, adobe, apple, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSBestRGB:{
      thread_ = new boost::thread(ThreadCreator<AdobeRGB, BestRGB>
				  (this, adobe, best, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSBetaRGB:{
      thread_ = new boost::thread(ThreadCreator<AdobeRGB, BetaRGB>
				  (this, adobe, beta, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSBruceRGB:{
      thread_ = new boost::thread(ThreadCreator<AdobeRGB, BruceRGB>
				  (this, adobe, bruce, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSCIERGB:{
      thread_ = new boost::thread(ThreadCreator<AdobeRGB, CIERGB>
				  (this, adobe, cie, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSColorMatchRGB:{
      thread_ = new boost::thread(ThreadCreator<AdobeRGB, ColorMatchRGB>
				  (this, adobe, colormatch, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSDonRGB4:{
      thread_ = new boost::thread(ThreadCreator<AdobeRGB, DonRGB4>
				  (this, adobe, don4, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSECIRGB:{
      thread_ = new boost::thread(ThreadCreator<AdobeRGB, ECIRGB>
				  (this, adobe, eci, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSEktaSpacePS5:{
      thread_ = new boost::thread(ThreadCreator<AdobeRGB, EktaSpacePS5>
				  (this, adobe, ekta, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSNTSCRGB:{
      thread_ = new boost::thread(ThreadCreator<AdobeRGB, NTSCRGB>
				  (this, adobe, ntsc, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSPALSECAMRGB:{
      thread_ = new boost::thread(ThreadCreator<AdobeRGB, PAL_SECAMRGB>
				  (this, adobe, pal, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSProPhotoRGB:{
      thread_ = new boost::thread(ThreadCreator<AdobeRGB, ProPhotoRGB>
				  (this, adobe, pro, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSSMPTECRGB:{
      thread_ = new boost::thread(ThreadCreator<AdobeRGB, SMPTE_CRGB>
				  (this, adobe, smpt, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSsRGB:{
      thread_ = new boost::thread(ThreadCreator<AdobeRGB, sRGB>
				  (this, adobe, srgb, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSWideGamutRGB:{
      thread_ = new boost::thread(ThreadCreator<AdobeRGB, WideGamutRGB>
				  (this, adobe, widegamut, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    }
    break;
  }


  case CSAppleRGB:{  // From AppleRGB to all other colour spaces.
    switch(scs_){
    case CSXYZ:{
      thread_ = new boost::thread(ThreadCreator<AppleRGB, XYZ>
				  (this, apple, xyz, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSxyY:{
      thread_ = new boost::thread(ThreadCreator<AppleRGB, xyY>
				  (this, apple, xyy, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSLab:{
      thread_ = new boost::thread(ThreadCreator<AppleRGB, Lab>
				  (this, apple, lab, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSLCHab:{
      thread_ = new boost::thread(ThreadCreator<AppleRGB, LCHab>
				  (this, apple, lchab, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSLuv:{
      thread_ = new boost::thread(ThreadCreator<AppleRGB, Luv>
				  (this, apple, luv, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSLCHuv:{
      thread_ = new boost::thread(ThreadCreator<AppleRGB, LCHuv>
				  (this, apple, lchuv, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSAdobeRGB:{
      thread_ = new boost::thread(ThreadCreator<AppleRGB, AdobeRGB>
				  (this, apple, adobe, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSAppleRGB:{
      thread_ = new boost::thread(ThreadCreator<AppleRGB, AppleRGB>
				  (this, apple, apple, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSBestRGB:{
      thread_ = new boost::thread(ThreadCreator<AppleRGB, BestRGB>
				  (this, apple, best, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSBetaRGB:{
      thread_ = new boost::thread(ThreadCreator<AppleRGB, BetaRGB>
				  (this, apple, beta, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSBruceRGB:{
      thread_ = new boost::thread(ThreadCreator<AppleRGB, BruceRGB>
				  (this, apple, bruce, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSCIERGB:{
      thread_ = new boost::thread(ThreadCreator<AppleRGB, CIERGB>
				  (this, apple, cie, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSColorMatchRGB:{
      thread_ = new boost::thread(ThreadCreator<AppleRGB, ColorMatchRGB>
				  (this, apple, colormatch, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSDonRGB4:{
      thread_ = new boost::thread(ThreadCreator<AppleRGB, DonRGB4>
				  (this, apple, don4, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSECIRGB:{
      thread_ = new boost::thread(ThreadCreator<AppleRGB, ECIRGB>
				  (this, apple, eci, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSEktaSpacePS5:{
      thread_ = new boost::thread(ThreadCreator<AppleRGB, EktaSpacePS5>
				  (this, apple, ekta, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSNTSCRGB:{
      thread_ = new boost::thread(ThreadCreator<AppleRGB, NTSCRGB>
				  (this, apple, ntsc, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSPALSECAMRGB:{
      thread_ = new boost::thread(ThreadCreator<AppleRGB, PAL_SECAMRGB>
				  (this, apple, pal, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSProPhotoRGB:{
      thread_ = new boost::thread(ThreadCreator<AppleRGB, ProPhotoRGB>
				  (this, apple, pro, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSSMPTECRGB:{
      thread_ = new boost::thread(ThreadCreator<AppleRGB, SMPTE_CRGB>
				  (this, apple, smpt, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSsRGB:{
      thread_ = new boost::thread(ThreadCreator<AppleRGB, sRGB>
				  (this, apple, srgb, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSWideGamutRGB:{
      thread_ = new boost::thread(ThreadCreator<AppleRGB, WideGamutRGB>
				  (this, apple, widegamut, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    }
    break;
  }


  case CSBestRGB:{  // From BestRGB to all other colour spaces.
    switch(scs_){
    case CSXYZ:{
      thread_ = new boost::thread(ThreadCreator<BestRGB, XYZ>
				  (this, best, xyz, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSxyY:{
      thread_ = new boost::thread(ThreadCreator<BestRGB, xyY>
				  (this, best, xyy, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSLab:{
      thread_ = new boost::thread(ThreadCreator<BestRGB, Lab>
				  (this, best, lab, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSLCHab:{
      thread_ = new boost::thread(ThreadCreator<BestRGB, LCHab>
				  (this, best, lchab, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSLuv:{
      thread_ = new boost::thread(ThreadCreator<BestRGB, Luv>
				  (this, best, luv, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSLCHuv:{
      thread_ = new boost::thread(ThreadCreator<BestRGB, LCHuv>
				  (this, best, lchuv, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSAdobeRGB:{
      thread_ = new boost::thread(ThreadCreator<BestRGB, AdobeRGB>
				  (this, best, adobe, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSAppleRGB:{
      thread_ = new boost::thread(ThreadCreator<BestRGB, AppleRGB>
				  (this, best, apple, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSBestRGB:{
      thread_ = new boost::thread(ThreadCreator<BestRGB, BestRGB>
				  (this, best, best, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSBetaRGB:{
      thread_ = new boost::thread(ThreadCreator<BestRGB, BetaRGB>
				  (this, best, beta, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSBruceRGB:{
      thread_ = new boost::thread(ThreadCreator<BestRGB, BruceRGB>
				  (this, best, bruce, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSCIERGB:{
      thread_ = new boost::thread(ThreadCreator<BestRGB, CIERGB>
				  (this, best, cie, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSColorMatchRGB:{
      thread_ = new boost::thread(ThreadCreator<BestRGB, ColorMatchRGB>
				  (this, best, colormatch, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSDonRGB4:{
      thread_ = new boost::thread(ThreadCreator<BestRGB, DonRGB4>
				  (this, best, don4, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSECIRGB:{
      thread_ = new boost::thread(ThreadCreator<BestRGB, ECIRGB>
				  (this, best, eci, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSEktaSpacePS5:{
      thread_ = new boost::thread(ThreadCreator<BestRGB, EktaSpacePS5>
				  (this, best, ekta, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSNTSCRGB:{
      thread_ = new boost::thread(ThreadCreator<BestRGB, NTSCRGB>
				  (this, best, ntsc, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSPALSECAMRGB:{
      thread_ = new boost::thread(ThreadCreator<BestRGB, PAL_SECAMRGB>
				  (this, best, pal, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSProPhotoRGB:{
      thread_ = new boost::thread(ThreadCreator<BestRGB, ProPhotoRGB>
				  (this, best, pro, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSSMPTECRGB:{
      thread_ = new boost::thread(ThreadCreator<BestRGB, SMPTE_CRGB>
				  (this, best, smpt, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSsRGB:{
      thread_ = new boost::thread(ThreadCreator<BestRGB, sRGB>
				  (this, best, srgb, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSWideGamutRGB:{
      thread_ = new boost::thread(ThreadCreator<BestRGB, WideGamutRGB>
				  (this, best, widegamut, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    }
    break;
  }


  case CSBetaRGB:{  // From BetaRGB to all other colour spaces.
    switch(scs_){
    case CSXYZ:{
      thread_ = new boost::thread(ThreadCreator<BetaRGB, XYZ>
				  (this, beta, xyz, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSxyY:{
      thread_ = new boost::thread(ThreadCreator<BetaRGB, xyY>
				  (this, beta, xyy, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSLab:{
      thread_ = new boost::thread(ThreadCreator<BetaRGB, Lab>
				  (this, beta, lab, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSLCHab:{
      thread_ = new boost::thread(ThreadCreator<BetaRGB, LCHab>
				  (this, beta, lchab, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSLuv:{
      thread_ = new boost::thread(ThreadCreator<BetaRGB, Luv>
				  (this, beta, luv, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSLCHuv:{
      thread_ = new boost::thread(ThreadCreator<BetaRGB, LCHuv>
				  (this, beta, lchuv, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSAdobeRGB:{
      thread_ = new boost::thread(ThreadCreator<BetaRGB, AdobeRGB>
				  (this, beta, adobe, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSAppleRGB:{
      thread_ = new boost::thread(ThreadCreator<BetaRGB, AppleRGB>
				  (this, beta, apple, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSBestRGB:{
      thread_ = new boost::thread(ThreadCreator<BetaRGB, BestRGB>
				  (this, beta, best, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSBetaRGB:{
      thread_ = new boost::thread(ThreadCreator<BetaRGB, BetaRGB>
				  (this, beta, beta, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSBruceRGB:{
      thread_ = new boost::thread(ThreadCreator<BetaRGB, BruceRGB>
				  (this, beta, bruce, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSCIERGB:{
      thread_ = new boost::thread(ThreadCreator<BetaRGB, CIERGB>
				  (this, beta, cie, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSColorMatchRGB:{
      thread_ = new boost::thread(ThreadCreator<BetaRGB, ColorMatchRGB>
				  (this, beta, colormatch, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSDonRGB4:{
      thread_ = new boost::thread(ThreadCreator<BetaRGB, DonRGB4>
				  (this, beta, don4, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSECIRGB:{
      thread_ = new boost::thread(ThreadCreator<BetaRGB, ECIRGB>
				  (this, beta, eci, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSEktaSpacePS5:{
      thread_ = new boost::thread(ThreadCreator<BetaRGB, EktaSpacePS5>
				  (this, beta, ekta, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSNTSCRGB:{
      thread_ = new boost::thread(ThreadCreator<BetaRGB, NTSCRGB>
				  (this, beta, ntsc, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSPALSECAMRGB:{
      thread_ = new boost::thread(ThreadCreator<BetaRGB, PAL_SECAMRGB>
				  (this, beta, pal, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSProPhotoRGB:{
      thread_ = new boost::thread(ThreadCreator<BetaRGB, ProPhotoRGB>
				  (this, beta, pro, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSSMPTECRGB:{
      thread_ = new boost::thread(ThreadCreator<BetaRGB, SMPTE_CRGB>
				  (this, beta, smpt, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSsRGB:{
      thread_ = new boost::thread(ThreadCreator<BetaRGB, sRGB>
				  (this, beta, srgb, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSWideGamutRGB:{
      thread_ = new boost::thread(ThreadCreator<BetaRGB, WideGamutRGB>
				  (this, beta, widegamut, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    }
    break;
  }


  case CSBruceRGB:{  // From BruceRGB to all other colour spaces.
    switch(scs_){
    case CSXYZ:{
      thread_ = new boost::thread(ThreadCreator<BruceRGB, XYZ>
				  (this, bruce, xyz, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSxyY:{
      thread_ = new boost::thread(ThreadCreator<BruceRGB, xyY>
				  (this, bruce, xyy, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSLab:{
      thread_ = new boost::thread(ThreadCreator<BruceRGB, Lab>
				  (this, bruce, lab, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSLCHab:{
      thread_ = new boost::thread(ThreadCreator<BruceRGB, LCHab>
				  (this, bruce, lchab, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSLuv:{
      thread_ = new boost::thread(ThreadCreator<BruceRGB, Luv>
				  (this, bruce, luv, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSLCHuv:{
      thread_ = new boost::thread(ThreadCreator<BruceRGB, LCHuv>
				  (this, bruce, lchuv, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSAdobeRGB:{
      thread_ = new boost::thread(ThreadCreator<BruceRGB, AdobeRGB>
				  (this, bruce, adobe, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSAppleRGB:{
      thread_ = new boost::thread(ThreadCreator<BruceRGB, AppleRGB>
				  (this, bruce, apple, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSBestRGB:{
      thread_ = new boost::thread(ThreadCreator<BruceRGB, BestRGB>
				  (this, bruce, best, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSBetaRGB:{
      thread_ = new boost::thread(ThreadCreator<BruceRGB, BetaRGB>
				  (this, bruce, beta, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSBruceRGB:{
      thread_ = new boost::thread(ThreadCreator<BruceRGB, BruceRGB>
				  (this, bruce, bruce, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSCIERGB:{
      thread_ = new boost::thread(ThreadCreator<BruceRGB, CIERGB>
				  (this, bruce, cie, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSColorMatchRGB:{
      thread_ = new boost::thread(ThreadCreator<BruceRGB, ColorMatchRGB>
				  (this, bruce, colormatch, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSDonRGB4:{
      thread_ = new boost::thread(ThreadCreator<BruceRGB, DonRGB4>
				  (this, bruce, don4, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSECIRGB:{
      thread_ = new boost::thread(ThreadCreator<BruceRGB, ECIRGB>
				  (this, bruce, eci, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSEktaSpacePS5:{
      thread_ = new boost::thread(ThreadCreator<BruceRGB, EktaSpacePS5>
				  (this, bruce, ekta, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSNTSCRGB:{
      thread_ = new boost::thread(ThreadCreator<BruceRGB, NTSCRGB>
				  (this, bruce, ntsc, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSPALSECAMRGB:{
      thread_ = new boost::thread(ThreadCreator<BruceRGB, PAL_SECAMRGB>
				  (this, bruce, pal, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSProPhotoRGB:{
      thread_ = new boost::thread(ThreadCreator<BruceRGB, ProPhotoRGB>
				  (this, bruce, pro, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSSMPTECRGB:{
      thread_ = new boost::thread(ThreadCreator<BruceRGB, SMPTE_CRGB>
				  (this, bruce, smpt, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSsRGB:{
      thread_ = new boost::thread(ThreadCreator<BruceRGB, sRGB>
				  (this, bruce, srgb, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSWideGamutRGB:{
      thread_ = new boost::thread(ThreadCreator<BruceRGB, WideGamutRGB>
				  (this, bruce, widegamut, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    }
    break;
  }


  case CSCIERGB:{  // From CIERGB to all other colour spaces.
    switch(scs_){
    case CSXYZ:{
      thread_ = new boost::thread(ThreadCreator<CIERGB, XYZ>
				  (this, cie, xyz, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSxyY:{
      thread_ = new boost::thread(ThreadCreator<CIERGB, xyY>
				  (this, cie, xyy, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSLab:{
      thread_ = new boost::thread(ThreadCreator<CIERGB, Lab>
				  (this, cie, lab, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSLCHab:{
      thread_ = new boost::thread(ThreadCreator<CIERGB, LCHab>
				  (this, cie, lchab, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSLuv:{
      thread_ = new boost::thread(ThreadCreator<CIERGB, Luv>
				  (this, cie, luv, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSLCHuv:{
      thread_ = new boost::thread(ThreadCreator<CIERGB, LCHuv>
				  (this, cie, lchuv, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSAdobeRGB:{
      thread_ = new boost::thread(ThreadCreator<CIERGB, AdobeRGB>
				  (this, cie, adobe, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSAppleRGB:{
      thread_ = new boost::thread(ThreadCreator<CIERGB, AppleRGB>
				  (this, cie, apple, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSBestRGB:{
      thread_ = new boost::thread(ThreadCreator<CIERGB, BestRGB>
				  (this, cie, best, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSBetaRGB:{
      thread_ = new boost::thread(ThreadCreator<CIERGB, BetaRGB>
				  (this, cie, beta, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSBruceRGB:{
      thread_ = new boost::thread(ThreadCreator<CIERGB, BruceRGB>
				  (this, cie, bruce, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSCIERGB:{
      thread_ = new boost::thread(ThreadCreator<CIERGB, CIERGB>
				  (this, cie, cie, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSColorMatchRGB:{
      thread_ = new boost::thread(ThreadCreator<CIERGB, ColorMatchRGB>
				  (this, cie, colormatch, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSDonRGB4:{
      thread_ = new boost::thread(ThreadCreator<CIERGB, DonRGB4>
				  (this, cie, don4, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSECIRGB:{
      thread_ = new boost::thread(ThreadCreator<CIERGB, ECIRGB>
				  (this, cie, eci, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSEktaSpacePS5:{
      thread_ = new boost::thread(ThreadCreator<CIERGB, EktaSpacePS5>
				  (this, cie, ekta, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSNTSCRGB:{
      thread_ = new boost::thread(ThreadCreator<CIERGB, NTSCRGB>
				  (this, cie, ntsc, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSPALSECAMRGB:{
      thread_ = new boost::thread(ThreadCreator<CIERGB, PAL_SECAMRGB>
				  (this, cie, pal, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSProPhotoRGB:{
      thread_ = new boost::thread(ThreadCreator<CIERGB, ProPhotoRGB>
				  (this, cie, pro, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSSMPTECRGB:{
      thread_ = new boost::thread(ThreadCreator<CIERGB, SMPTE_CRGB>
				  (this, cie, smpt, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSsRGB:{
      thread_ = new boost::thread(ThreadCreator<CIERGB, sRGB>
				  (this, cie, srgb, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSWideGamutRGB:{
      thread_ = new boost::thread(ThreadCreator<CIERGB, WideGamutRGB>
				  (this, cie, widegamut, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    }
    break;
  }


  case CSColorMatchRGB:{  // From ColorMatchRGB to all other colour spaces.
    switch(scs_){
    case CSXYZ:{
      thread_ = new boost::thread(ThreadCreator<ColorMatchRGB, XYZ>
				  (this, colormatch, xyz, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSxyY:{
      thread_ = new boost::thread(ThreadCreator<ColorMatchRGB, xyY>
				  (this, colormatch, xyy, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSLab:{
      thread_ = new boost::thread(ThreadCreator<ColorMatchRGB, Lab>
				  (this, colormatch, lab, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSLCHab:{
      thread_ = new boost::thread(ThreadCreator<ColorMatchRGB, LCHab>
				  (this, colormatch, lchab, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSLuv:{
      thread_ = new boost::thread(ThreadCreator<ColorMatchRGB, Luv>
				  (this, colormatch, luv, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSLCHuv:{
      thread_ = new boost::thread(ThreadCreator<ColorMatchRGB, LCHuv>
				  (this, colormatch, lchuv, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSAdobeRGB:{
      thread_ = new boost::thread(ThreadCreator<ColorMatchRGB, AdobeRGB>
				  (this, colormatch, adobe, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSAppleRGB:{
      thread_ = new boost::thread(ThreadCreator<ColorMatchRGB, AppleRGB>
				  (this, colormatch, apple, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSBestRGB:{
      thread_ = new boost::thread(ThreadCreator<ColorMatchRGB, BestRGB>
				  (this, colormatch, best, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSBetaRGB:{
      thread_ = new boost::thread(ThreadCreator<ColorMatchRGB, BetaRGB>
				  (this, colormatch, beta, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSBruceRGB:{
      thread_ = new boost::thread(ThreadCreator<ColorMatchRGB, BruceRGB>
				  (this, colormatch, bruce, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSCIERGB:{
      thread_ = new boost::thread(ThreadCreator<ColorMatchRGB, CIERGB>
				  (this, colormatch, cie, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSColorMatchRGB:{
      thread_ = new boost::thread(ThreadCreator<ColorMatchRGB, ColorMatchRGB>
				  (this, colormatch, colormatch, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSDonRGB4:{
      thread_ = new boost::thread(ThreadCreator<ColorMatchRGB, DonRGB4>
				  (this, colormatch, don4, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSECIRGB:{
      thread_ = new boost::thread(ThreadCreator<ColorMatchRGB, ECIRGB>
				  (this, colormatch, eci, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSEktaSpacePS5:{
      thread_ = new boost::thread(ThreadCreator<ColorMatchRGB, EktaSpacePS5>
				  (this, colormatch, ekta, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSNTSCRGB:{
      thread_ = new boost::thread(ThreadCreator<ColorMatchRGB, NTSCRGB>
				  (this, colormatch, ntsc, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSPALSECAMRGB:{
      thread_ = new boost::thread(ThreadCreator<ColorMatchRGB, PAL_SECAMRGB>
				  (this, colormatch, pal, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSProPhotoRGB:{
      thread_ = new boost::thread(ThreadCreator<ColorMatchRGB, ProPhotoRGB>
				  (this, colormatch, pro, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSSMPTECRGB:{
      thread_ = new boost::thread(ThreadCreator<ColorMatchRGB, SMPTE_CRGB>
				  (this, colormatch, smpt, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSsRGB:{
      thread_ = new boost::thread(ThreadCreator<ColorMatchRGB, sRGB>
				  (this, colormatch, srgb, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSWideGamutRGB:{
      thread_ = new boost::thread(ThreadCreator<ColorMatchRGB, WideGamutRGB>
				  (this, colormatch, widegamut, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    }
    break;
  }


  case CSDonRGB4:{  // From DonRGB4 to all other colour spaces.
    switch(scs_){
    case CSXYZ:{
      thread_ = new boost::thread(ThreadCreator<DonRGB4, XYZ>
				  (this, don4, xyz, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSxyY:{
      thread_ = new boost::thread(ThreadCreator<DonRGB4, xyY>
				  (this, don4, xyy, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSLab:{
      thread_ = new boost::thread(ThreadCreator<DonRGB4, Lab>
				  (this, don4, lab, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSLCHab:{
      thread_ = new boost::thread(ThreadCreator<DonRGB4, LCHab>
				  (this, don4, lchab, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSLuv:{
      thread_ = new boost::thread(ThreadCreator<DonRGB4, Luv>
				  (this, don4, luv, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSLCHuv:{
      thread_ = new boost::thread(ThreadCreator<DonRGB4, LCHuv>
				  (this, don4, lchuv, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSAdobeRGB:{
      thread_ = new boost::thread(ThreadCreator<DonRGB4, AdobeRGB>
				  (this, don4, adobe, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSAppleRGB:{
      thread_ = new boost::thread(ThreadCreator<DonRGB4, AppleRGB>
				  (this, don4, apple, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSBestRGB:{
      thread_ = new boost::thread(ThreadCreator<DonRGB4, BestRGB>
				  (this, don4, best, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSBetaRGB:{
      thread_ = new boost::thread(ThreadCreator<DonRGB4, BetaRGB>
				  (this, don4, beta, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSBruceRGB:{
      thread_ = new boost::thread(ThreadCreator<DonRGB4, BruceRGB>
				  (this, don4, bruce, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSCIERGB:{
      thread_ = new boost::thread(ThreadCreator<DonRGB4, CIERGB>
				  (this, don4, cie, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSColorMatchRGB:{
      thread_ = new boost::thread(ThreadCreator<DonRGB4, ColorMatchRGB>
				  (this, don4, colormatch, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSDonRGB4:{
      thread_ = new boost::thread(ThreadCreator<DonRGB4, DonRGB4>
				  (this, don4, don4, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSECIRGB:{
      thread_ = new boost::thread(ThreadCreator<DonRGB4, ECIRGB>
				  (this, don4, eci, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSEktaSpacePS5:{
      thread_ = new boost::thread(ThreadCreator<DonRGB4, EktaSpacePS5>
				  (this, don4, ekta, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSNTSCRGB:{
      thread_ = new boost::thread(ThreadCreator<DonRGB4, NTSCRGB>
				  (this, don4, ntsc, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSPALSECAMRGB:{
      thread_ = new boost::thread(ThreadCreator<DonRGB4, PAL_SECAMRGB>
				  (this, don4, pal, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSProPhotoRGB:{
      thread_ = new boost::thread(ThreadCreator<DonRGB4, ProPhotoRGB>
				  (this, don4, pro, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSSMPTECRGB:{
      thread_ = new boost::thread(ThreadCreator<DonRGB4, SMPTE_CRGB>
				  (this, don4, smpt, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSsRGB:{
      thread_ = new boost::thread(ThreadCreator<DonRGB4, sRGB>
				  (this, don4, srgb, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSWideGamutRGB:{
      thread_ = new boost::thread(ThreadCreator<DonRGB4, WideGamutRGB>
				  (this, don4, widegamut, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    }
    break;
  }


  case CSECIRGB:{  // From ECIRGB to all other colour spaces.
    switch(scs_){
    case CSXYZ:{
      thread_ = new boost::thread(ThreadCreator<ECIRGB, XYZ>
				  (this, eci, xyz, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSxyY:{
      thread_ = new boost::thread(ThreadCreator<ECIRGB, xyY>
				  (this, eci, xyy, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSLab:{
      thread_ = new boost::thread(ThreadCreator<ECIRGB, Lab>
				  (this, eci, lab, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSLCHab:{
      thread_ = new boost::thread(ThreadCreator<ECIRGB, LCHab>
				  (this, eci, lchab, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSLuv:{
      thread_ = new boost::thread(ThreadCreator<ECIRGB, Luv>
				  (this, eci, luv, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSLCHuv:{
      thread_ = new boost::thread(ThreadCreator<ECIRGB, LCHuv>
				  (this, eci, lchuv, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSAdobeRGB:{
      thread_ = new boost::thread(ThreadCreator<ECIRGB, AdobeRGB>
				  (this, eci, adobe, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSAppleRGB:{
      thread_ = new boost::thread(ThreadCreator<ECIRGB, AppleRGB>
				  (this, eci, apple, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSBestRGB:{
      thread_ = new boost::thread(ThreadCreator<ECIRGB, BestRGB>
				  (this, eci, best, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSBetaRGB:{
      thread_ = new boost::thread(ThreadCreator<ECIRGB, BetaRGB>
				  (this, eci, beta, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSBruceRGB:{
      thread_ = new boost::thread(ThreadCreator<ECIRGB, BruceRGB>
				  (this, eci, bruce, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSCIERGB:{
      thread_ = new boost::thread(ThreadCreator<ECIRGB, CIERGB>
				  (this, eci, cie, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSColorMatchRGB:{
      thread_ = new boost::thread(ThreadCreator<ECIRGB, ColorMatchRGB>
				  (this, eci, colormatch, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSDonRGB4:{
      thread_ = new boost::thread(ThreadCreator<ECIRGB, DonRGB4>
				  (this, eci, don4, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSECIRGB:{
      thread_ = new boost::thread(ThreadCreator<ECIRGB, ECIRGB>
				  (this, eci, eci, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSEktaSpacePS5:{
      thread_ = new boost::thread(ThreadCreator<ECIRGB, EktaSpacePS5>
				  (this, eci, ekta, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSNTSCRGB:{
      thread_ = new boost::thread(ThreadCreator<ECIRGB, NTSCRGB>
				  (this, eci, ntsc, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSPALSECAMRGB:{
      thread_ = new boost::thread(ThreadCreator<ECIRGB, PAL_SECAMRGB>
				  (this, eci, pal, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSProPhotoRGB:{
      thread_ = new boost::thread(ThreadCreator<ECIRGB, ProPhotoRGB>
				  (this, eci, pro, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSSMPTECRGB:{
      thread_ = new boost::thread(ThreadCreator<ECIRGB, SMPTE_CRGB>
				  (this, eci, smpt, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSsRGB:{
      thread_ = new boost::thread(ThreadCreator<ECIRGB, sRGB>
				  (this, eci, srgb, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSWideGamutRGB:{
      thread_ = new boost::thread(ThreadCreator<ECIRGB, WideGamutRGB>
				  (this, eci, widegamut, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    }
    break;
  }


  case CSEktaSpacePS5:{  // From EktaSpacePS5 to all other colour spaces.
    switch(scs_){
    case CSXYZ:{
      thread_ = new boost::thread(ThreadCreator<EktaSpacePS5, XYZ>
				  (this, ekta, xyz, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSxyY:{
      thread_ = new boost::thread(ThreadCreator<EktaSpacePS5, xyY>
				  (this, ekta, xyy, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSLab:{
      thread_ = new boost::thread(ThreadCreator<EktaSpacePS5, Lab>
				  (this, ekta, lab, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSLCHab:{
      thread_ = new boost::thread(ThreadCreator<EktaSpacePS5, LCHab>
				  (this, ekta, lchab, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSLuv:{
      thread_ = new boost::thread(ThreadCreator<EktaSpacePS5, Luv>
				  (this, ekta, luv, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSLCHuv:{
      thread_ = new boost::thread(ThreadCreator<EktaSpacePS5, LCHuv>
				  (this, ekta, lchuv, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSAdobeRGB:{
      thread_ = new boost::thread(ThreadCreator<EktaSpacePS5, AdobeRGB>
				  (this, ekta, adobe, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSAppleRGB:{
      thread_ = new boost::thread(ThreadCreator<EktaSpacePS5, AppleRGB>
				  (this, ekta, apple, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSBestRGB:{
      thread_ = new boost::thread(ThreadCreator<EktaSpacePS5, BestRGB>
				  (this, ekta, best, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSBetaRGB:{
      thread_ = new boost::thread(ThreadCreator<EktaSpacePS5, BetaRGB>
				  (this, ekta, beta, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSBruceRGB:{
      thread_ = new boost::thread(ThreadCreator<EktaSpacePS5, BruceRGB>
				  (this, ekta, bruce, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSCIERGB:{
      thread_ = new boost::thread(ThreadCreator<EktaSpacePS5, CIERGB>
				  (this, ekta, cie, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSColorMatchRGB:{
      thread_ = new boost::thread(ThreadCreator<EktaSpacePS5, ColorMatchRGB>
				  (this, ekta, colormatch, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSDonRGB4:{
      thread_ = new boost::thread(ThreadCreator<EktaSpacePS5, DonRGB4>
				  (this, ekta, don4, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSECIRGB:{
      thread_ = new boost::thread(ThreadCreator<EktaSpacePS5, ECIRGB>
				  (this, ekta, eci, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSEktaSpacePS5:{
      thread_ = new boost::thread(ThreadCreator<EktaSpacePS5, EktaSpacePS5>
				  (this, ekta, ekta, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSNTSCRGB:{
      thread_ = new boost::thread(ThreadCreator<EktaSpacePS5, NTSCRGB>
				  (this, ekta, ntsc, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSPALSECAMRGB:{
      thread_ = new boost::thread(ThreadCreator<EktaSpacePS5, PAL_SECAMRGB>
				  (this, ekta, pal, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSProPhotoRGB:{
      thread_ = new boost::thread(ThreadCreator<EktaSpacePS5, ProPhotoRGB>
				  (this, ekta, pro, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSSMPTECRGB:{
      thread_ = new boost::thread(ThreadCreator<EktaSpacePS5, SMPTE_CRGB>
				  (this, ekta, smpt, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSsRGB:{
      thread_ = new boost::thread(ThreadCreator<EktaSpacePS5, sRGB>
				  (this, ekta, srgb, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSWideGamutRGB:{
      thread_ = new boost::thread(ThreadCreator<EktaSpacePS5, WideGamutRGB>
				  (this, ekta, widegamut, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    }
    break;
  }


  case CSNTSCRGB:{  // From NTSCRGB to all other colour spaces.
    switch(scs_){
    case CSXYZ:{
      thread_ = new boost::thread(ThreadCreator<NTSCRGB, XYZ>
				  (this, ntsc, xyz, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSxyY:{
      thread_ = new boost::thread(ThreadCreator<NTSCRGB, xyY>
				  (this, ntsc, xyy, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSLab:{
      thread_ = new boost::thread(ThreadCreator<NTSCRGB, Lab>
				  (this, ntsc, lab, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSLCHab:{
      thread_ = new boost::thread(ThreadCreator<NTSCRGB, LCHab>
				  (this, ntsc, lchab, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSLuv:{
      thread_ = new boost::thread(ThreadCreator<NTSCRGB, Luv>
				  (this, ntsc, luv, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSLCHuv:{
      thread_ = new boost::thread(ThreadCreator<NTSCRGB, LCHuv>
				  (this, ntsc, lchuv, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSAdobeRGB:{
      thread_ = new boost::thread(ThreadCreator<NTSCRGB, AdobeRGB>
				  (this, ntsc, adobe, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSAppleRGB:{
      thread_ = new boost::thread(ThreadCreator<NTSCRGB, AppleRGB>
				  (this, ntsc, apple, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSBestRGB:{
      thread_ = new boost::thread(ThreadCreator<NTSCRGB, BestRGB>
				  (this, ntsc, best, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSBetaRGB:{
      thread_ = new boost::thread(ThreadCreator<NTSCRGB, BetaRGB>
				  (this, ntsc, beta, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSBruceRGB:{
      thread_ = new boost::thread(ThreadCreator<NTSCRGB, BruceRGB>
				  (this, ntsc, bruce, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSCIERGB:{
      thread_ = new boost::thread(ThreadCreator<NTSCRGB, CIERGB>
				  (this, ntsc, cie, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSColorMatchRGB:{
      thread_ = new boost::thread(ThreadCreator<NTSCRGB, ColorMatchRGB>
				  (this, ntsc, colormatch, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSDonRGB4:{
      thread_ = new boost::thread(ThreadCreator<NTSCRGB, DonRGB4>
				  (this, ntsc, don4, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSECIRGB:{
      thread_ = new boost::thread(ThreadCreator<NTSCRGB, ECIRGB>
				  (this, ntsc, eci, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSEktaSpacePS5:{
      thread_ = new boost::thread(ThreadCreator<NTSCRGB, EktaSpacePS5>
				  (this, ntsc, ekta, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSNTSCRGB:{
      thread_ = new boost::thread(ThreadCreator<NTSCRGB, NTSCRGB>
				  (this, ntsc, ntsc, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSPALSECAMRGB:{
      thread_ = new boost::thread(ThreadCreator<NTSCRGB, PAL_SECAMRGB>
				  (this, ntsc, pal, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSProPhotoRGB:{
      thread_ = new boost::thread(ThreadCreator<NTSCRGB, ProPhotoRGB>
				  (this, ntsc, pro, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSSMPTECRGB:{
      thread_ = new boost::thread(ThreadCreator<NTSCRGB, SMPTE_CRGB>
				  (this, ntsc, smpt, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSsRGB:{
      thread_ = new boost::thread(ThreadCreator<NTSCRGB, sRGB>
				  (this, ntsc, srgb, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSWideGamutRGB:{
      thread_ = new boost::thread(ThreadCreator<NTSCRGB, WideGamutRGB>
				  (this, ntsc, widegamut, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    }
    break;
  }


  case CSPALSECAMRGB:{  // From PAL_SECAMRGB to all other colour spaces.
    switch(scs_){
    case CSXYZ:{
      thread_ = new boost::thread(ThreadCreator<PAL_SECAMRGB, XYZ>
				  (this, pal, xyz, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSxyY:{
      thread_ = new boost::thread(ThreadCreator<PAL_SECAMRGB, xyY>
				  (this, pal, xyy, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSLab:{
      thread_ = new boost::thread(ThreadCreator<PAL_SECAMRGB, Lab>
				  (this, pal, lab, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSLCHab:{
      thread_ = new boost::thread(ThreadCreator<PAL_SECAMRGB, LCHab>
				  (this, pal, lchab, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSLuv:{
      thread_ = new boost::thread(ThreadCreator<PAL_SECAMRGB, Luv>
				  (this, pal, luv, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSLCHuv:{
      thread_ = new boost::thread(ThreadCreator<PAL_SECAMRGB, LCHuv>
				  (this, pal, lchuv, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSAdobeRGB:{
      thread_ = new boost::thread(ThreadCreator<PAL_SECAMRGB, AdobeRGB>
				  (this, pal, adobe, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSAppleRGB:{
      thread_ = new boost::thread(ThreadCreator<PAL_SECAMRGB, AppleRGB>
				  (this, pal, apple, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSBestRGB:{
      thread_ = new boost::thread(ThreadCreator<PAL_SECAMRGB, BestRGB>
				  (this, pal, best, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSBetaRGB:{
      thread_ = new boost::thread(ThreadCreator<PAL_SECAMRGB, BetaRGB>
				  (this, pal, beta, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSBruceRGB:{
      thread_ = new boost::thread(ThreadCreator<PAL_SECAMRGB, BruceRGB>
				  (this, pal, bruce, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSCIERGB:{
      thread_ = new boost::thread(ThreadCreator<PAL_SECAMRGB, CIERGB>
				  (this, pal, cie, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSColorMatchRGB:{
      thread_ = new boost::thread(ThreadCreator<PAL_SECAMRGB, ColorMatchRGB>
				  (this, pal, colormatch, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSDonRGB4:{
      thread_ = new boost::thread(ThreadCreator<PAL_SECAMRGB, DonRGB4>
				  (this, pal, don4, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSECIRGB:{
      thread_ = new boost::thread(ThreadCreator<PAL_SECAMRGB, ECIRGB>
				  (this, pal, eci, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSEktaSpacePS5:{
      thread_ = new boost::thread(ThreadCreator<PAL_SECAMRGB, EktaSpacePS5>
				  (this, pal, ekta, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSNTSCRGB:{
      thread_ = new boost::thread(ThreadCreator<PAL_SECAMRGB, NTSCRGB>
				  (this, pal, ntsc, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSPALSECAMRGB:{
      thread_ = new boost::thread(ThreadCreator<PAL_SECAMRGB, PAL_SECAMRGB>
				  (this, pal, pal, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSProPhotoRGB:{
      thread_ = new boost::thread(ThreadCreator<PAL_SECAMRGB, ProPhotoRGB>
				  (this, pal, pro, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSSMPTECRGB:{
      thread_ = new boost::thread(ThreadCreator<PAL_SECAMRGB, SMPTE_CRGB>
				  (this, pal, smpt, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSsRGB:{
      thread_ = new boost::thread(ThreadCreator<PAL_SECAMRGB, sRGB>
				  (this, pal, srgb, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSWideGamutRGB:{
      thread_ = new boost::thread(ThreadCreator<PAL_SECAMRGB, WideGamutRGB>
				  (this, pal, widegamut, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    }
    break;
  }


  case CSProPhotoRGB:{  // From ProPhotoRGB to all other colour spaces.
    switch(scs_){
    case CSXYZ:{
      thread_ = new boost::thread(ThreadCreator<ProPhotoRGB, XYZ>
				  (this, pro, xyz, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSxyY:{
      thread_ = new boost::thread(ThreadCreator<ProPhotoRGB, xyY>
				  (this, pro, xyy, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSLab:{
      thread_ = new boost::thread(ThreadCreator<ProPhotoRGB, Lab>
				  (this, pro, lab, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSLCHab:{
      thread_ = new boost::thread(ThreadCreator<ProPhotoRGB, LCHab>
				  (this, pro, lchab, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSLuv:{
      thread_ = new boost::thread(ThreadCreator<ProPhotoRGB, Luv>
				  (this, pro, luv, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSLCHuv:{
      thread_ = new boost::thread(ThreadCreator<ProPhotoRGB, LCHuv>
				  (this, pro, lchuv, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSAdobeRGB:{
      thread_ = new boost::thread(ThreadCreator<ProPhotoRGB, AdobeRGB>
				  (this, pro, adobe, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSAppleRGB:{
      thread_ = new boost::thread(ThreadCreator<ProPhotoRGB, AppleRGB>
				  (this, pro, apple, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSBestRGB:{
      thread_ = new boost::thread(ThreadCreator<ProPhotoRGB, BestRGB>
				  (this, pro, best, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSBetaRGB:{
      thread_ = new boost::thread(ThreadCreator<ProPhotoRGB, BetaRGB>
				  (this, pro, beta, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSBruceRGB:{
      thread_ = new boost::thread(ThreadCreator<ProPhotoRGB, BruceRGB>
				  (this, pro, bruce, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSCIERGB:{
      thread_ = new boost::thread(ThreadCreator<ProPhotoRGB, CIERGB>
				  (this, pro, cie, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSColorMatchRGB:{
      thread_ = new boost::thread(ThreadCreator<ProPhotoRGB, ColorMatchRGB>
				  (this, pro, colormatch, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSDonRGB4:{
      thread_ = new boost::thread(ThreadCreator<ProPhotoRGB, DonRGB4>
				  (this, pro, don4, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSECIRGB:{
      thread_ = new boost::thread(ThreadCreator<ProPhotoRGB, ECIRGB>
				  (this, pro, eci, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSEktaSpacePS5:{
      thread_ = new boost::thread(ThreadCreator<ProPhotoRGB, EktaSpacePS5>
				  (this, pro, ekta, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSNTSCRGB:{
      thread_ = new boost::thread(ThreadCreator<ProPhotoRGB, NTSCRGB>
				  (this, pro, ntsc, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSPALSECAMRGB:{
      thread_ = new boost::thread(ThreadCreator<ProPhotoRGB, PAL_SECAMRGB>
				  (this, pro, pal, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSProPhotoRGB:{
      thread_ = new boost::thread(ThreadCreator<ProPhotoRGB, ProPhotoRGB>
				  (this, pro, pro, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSSMPTECRGB:{
      thread_ = new boost::thread(ThreadCreator<ProPhotoRGB, SMPTE_CRGB>
				  (this, pro, smpt, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSsRGB:{
      thread_ = new boost::thread(ThreadCreator<ProPhotoRGB, sRGB>
				  (this, pro, srgb, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSWideGamutRGB:{
      thread_ = new boost::thread(ThreadCreator<ProPhotoRGB, WideGamutRGB>
				  (this, pro, widegamut, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    }
    break;
  }


  case CSSMPTECRGB:{  // From SMPTE_CRGB to all other colour spaces.
    switch(scs_){
    case CSXYZ:{
      thread_ = new boost::thread(ThreadCreator<SMPTE_CRGB, XYZ>
				  (this, smpt, xyz, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSxyY:{
      thread_ = new boost::thread(ThreadCreator<SMPTE_CRGB, xyY>
				  (this, smpt, xyy, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSLab:{
      thread_ = new boost::thread(ThreadCreator<SMPTE_CRGB, Lab>
				  (this, smpt, lab, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSLCHab:{
      thread_ = new boost::thread(ThreadCreator<SMPTE_CRGB, LCHab>
				  (this, smpt, lchab, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSLuv:{
      thread_ = new boost::thread(ThreadCreator<SMPTE_CRGB, Luv>
				  (this, smpt, luv, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSLCHuv:{
      thread_ = new boost::thread(ThreadCreator<SMPTE_CRGB, LCHuv>
				  (this, smpt, lchuv, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSAdobeRGB:{
      thread_ = new boost::thread(ThreadCreator<SMPTE_CRGB, AdobeRGB>
				  (this, smpt, adobe, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSAppleRGB:{
      thread_ = new boost::thread(ThreadCreator<SMPTE_CRGB, AppleRGB>
				  (this, smpt, apple, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSBestRGB:{
      thread_ = new boost::thread(ThreadCreator<SMPTE_CRGB, BestRGB>
				  (this, smpt, best, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSBetaRGB:{
      thread_ = new boost::thread(ThreadCreator<SMPTE_CRGB, BetaRGB>
				  (this, smpt, beta, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSBruceRGB:{
      thread_ = new boost::thread(ThreadCreator<SMPTE_CRGB, BruceRGB>
				  (this, smpt, bruce, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSCIERGB:{
      thread_ = new boost::thread(ThreadCreator<SMPTE_CRGB, CIERGB>
				  (this, smpt, cie, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSColorMatchRGB:{
      thread_ = new boost::thread(ThreadCreator<SMPTE_CRGB, ColorMatchRGB>
				  (this, smpt, colormatch, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSDonRGB4:{
      thread_ = new boost::thread(ThreadCreator<SMPTE_CRGB, DonRGB4>
				  (this, smpt, don4, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSECIRGB:{
      thread_ = new boost::thread(ThreadCreator<SMPTE_CRGB, ECIRGB>
				  (this, smpt, eci, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSEktaSpacePS5:{
      thread_ = new boost::thread(ThreadCreator<SMPTE_CRGB, EktaSpacePS5>
				  (this, smpt, ekta, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSNTSCRGB:{
      thread_ = new boost::thread(ThreadCreator<SMPTE_CRGB, NTSCRGB>
				  (this, smpt, ntsc, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSPALSECAMRGB:{
      thread_ = new boost::thread(ThreadCreator<SMPTE_CRGB, PAL_SECAMRGB>
				  (this, smpt, pal, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSProPhotoRGB:{
      thread_ = new boost::thread(ThreadCreator<SMPTE_CRGB, ProPhotoRGB>
				  (this, smpt, pro, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSSMPTECRGB:{
      thread_ = new boost::thread(ThreadCreator<SMPTE_CRGB, SMPTE_CRGB>
				  (this, smpt, smpt, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSsRGB:{
      thread_ = new boost::thread(ThreadCreator<SMPTE_CRGB, sRGB>
				  (this, smpt, srgb, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSWideGamutRGB:{
      thread_ = new boost::thread(ThreadCreator<SMPTE_CRGB, WideGamutRGB>
				  (this, smpt, widegamut, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    }
    break;
  }


  case CSsRGB:{  // From sRGB to all other colour spaces.
    switch(scs_){
    case CSXYZ:{
      thread_ = new boost::thread(ThreadCreator<sRGB, XYZ>
				  (this, srgb, xyz, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSxyY:{
      thread_ = new boost::thread(ThreadCreator<sRGB, xyY>
				  (this, srgb, xyy, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSLab:{
      thread_ = new boost::thread(ThreadCreator<sRGB, Lab>
				  (this, srgb, lab, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSLCHab:{
      thread_ = new boost::thread(ThreadCreator<sRGB, LCHab>
				  (this, srgb, lchab, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSLuv:{
      thread_ = new boost::thread(ThreadCreator<sRGB, Luv>
				  (this, srgb, luv, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSLCHuv:{
      thread_ = new boost::thread(ThreadCreator<sRGB, LCHuv>
				  (this, srgb, lchuv, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSAdobeRGB:{
      thread_ = new boost::thread(ThreadCreator<sRGB, AdobeRGB>
				  (this, srgb, adobe, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSAppleRGB:{
      thread_ = new boost::thread(ThreadCreator<sRGB, AppleRGB>
				  (this, srgb, apple, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSBestRGB:{
      thread_ = new boost::thread(ThreadCreator<sRGB, BestRGB>
				  (this, srgb, best, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSBetaRGB:{
      thread_ = new boost::thread(ThreadCreator<sRGB, BetaRGB>
				  (this, srgb, beta, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSBruceRGB:{
      thread_ = new boost::thread(ThreadCreator<sRGB, BruceRGB>
				  (this, srgb, bruce, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSCIERGB:{
      thread_ = new boost::thread(ThreadCreator<sRGB, CIERGB>
				  (this, srgb, cie, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSColorMatchRGB:{
      thread_ = new boost::thread(ThreadCreator<sRGB, ColorMatchRGB>
				  (this, srgb, colormatch, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSDonRGB4:{
      thread_ = new boost::thread(ThreadCreator<sRGB, DonRGB4>
				  (this, srgb, don4, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSECIRGB:{
      thread_ = new boost::thread(ThreadCreator<sRGB, ECIRGB>
				  (this, srgb, eci, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSEktaSpacePS5:{
      thread_ = new boost::thread(ThreadCreator<sRGB, EktaSpacePS5>
				  (this, srgb, ekta, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSNTSCRGB:{
      thread_ = new boost::thread(ThreadCreator<sRGB, NTSCRGB>
				  (this, srgb, ntsc, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSPALSECAMRGB:{
      thread_ = new boost::thread(ThreadCreator<sRGB, PAL_SECAMRGB>
				  (this, srgb, pal, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSProPhotoRGB:{
      thread_ = new boost::thread(ThreadCreator<sRGB, ProPhotoRGB>
				  (this, srgb, pro, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSSMPTECRGB:{
      thread_ = new boost::thread(ThreadCreator<sRGB, SMPTE_CRGB>
				  (this, srgb, smpt, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSsRGB:{
      thread_ = new boost::thread(ThreadCreator<sRGB, sRGB>
				  (this, srgb, srgb, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSWideGamutRGB:{
      thread_ = new boost::thread(ThreadCreator<sRGB, WideGamutRGB>
				  (this, srgb, widegamut, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    }
    break;
  }


  case CSWideGamutRGB:{  // From WideGamutRGB to all other colour spaces.
    switch(scs_){
    case CSXYZ:{
      thread_ = new boost::thread(ThreadCreator<WideGamutRGB, XYZ>
				  (this, widegamut, xyz, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSxyY:{
      thread_ = new boost::thread(ThreadCreator<WideGamutRGB, xyY>
				  (this, widegamut, xyy, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSLab:{
      thread_ = new boost::thread(ThreadCreator<WideGamutRGB, Lab>
				  (this, widegamut, lab, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSLCHab:{
      thread_ = new boost::thread(ThreadCreator<WideGamutRGB, LCHab>
				  (this, widegamut, lchab, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSLuv:{
      thread_ = new boost::thread(ThreadCreator<WideGamutRGB, Luv>
				  (this, widegamut, luv, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSLCHuv:{
      thread_ = new boost::thread(ThreadCreator<WideGamutRGB, LCHuv>
				  (this, widegamut, lchuv, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSAdobeRGB:{
      thread_ = new boost::thread(ThreadCreator<WideGamutRGB, AdobeRGB>
				  (this, widegamut, adobe, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSAppleRGB:{
      thread_ = new boost::thread(ThreadCreator<WideGamutRGB, AppleRGB>
				  (this, widegamut, apple, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSBestRGB:{
      thread_ = new boost::thread(ThreadCreator<WideGamutRGB, BestRGB>
				  (this, widegamut, best, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSBetaRGB:{
      thread_ = new boost::thread(ThreadCreator<WideGamutRGB, BetaRGB>
				  (this, widegamut, beta, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSBruceRGB:{
      thread_ = new boost::thread(ThreadCreator<WideGamutRGB, BruceRGB>
				  (this, widegamut, bruce, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSCIERGB:{
      thread_ = new boost::thread(ThreadCreator<WideGamutRGB, CIERGB>
				  (this, widegamut, cie, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSColorMatchRGB:{
      thread_ = new boost::thread(ThreadCreator<WideGamutRGB, ColorMatchRGB>
				  (this, widegamut, colormatch, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSDonRGB4:{
      thread_ = new boost::thread(ThreadCreator<WideGamutRGB, DonRGB4>
				  (this, widegamut, don4, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSECIRGB:{
      thread_ = new boost::thread(ThreadCreator<WideGamutRGB, ECIRGB>
				  (this, widegamut, eci, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSEktaSpacePS5:{
      thread_ = new boost::thread(ThreadCreator<WideGamutRGB, EktaSpacePS5>
				  (this, widegamut, ekta, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSNTSCRGB:{
      thread_ = new boost::thread(ThreadCreator<WideGamutRGB, NTSCRGB>
				  (this, widegamut, ntsc, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSPALSECAMRGB:{
      thread_ = new boost::thread(ThreadCreator<WideGamutRGB, PAL_SECAMRGB>
				  (this, widegamut, pal, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSProPhotoRGB:{
      thread_ = new boost::thread(ThreadCreator<WideGamutRGB, ProPhotoRGB>
				  (this, widegamut, pro, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSSMPTECRGB:{
      thread_ = new boost::thread(ThreadCreator<WideGamutRGB, SMPTE_CRGB>
				  (this, widegamut, smpt, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSsRGB:{
      thread_ = new boost::thread(ThreadCreator<WideGamutRGB, sRGB>
				  (this, widegamut, srgb, camera_,
				   imageWidth_, imageHeight_));
      break;
    }
    case CSWideGamutRGB:{
      thread_ = new boost::thread(ThreadCreator<WideGamutRGB, WideGamutRGB>
				  (this, widegamut, widegamut, camera_,
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












