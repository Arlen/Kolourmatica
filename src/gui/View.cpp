/***********************************************************************
|*  Copyright (C) 2011 Arlen Avakian
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


#include "View.hpp"
#include "ChromaticAdaptation.hpp"

#include <QtGui/QGraphicsSceneResizeEvent>
#include <QtGui/QGraphicsSceneMouseEvent>
#include <QtGui/QStyleOptionGraphicsItem>
#include <QtGui/QPainter>
#include <QtCore/QRectF>

#include <list>


View::View(Side side){

  _fromIndex = 0;
  _toIndex = 1;
  _srwIndex = 0;
  _drwIndex = 0;
  _srcObsIndex = 0;
  _dstObsIndex = 0;
  _camIndex = 2;

  _resizeHandleThickness = 2.0;
  _resizeHandleLength = 32.0;
  _resizeHandleGap = 8.0;
  _region = Region::None;
  _side = side;
  _showResizeHandle = false;
  _dirty = true;
  _showImage = true;
  setFlag(QGraphicsItem::ItemIsMovable, true);
  setFlag(QGraphicsItem::ItemIsSelectable, true);
  setAcceptHoverEvents(true);
  _timer.setSingleShot(true);
  connect(&_timer, SIGNAL(timeout()), this, SLOT(updateView()));

  _activeBuffer = nullptr;
  setMinimumSize(128, 128);
  resetView(512.0, 512.0);
  setupCamera(_fromIndex);
  setCursor(Qt::ArrowCursor);

  connect(this, SIGNAL(visibleChanged()), this, SLOT(startStopRendering()));
  connect(this, SIGNAL(viewSizeChanged()), this, SLOT(setViewSize()));
  connect(this, SIGNAL(renderingStarted()), this, SLOT(render()));
}

void View::paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
		 QWidget* widget){

  painter->fillRect(0, 0, size().width(), size().height(), Qt::black);

  if( _showResizeHandle or _timer.isActive() )
    paintResizeHandle(_resizeHandleLength, _resizeHandleGap,
		      _resizeHandleThickness, painter);

  if( _showImage and !_dirty )
    painter->drawImage(QPoint(_resizeHandleThickness, _resizeHandleThickness),
		       _renderedImage.mirrored());
}

void View::setFrom(int index){

  _fromIndex = index;
  _dirty = true;
  resetView(512, 512);
  setupCamera(_fromIndex);
  restartRendering();
}

void View::setTo(int index){
  
  _toIndex = index; _dirty = true;
  restartRendering();
}

void View::setSrcRefWhite(int index){

  _srwIndex = index * 2; _dirty = true;
  restartRendering();
}

void View::setDstRefWhite(int index){

  _drwIndex = index * 2; _dirty = true;
  restartRendering();
}

void View::setSrcObserver(int index){

  _srcObsIndex = index; _dirty = true;
  restartRendering();
}

void View::setDstObserver(int index){

  _dstObsIndex = index; _dirty = true;
  restartRendering();
}

void View::setAdaptationMethod(int index){

  _camIndex = index; _dirty = true;
  restartRendering();
}

void View::mousePressEvent(QGraphicsSceneMouseEvent* event){

  if(event->button() & Qt::LeftButton){

    _region = Region::None;
    qreal s = _resizeHandleThickness * 3.0;
    if(!rect().adjusted(s, s, -s, -s).contains(event->pos())){

      emit renderingStopped();
      _showImage = false;
      _resizeHandleOffsetX =
	rect().width() - (abs(rect().center().x() - event->pos().x()) * 2.0);
      _resizeHandleOffsetY =
	rect().height() - (abs(rect().center().y() - event->pos().y()) * 2.0);
      _resizeHandleOffsetX *= 0.5;
      _resizeHandleOffsetY *= 0.5;

      qreal d = _resizeHandleGap + _resizeHandleLength;
      qreal hd = (rect().width() - (d * 2.0)) + _resizeHandleGap;
      qreal vd = (rect().height() - (d * 2.0)) + _resizeHandleGap;

      QRectF r11 = rect().adjusted(     0,      0, -(hd + d), -(vd + d));
      QRectF r12 = rect().adjusted(     d,      0,        -d, -(vd + d));
      QRectF r13 = rect().adjusted(hd + d,      0,         0, -(vd + d));
      QRectF r21 = rect().adjusted(     0,      d, -(hd + d),        -d);
      QRectF r23 = rect().adjusted(hd + d,      d,         0,        -d);
      QRectF r31 = rect().adjusted(     0, d + vd, -(d + hd),         0);
      QRectF r32 = rect().adjusted(     d, d + vd,        -d,         0);
      QRectF r33 = rect().adjusted(d + hd, d + vd,         0,         0);

      if(r11.contains(event->pos())) _region = Region::TopLeft;
      else if(r12.contains(event->pos())) _region = Region::Top;
      else if(r13.contains(event->pos())) _region = Region::TopRight;
      else if(r21.contains(event->pos())) _region = Region::Left;
      else if(r23.contains(event->pos())) _region = Region::Right;
      else if(r31.contains(event->pos())) _region = Region::BottomLeft;
      else if(r32.contains(event->pos())) _region = Region::Bottom;
      else if(r33.contains(event->pos())) _region = Region::BottomRight;
    }
  }
  QGraphicsWidget::mousePressEvent(event);
}

void View::mouseMoveEvent(QGraphicsSceneMouseEvent* event){

  if(event->buttons() & Qt::LeftButton and _region != Region::None){
    
    setFlag(QGraphicsItem::ItemIsMovable, false);
    QRectF rec = rect();
    QPointF offset(_resizeHandleOffsetX, _resizeHandleOffsetY);
    QPointF d = event->pos() - event->lastPos();
    qreal max = abs(d.x()) > abs(d.y()) ? d.x() : d.y();

    QPointF nonUniform = event->pos();
    QPointF uniform = QPointF(max, max);

    switch(_region){
    case Region::TopLeft:
      rec.setTopLeft(rec.topLeft() + uniform); break;
    case Region::Top:
      rec.setTop(nonUniform.y() - _resizeHandleOffsetY); break;
    case Region::TopRight:
      rec.setTopRight(nonUniform + QPoint(_resizeHandleOffsetX,
					  -_resizeHandleOffsetY)); break;
    case Region::Left:
      rec.setLeft(nonUniform.x() - _resizeHandleOffsetX); break;
    case Region::Right:
      rec.setRight(nonUniform.x() + _resizeHandleOffsetX); break;
    case Region::BottomLeft:
      rec.setBottomLeft(nonUniform + QPoint(-_resizeHandleOffsetX,
					    _resizeHandleOffsetY)); break;
    case Region::Bottom:
      rec.setBottom(nonUniform.y() + _resizeHandleOffsetY); break;
    case Region::BottomRight:
      rec.setBottomRight(rec.bottomRight() + uniform); break;
    case Region::None: break;
    }
    setGeometry(mapToScene(rec).boundingRect());
  }

  QGraphicsWidget::mouseMoveEvent(event);
  if( !(flags() & QGraphicsItem::ItemIsMovable) )
    setFlag(QGraphicsItem::ItemIsMovable, true);
}

void View::mouseReleaseEvent(QGraphicsSceneMouseEvent* event){

  if(event->button() == Qt::LeftButton && _region != Region::None){
    emit viewSizeChanged();
  }
  QGraphicsWidget::mouseReleaseEvent(event);
}

void View::hoverLeaveEvent(QGraphicsSceneHoverEvent* event){

  if( _showResizeHandle ){
    _showResizeHandle = false;
    _timer.start(500);
  }
}

void View::hoverMoveEvent(QGraphicsSceneHoverEvent* event){

  qreal s = _resizeHandleThickness * 3.0;
  if(!rect().adjusted(s, s, -s, -s).contains(event->pos())){
    if( !_showResizeHandle ){
      _showResizeHandle = true;
      _timer.start(500);
      update();
    }
  }else{
    if( _showResizeHandle ){
      _showResizeHandle = false;
      _timer.start(500);
      update();
    }
  }
  QGraphicsWidget::hoverMoveEvent(event);
}

int View::idealThreadCount() const{

  int threadCount = QThread::idealThreadCount();
  threadCount = threadCount == -1 ? 1 : threadCount;
  if(_imageHeight < threadCount)
    threadCount = _imageHeight;
  return threadCount;
}

void View::resetView(qreal w, qreal h){

  qreal offset = _resizeHandleThickness * 2.0;
  resize(w + offset, h + offset);
  _imageWidth = static_cast<unsigned>(qRound(w));
  _imageHeight = static_cast<unsigned>(qRound(h));
}

void View::setupCamera(int fromIndex){

  switch(fromIndex){
  case 0:{
    if( _side == Side::Front ){
      _camera = {{0.0, 1.0, 1.0, 0.0, 1.0, 0.0, -0.001}, 0, 1, 2, 3, 4, 5, 6, 0, 1, 2};
      _camera._pred = [&_camera](const double in){ return in >= _camera.back(); };
    }else if( _side == Side::Left ){
      _camera = {{0.0, 1.0, 1.0, 0.0, 1.0, 0.0, 0.001}, 0, 1, 2, 3, 5, 4, 6, 2, 1, 0};
      _camera._pred = [&_camera](const double in){ return in <= _camera.back(); };
    }else if( _side == Side::Right ){
      _camera = {{0.0, 1.0, 1.0, 0.0, 1.0, 0.0, -0.001}, 1, 0, 2, 3, 4, 5, 6, 2, 1, 0};
      _camera._pred = [&_camera](const double in){ return in >= _camera.back(); };
    }else if( _side == Side::Top ){
      _camera = {{0.0, 1.0, 1.0, 0.0, 1.0, 0.0, -0.001}, 0, 1, 3, 2, 4, 5, 6, 0, 2, 1};
      _camera._pred = [&_camera](const double in){ return in >= _camera.back(); };
    }else if( _side == Side::Bottom ){
      _camera = {{0.0, 1.0, 1.0, 0.0, 1.0, 0.0, 0.001}, 0, 1, 2, 3, 5, 4, 6, 0, 2, 1};
      _camera._pred = [&_camera](const double in){ return in <= _camera.back(); };
    }
  } break;

  case 1:{
    if( _side == Side::Front ){
      _camera = {{0.0, 1.0, 1.0, 0.0, 1.0, 0.0, -0.5}, 0, 1, 2, 3, 4, 5, 6, 0, 1, 2};
      _camera._pred = [&_camera](const double in){ return in >= _camera.back(); };
    }else if( _side == Side::Left ){
      _camera = {{0.0, 1.0, 1.0, 0.0, 1.0, -0.5, 0.0005}, 0, 1, 2, 3, 5, 4, 6, 2, 1, 0};
      _camera._pred = [&_camera](const double in){ return in <= _camera.back(); };
    }else if( _side == Side::Right ){
      _camera = {{0.0, 1.0, 1.0, 0.0, 1.0, 0.0, -0.0005}, 1, 0, 2, 3, 4, 5, 6, 2, 1, 0};
      _camera._pred = [&_camera](const double in){ return in >= _camera.back(); };
    }else if( _side == Side::Top ){
      _camera = {{0.0, 1.0, 1.0, 0.0, 1.0, 0.0, -0.0025}, 0, 1, 3, 2, 4, 5, 6, 0, 2, 1};
      _camera._pred = [&_camera](const double in){ return in >= _camera.back(); };
    }else if( _side == Side::Bottom ){
      _camera = {{0.0, 1.0, 1.0, 0.0, 1.0, 0.0, 0.0005}, 0, 1, 2, 3, 5, 4, 6, 0, 2, 1};
      _camera._pred = [&_camera](const double in){ return in <= _camera.back(); };
    }
  } break;

  case 2:
  case 3:
  case 4:
  case 5:{
    if( _side == Side::Front ){
      _camera = {{-200.0, 200.0, 200.0, -200.0, 100.0, 0.0, -0.5}, 0, 1, 2, 3, 4, 5, 6, 1, 2, 0};
      _camera._pred = [&_camera](const double in){ return in >= _camera.back(); };
      break;
    }else if( _side == Side::Left ){
      _camera = {{-200.0, 200.0, 200.0, -200.0, 100.0, 0.0, 0.5}, 5, 4, 2, 3, 0, 1, 6, 0, 2, 1};
      _camera._pred = [&_camera](const double in){ return in <= _camera.back(); };
      break;
    }else if( _side == Side::Right ){
      _camera = {{-200.0, 200.0, 200.0, -200.0, 100.0, 0.0, -0.5}, 4, 5, 2, 3, 1, 0, 6, 0, 2, 1};
      _camera._pred = [&_camera](const double in){ return in >= _camera.back(); };
      break;
    }else if( _side == Side::Top ){
      _camera = {{-200.0, 200.0, 200.0, -200.0, 100.0, 0.0, -0.5}, 0, 1, 5, 4, 2, 3, 6, 1, 0, 2};
      _camera._pred = [&_camera](const double in){ return in >= _camera.back(); };
      break;
    }else if( _side == Side::Bottom ){
      _camera = {{-200.0, 200.0, 200.0, -200.0, 100.0, 0.0, 0.5}, 0, 1, 4, 5, 3, 2, 6, 1, 0, 2};
      _camera._pred = [&_camera](const double in){ return in <= _camera.back(); };
      break;
    }
  } break;
  }
}

void View::paintResizeHandle(qreal len, qreal gap, qreal width,
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

void View::restartRendering(){

  if(isVisible()){
    if(_threads.isEmpty())
      emit renderingStarted();
    else
      emit renderingStopped();
  }
}

void View::updateView(){ update(); }

void View::saveToImage(){

  _renderedImage = QImage(_activeBuffer, _imageWidth, _imageHeight,
			  QImage::Format_RGB32).copy();
  update();
  emit saved();
}

void View::startStopRendering(){

  if(isVisible()) emit renderingStarted();
  else emit renderingStopped();
}

void View::render(){

  _dirty = false;
  std::list<QThread*> threads;

  int srwIndex = _srwIndex + _srcObsIndex;
  int drwIndex = _drwIndex + _dstObsIndex;
  int idealHeight = _imageHeight / idealThreadCount();
  _activeBuffer = new uchar[4 * _imageWidth * _imageHeight];
  memset(_activeBuffer, 0, 4 * _imageWidth * _imageHeight);

  for( int y = 0; y < _imageHeight; ){

    Renderer* rn = new Renderer(_activeBuffer, _camera, _imageHeight,
				_fromIndex, _toIndex, _camIndex, y, _imageWidth,
				idealHeight, srwIndex, drwIndex);
    Thread* thread = new Thread;
    _threads.push(true);
    threads.push_back(thread);
    rn->moveToThread(thread);

    connect(thread, SIGNAL(started()), rn, SLOT(dispatch()));
    connect(thread, SIGNAL(expired(QThread*)), this, SLOT(deleteThread(QThread*)));
    connect(rn, SIGNAL(destroyed()), thread, SLOT(quit()));
    connect(rn, SIGNAL(rendered()), this, SLOT(saveToImage()));
    connect(this, SIGNAL(saved()), rn, SLOT(dispatch()));
    connect(this, SIGNAL(renderingStopped()), rn, SLOT(abort()));

    y += idealHeight;
    if((_imageHeight - y) < idealHeight)
      idealHeight = _imageHeight - y;
  }
  for(auto thread : threads) thread->start();
}

void View::setViewSize(){

  QRectF rec = rect();
  Real offset = _resizeHandleThickness * 2.0;
  Real newWidthF = rec.width() - offset;
  Real newHeightF = rec.height() - offset;
  Real oldWidthF = static_cast<Real>(_imageWidth);
  Real oldHeightF = static_cast<Real>(_imageHeight);

  Real newTop = (_camera.top() * newHeightF) / oldHeightF;
  Real newBottom = (_camera.bottom() * newHeightF) / oldHeightF;
  Real newRight = (_camera.right() * newWidthF) / oldWidthF;
  Real newLeft  = (_camera.left() * newWidthF) / oldWidthF;
  Real newHDist = newRight - newLeft;
  Real oldHDist = _camera.right() - _camera.left();
  Real hOffset = newHDist - oldHDist;
  Real newVDist = newTop - newBottom;
  Real oldVDist = _camera.top() - _camera.bottom();
  Real vOffset = newVDist - oldVDist;

  if(_region == Region::TopLeft){
    _camera.top() = _camera.top() + vOffset;
    _camera.left() = _camera.left() - hOffset;

  }else if(_region == Region::Top){
    _camera.top() = _camera.top() + vOffset;

  }else if(_region == Region::TopRight){
    _camera.top() = _camera.top() + vOffset;
    _camera.right() = _camera.right() + hOffset;

  }else if(_region == Region::Left){
    _camera.left() = _camera.left() - hOffset;

  }else if(_region == Region::Right){
    _camera.right() = _camera.right() + hOffset;

  }else if(_region == Region::BottomLeft){
    _camera.bottom() = _camera.bottom() - vOffset;
    _camera.left() = _camera.left() - hOffset;

  }else if(_region == Region::Bottom){
    _camera.bottom() = _camera.bottom() - vOffset;
 
  }else if(_region == Region::BottomRight){
    _camera.bottom() = _camera.bottom() - vOffset;
    _camera.right() = _camera.right() + hOffset;
  }

  _imageWidth = static_cast<unsigned>(qRound(newWidthF));
  _imageHeight = static_cast<unsigned>(qRound(newHeightF));
  _showImage = true;
  _dirty = true;

  if(_threads.isEmpty()) emit renderingStarted();
}

void View::deleteThread(QThread* thread){

  delete thread;
  _threads.pop();

  if(_threads.isEmpty()){
    delete[] _activeBuffer;
    if(_dirty)
      emit renderingStarted();
  }
}


/*  T H R E A D  */
Thread::Thread(){ connect(this, SIGNAL(finished()), this, SLOT(deleteMe())); }

