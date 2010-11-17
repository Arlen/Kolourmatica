#ifndef VIEWER_HPP
#define VIEWER_HPP


#include "../core/Convert.hpp"

#include "../../../eigen/Eigen/Core"

using namespace Eigen;

#include <QtGui/QWidget>
#include <QtCore/QThread>

class Viewer;


class RenderThread : public QThread{

public:
  RenderThread(Viewer* pWidget, int id);
  void run();
  void Init();

private:
  void RenderSpace(int w, int h);

  int id_;
  Viewer* pWidget_;
  Input in_;
  Vector3f (* convert_)(const Input&);
};


class Viewer : public QWidget{

public:

  Viewer();
  ~Viewer();
  void SetWorkingColorSpace(int cs);
  void SetSystemColorSpace(int cs);
  void SetReferenceWhite(const Vector3f& rRw);
  void SetAdaptationMethod(const Matrix3f& rAd);

protected:
  void resizeEvent(QResizeEvent* );
  void paintEvent(QPaintEvent* );
  void mousePressEvent(QMouseEvent*);

private:
  void StopAllThreads();
  void UpdateAllThreads();

  friend class RenderThread;

  unsigned char* buffer_;
  RenderThread** threads_;
  int size_;
  int proxy_;
  int threadCount_;

  int wcs_;
  int scs_;
  Vector3f rw_;
  Matrix3f am_;
  Input in_;
  Vector3f (*convert_)(const Input&);
};

#endif 
