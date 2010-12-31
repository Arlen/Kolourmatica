/***********************************************************************
|*  Copyright (C) 2010 Arlen Avakian
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


#include "Render.hpp"

#include <QtGui/QGraphicsSceneResizeEvent>
#include <QtGui/QPainter>
#include <QtGui/QStyleOptionGraphicsItem>
#include <QtGui/QGraphicsLineItem>
#include <QtGui/QGraphicsScene>
#include <QtGui/QGraphicsSceneMouseEvent>
#include <QtCore/QRectF>

#include <iostream>
using namespace std;


Render::Render(){

  resizeHandleWidth_ = 2.0;
  resizeHandleLength_ = 32.0;
  resizeHandleGap_ = 8.0;
  resizeFlags_ = 0;
  showResizeHandle_ = false;
  setFlag(QGraphicsItem::ItemIsMovable, true);
  setFlag(QGraphicsItem::ItemIsSelectable, true);
  setAcceptHoverEvents(true);
  resize(512, 512);
}

void Render::paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
		   QWidget* widget){

  painter->fillRect(0, 0, size().width(), size().height(), Qt::black);

  if( showResizeHandle_ )
    paintResizeHandle(resizeHandleLength_, resizeHandleGap_,
		      resizeHandleWidth_, painter);
}

void Render::resizeEvent(QGraphicsSceneResizeEvent* event){

}

void Render::mousePressEvent(QGraphicsSceneMouseEvent* event){

  resizeFlags_ = 0;
  qreal s = resizeHandleWidth_ * 4.0;
  if(!rect().adjusted(s, s, -s, -s).contains(event->pos())){
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
  }
  QGraphicsWidget::mousePressEvent(event);
}

void Render::mouseMoveEvent(QGraphicsSceneMouseEvent* event){

  if(resizeFlags_){
    setFlag(QGraphicsItem::ItemIsMovable, false);
    QRectF rec = rect();
    QPointF p = event->pos();
    QPointF uniform1 = QPointF(qMax(p.x(), p.y()), qMax(p.x(), p.y()));

    if(resizeFlags_ == (Top    | Left )) rec.setTopLeft(uniform1);
    if(resizeFlags_ == (Top           )) rec.setTop(p.y());
    if(resizeFlags_ == (Top    | Right)) rec.setTopRight(p);
    if(resizeFlags_ == (         Left )) rec.setLeft(p.x());
    if(resizeFlags_ == (         Right)) rec.setRight(p.x());
    if(resizeFlags_ == (Bottom | Left )) rec.setBottomLeft(p);
    if(resizeFlags_ == (Bottom        )) rec.setBottom(p.y());
    if(resizeFlags_ == (Bottom | Right)) rec.setBottomRight(uniform1);
    setGeometry(mapToScene(rec).boundingRect());
  }
  QGraphicsWidget::mouseMoveEvent(event);
  setFlag(QGraphicsItem::ItemIsMovable, true);
}

void Render::mouseReleaseEvent(QGraphicsSceneMouseEvent* event){

  QGraphicsWidget::mouseReleaseEvent(event);
}

void Render::hoverEnterEvent(QGraphicsSceneHoverEvent* event){

}

void Render::hoverLeaveEvent(QGraphicsSceneHoverEvent* event){

  if( showResizeHandle_ ){
    showResizeHandle_ = false;
    update();
  }
}

void Render::hoverMoveEvent(QGraphicsSceneHoverEvent* event){

  qreal s = resizeHandleWidth_ * 4.0;
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
 void Render::paintResizeHandle(qreal len, qreal gap, qreal width,
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
  /* top left end */
  painter->drawLine(tl.x(), tl.y(), tl.x() + len, tl.y());
  /* top right end */
  painter->drawLine(hp2, tr.y(), tr.x(), tr.y());

  /* bottom left end*/
  painter->drawLine(bl.x(), bl.y(), bl.x() + len, bl.y());
  /* bottom right end */
  painter->drawLine(hp2, br.y(), br.x(), br.y());

  /* left top end */
  painter->drawLine(tl.x(), tl.y(), tl.x(), tl.y() + len);
  /* left bottom end */
  painter->drawLine(tl.x(), vp2, tl.x(), bl.y());

  /* right top end */
  painter->drawLine(tr.x(), tr.y(), tr.x(), tr.y() + len);
  /* right bottom end */
  painter->drawLine(tr.x(), vp2, tr.x(), br.y());

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
