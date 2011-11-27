#ifndef VIEW_HPP
#define VIEW_HPP

#include "Typedefs.hpp"

#include <QtGui/QGraphicsWidget>
#include <QtCore/QTimer>
#include <QtCore/QTime>
#include <QtCore/QSemaphore>

#include <Eigen/Core>

#include <tuple>
#include <utility>
#include <vector>
#include <list>
#include <map>

using namespace Eigen;
using std::tuple;
using std::pair;
using std::vector;
using std::list;
using std::map;

class Dispatcher;
class Renderer;
class QThread;
class QImage;

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
  void renderingAborted();

public:
  enum class Side { Front, Left, Right, Top, Bottom };

  View(Side side);
  // ~View need to delete all the running threads if any
  void setFrom(int index);
  void setTo(int index);
  void setSrcRefWhite(int index);
  void setDstRefWhite(int index);
  void setSrcObserver(int index);
  void setDstObserver(int index);
  void setAdaptationMethod(int index);
  void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
  uchar* const buffer() const;

public slots:
  void saveToImage(uchar*);

protected:
  void mousePressEvent(QGraphicsSceneMouseEvent* event);
  void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
  void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);
  void hoverLeaveEvent(QGraphicsSceneHoverEvent* event);
  void hoverMoveEvent (QGraphicsSceneHoverEvent* event);

private:
  enum class Region { None, Top, Bottom, Left, Right, TopLeft, TopRight, BottomLeft, BottomRight };

  typedef map<QThread*, Dispatcher*> Dispatchers;
  typedef map<QThread*, Renderer*> Renderers;

  typedef pair<QThread*, Dispatcher*> DispatchUnit;
  typedef pair<QThread*, Renderer*> RenderUnit;
  typedef pair<DispatchUnit, RenderUnit> WorkUnit;
  typedef list<WorkUnit> WorkUnits;
  typedef map<uchar* const, WorkUnits> WorkArea;

  WorkArea _workArea;

  int idealThreadCount() const;
  void resetView(qreal w, qreal h);
  void setupCamera(int fromIndex);
  void paintResizeHandle(qreal len, qreal gap, qreal width, QPainter* painter);
  void render();
  WorkArea::iterator createWorkArea();
  void abortRendering();
  bool isRenderingReady();
  bool isRendererRunning();

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

  Dispatchers _dispatchers;
  Renderers _renderers;

  QImage _renderedImage;
  QTimer _timer;
  QSemaphore* _bufSem;
  bool _showResizeHandle;
  bool _dirty;
  bool _showImage;
  Region _region;
  Side _side;

public:
  uchar* _activeBuffer;

private slots:
  void updateView();
  void resetRenderer();
};


class Dispatcher : public QObject{

Q_OBJECT

  typedef Matrix<Real, 3, 3> Matrix3;
  typedef Matrix<Real, 3, 1> Vector3;
  typedef ColourSpace<Real, Vector3> BaseColourSpace;

signals:
  void dispatched(int oldY, int y, int width, int height);
  void finished();

public:
  Dispatcher(int y, int width, int height);

public slots:
  void dispatch();
  void abortDispatching();

private:
  int _oldY;
  int _y;
  int _width;
  int _height;
  bool _abort;
};


class Renderer : public QObject{

Q_OBJECT

  typedef Matrix<Real, 3, 3> Matrix3;
  typedef Matrix<Real, 3, 1> Vector3;
  typedef ColourSpace<Real, Vector3> BaseColourSpace;

signals:
  void wroteToBuffer(uchar*);
  void rendered();
  void finished();

public:
  Renderer(uchar* const buffer, Camera* camera, int imageHeight,
	   int fromIndex, int toIndex, int camIndex, int srwIndex,
	   int drwIndex);
  ~Renderer();

public slots:
  void render(int oldY, int y, int width, int height);
  void abortRendering();

private:
  uchar* _buffer;
  Camera* _camera;
  const Illuminant* _srw;
  const Illuminant* _drw;
  int _imageHeight;
  int _fromIndex;
  int _toIndex;
  int _camIndex;
  Matrix3 _cam;
  vector<BaseColourSpace*> _cs;
  vector<Illuminant*> _rw;
  bool _abort;
};


#endif