void Thread::deleteMe(){ emit expired(this); }


/*  R E N D E R E R  */
Renderer::Renderer(uchar* buffer, Camera camera, int imageHeight,
		   int fromIndex, int toIndex, int camIndex, int y, int width, int height, int srwIndex, int drwIndex) :
  _buffer(buffer), _camera(camera), _srw(nullptr), _drw(nullptr),
  _imageHeight(imageHeight),  _fromIndex(fromIndex), _toIndex(toIndex),
  _camIndex(camIndex), _oldY(y), _y(y), _width(width), _height(height){

  _cs = {
    new XYZ, new xyY, new Luv, new LCHuv, new Lab, new LCHab, new AdobeRGB,
    new AppleRGB, new BestRGB, new BetaRGB, new BruceRGB, new CIERGB,
    new ColorMatchRGB, new DonRGB4, new ECIRGB, new EktaSpacePS5, new NTSCRGB,
    new PAL_SECAMRGB, new ProPhotoRGB, new SMPTE_CRGB, new sRGB,
    new WideGamutRGB
  };

  _rw = {
    new A_1931_2, new A_1964_10, new B_1931_2, new B_1964_10, new C_1931_2,
    new C_1964_10, new D50_1931_2, new D50_1964_10, new D55_1931_2,
    new D55_1964_10, new D65_1931_2, new D65_1964_10, new D75_1931_2,
    new D75_1964_10, new E_1931_2, new E_1964_10, new F1_1931_2, new F1_1964_10,
    new F2_1931_2, new F2_1964_10, new F3_1931_2, new F3_1964_10, new F4_1931_2,
    new F4_1964_10, new F5_1931_2, new F5_1964_10, new F6_1931_2,
    new F6_1964_10, new F7_1931_2, new F7_1964_10, new F8_1931_2,
    new F8_1964_10, new F9_1931_2, new F9_1964_10, new F10_1931_2,
    new F10_1964_10, new F11_1931_2, new F11_1964_10, new F12_1931_2,
    new F12_1964_10
  };

  vector<Matrix3> am = {
    AdaptationMethod<Real>::_XYZScaling,
    AdaptationMethod<Real>::_VonKries,
    AdaptationMethod<Real>::_Bradford
  };

  if(_fromIndex >= 0 and _fromIndex <= 5)
    _srw = _rw[srwIndex];
  else
    _cs[_fromIndex]->referenceWhite(_srw);


  if(_toIndex >= 0 and _toIndex <= 5)
    _drw = _rw[drwIndex];
  else
    _cs[_toIndex]->referenceWhite(_drw);

  _cam = chromaticAdaptationMatrix(*_srw, *_drw, am[_camIndex]);
  _abort = false;


  connect(this, SIGNAL(dispatched(int, int, int, int)),
	  this, SLOT(render(int, int, int, int)));
  connect(this, SIGNAL(finished()), this, SLOT(deleteLater()));
}

