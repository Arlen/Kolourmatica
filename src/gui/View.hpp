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


#ifndef VIEW_HPP
#define VIEW_HPP

#include "Typedefs.hpp"

#include <QtGui/QGraphicsWidget>
#include <QtCore/QThread>
#include <QtCore/QTimer>
#include <QtCore/QStack>

#include <Eigen/Core>

#include <vector>

using Eigen::Matrix;
using std::vector;

class Renderer;


struct Camera{

  Real& left(){ return _data[_leftIndex]; }
  Real& right(){ return _data[_rightIndex]; }
  Real& top(){ return _data[_topIndex]; }
  Real& bottom(){ return _data[_bottomIndex]; }
  Real& front(){ return _data[_frontIndex]; }
  Real& back(){ return _data[_backIndex]; }
  Real& rayDir(){ return _data[_rayDirIndex]; }

  Real _data[7];
  unsigned _leftIndex;
  unsigned _rightIndex;
  unsigned _topIndex;
  unsigned _bottomIndex;
  unsigned _frontIndex;
  unsigned _backIndex;
  unsigned _rayDirIndex;
  unsigned _x;
  unsigned _y;
  unsigned _z;
  std::function<bool (const Real)> _pred;
};


class View : public QGraphicsWidget{

  Q_OBJECT

  typedef Matrix<Real, 3, 3> Matrix3;
  typedef Matrix<Real, 3, 1> Vector3;
  typedef ColourSpace<Real, Vector3> BaseColourSpace;

signals:
  void renderingStarted();
  void renderingStopped();
  void viewSizeChanged();
  void saved();

public:
  enum class Side { Front, Left, Right, Top, Bottom };

  View(Side side);
  // ~View
  void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);

public slots:
  void setFrom(int index);
  void setTo(int index);
  void setSrcRefWhite(int index);
  void setDstRefWhite(int index);
  void setSrcObserver(int index);
  void setDstObserver(int index);
  void setAdaptationMethod(int index);

protected:
  void mousePressEvent(QGraphicsSceneMouseEvent* event);
  void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
  void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);
  void hoverLeaveEvent(QGraphicsSceneHoverEvent* event);
  void hoverMoveEvent (QGraphicsSceneHoverEvent* event);

private:
  enum class Region { None, Top, Bottom, Left, Right, TopLeft, TopRight,
      BottomLeft, BottomRight };

  int idealThreadCount() const;
  void resetView(qreal w, qreal h);
  void setupCamera(int fromIndex);
  void paintResizeHandle(qreal len, qreal gap, qreal width, QPainter* painter);
  void restartRendering();

  uchar* _activeBuffer;
  qreal _resizeHandleOffsetX;
  qreal _resizeHandleOffsetY;
  qreal _resizeHandleThickness;
  qreal _resizeHandleLength;
  qreal _resizeHandleGap;
  int _imageWidth;
  int _imageHeight;
  int _fromIndex;
  int _toIndex;
  int _srwIndex;
  int _drwIndex;
  int _srcObsIndex;
  int _dstObsIndex;
  int _camIndex;
  Camera _camera;

  QStack<bool> _threads;
  QImage _renderedImage;
  QTimer _timer;
  bool _showResizeHandle;
  bool _dirty;
  bool _showImage;
  Region _region;
  Side _side;

private slots:
  void updateView();
  void saveToImage();
  void startStopRendering();
  void render();
  void setViewSize();
  void deleteThread(QThread* thread);
};


class Thread : public QThread{

Q_OBJECT

signals:
  void expired(QThread* thread);

public:
  Thread();

private slots:
  void deleteMe();
};


class Renderer : public QObject{

Q_OBJECT

  typedef Matrix<Real, 3, 3> Matrix3;
  typedef Matrix<Real, 3, 1> Vector3;
  typedef ColourSpace<Real, Vector3> BaseColourSpace;

signals:
  void dispatched(int oldY, int y, int width, int height);
  void rendered();
  void finished();

public:
  Renderer(uchar* const buffer, Camera camera, int imageHeight,
	   int fromIndex, int toIndex, int camIndex, int y, int width, int height, int srwIndex,
	   int drwIndex);
  ~Renderer();

public slots:
  void dispatch();
  void render(int oldY, int y, int width, int height);
  void abort();

private:
  uchar* _buffer;
  Camera _camera;
  const Illuminant* _srw;
  const Illuminant* _drw;
  int _imageHeight;
  int _fromIndex;
  int _toIndex;
  int _camIndex;
  int _oldY;
  int _y;
  int _width;
  int _height;
  Matrix3 _cam;
  vector<BaseColourSpace*> _cs;
  vector<Illuminant*> _rw;
  bool _abort;
};

#endif
