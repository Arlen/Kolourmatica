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


#ifndef RENDER_HPP
#define RENDER_HPP


#include <QtGui/QGraphicsWidget>


class Render : public QGraphicsWidget{


public:
  Render();

  void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);

protected:
  void resizeEvent(QGraphicsSceneResizeEvent* event);
  void mousePressEvent(QGraphicsSceneMouseEvent* event);
  void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
  void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);

  void hoverEnterEvent(QGraphicsSceneHoverEvent* event);
  void hoverLeaveEvent(QGraphicsSceneHoverEvent* event);
  void hoverMoveEvent (QGraphicsSceneHoverEvent* event);


private:
  void paintResizeHandle(qreal len, qreal gap, qreal width, QPainter* painter);

  qreal resizeHandleWidth_;
  qreal resizeHandleLength_;
  qreal resizeHandleGap_;
  unsigned int resizeFlags_;
  bool showResizeHandle_;

  enum {Left = 0x1, Top = 0x2, Right = 0x4, Bottom = 0x8};
};



#endif