Renderer::~Renderer(){

  for(BaseColourSpace* cs : _cs)
    delete cs;

  for(Illuminant* rw : _rw)
    delete rw;
}

void Renderer::dispatch(){

  if((_y - _oldY) == _height or _abort){
    emit finished();
    return;
  }

  emit dispatched(_oldY, _y, _width, _height);
  ++_y;
}

void Renderer::render(int oldY, int y, int width, int height){

  Vector3 out;
  Vector3 in;
  XYZ xyz;
  double delta = _camera.rayDir();
  int si = y * width;

  double yd = (double)y;
  double vDist = _camera.top() - _camera.bottom();
  double hDist = _camera.right() - _camera.left();

  in(_camera._y) = ((yd * vDist) / _imageHeight) + _camera.bottom();

  for(int j = 0; j < width; ++j){

    in(_camera._x) = ((static_cast<double>(j) * hDist) / width) + _camera.left();
    in(_camera._z) = _camera.front();
    double d = delta;
    int ep = 0;
    
    do{
      _cs[_fromIndex]->coords() = in;
      xyz.coords() = _cs[_fromIndex]->to_XYZ(_srw);
      xyz.coords() = _cam * xyz.coords();
      _cs[_toIndex]->from_XYZ(xyz.coords(), _drw);
      out = _cs[_toIndex]->coords();

      if( (out.array() <= 1).all() and (out.array() >= 0).all() ){
	if(ep == 8) break;
      	in(_camera._z) -= d;
      	d *= 0.5;
	++ep;
      }else{
      	in(_camera._z) += d;
      }
    }while(_camera._pred( in(_camera._z) ));

    if( (out.array() < 0).any() or (out.array() > 1).any() )
      out << 0, 0, 0;

    _buffer[4 * (si + j)    ] = static_cast<uchar>(255.0 * out(2));
    _buffer[4 * (si + j) + 1] = static_cast<uchar>(255.0 * out(1));
    _buffer[4 * (si + j) + 2] = static_cast<uchar>(255.0 * out(0));
  }
  emit rendered();
}

void Renderer::abort(){ _abort = true; }
