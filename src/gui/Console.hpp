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


#ifndef CONSOLE_HPP
#define CONSOLE_HPP

#include "Typedefs.hpp"

#include <QtGui/QWidget>

#include <Eigen/Core>

#include <tuple>
#include <vector>

using namespace Eigen;
using std::tuple;
using std::vector;


class View;

class QComboBox;
class QLineEdit;
class QPushButton;
class QGraphicsScene;
class QGraphicsView;
class QHBoxLayout;
class QVBoxLayout;
class QGridLayout;


class Console : public QWidget{

Q_OBJECT

public:
  Console();
  ~Console();

private:
  typedef tuple<QComboBox*, QLineEdit*, QLineEdit*, QLineEdit*, QPushButton*> Input;
  typedef tuple<QComboBox*, QLineEdit*, QLineEdit*, QLineEdit*> Output;
  typedef Matrix<Real, 3, 3> Matrix3;
  typedef Matrix<Real, 3, 1> Vector3;
  typedef ColourSpace<Real, Vector3> BaseColourSpace;

  void initWidgets();
  void setupViews(QVBoxLayout* layout);
  void setupViewButtons(QHBoxLayout* layout);
  void setupControls(QGridLayout* layout);
  void doCompute();
  void clearInput();
  void clearOutput();
  void setDoubleValidator(QLineEdit* const lineEdit);
  void setSrcRefWhiteDisabled(bool disable);
  void setDstRefWhiteDisabled(bool disable);

  QComboBox* _comboBox1;
  QComboBox* _comboBox2;
  QComboBox* _comboBox3;
  QComboBox* _comboBox4;
  QPushButton* _front;
  QPushButton* _left;
  QPushButton* _right;
  QPushButton* _top;
  QPushButton* _bottom;
  View* _frontView;
  View* _leftView;
  View* _rightView;
  View* _topView;
  View* _bottomView;

  ColourSpace<Real, Vector3>* _result;
  QGraphicsScene* _scene;
  QGraphicsView* _view;
  int _fromIndex;
  int _toIndex;
  int _srwIndex;
  int _drwIndex;
  int _srcObsIndex;
  int _dstObsIndex;
  int _camIndex;
  Input _input;
  Output _output;
  vector<BaseColourSpace*> _cs;
  vector<Illuminant*> _rw;
  vector<Matrix3> _am;

private slots:
  void setFrom(int index);
  void setTo(int index);
  void setSrcRefWhite(int index);
  void setDstRefWhite(int index);
  void setSrcObserver(int index);
  void setDstObserver(int index);
  void setAdaptationMethod(int index);
  void frontView();
  void leftView();
  void rightView();
  void topView();
  void bottomView();
  void compute();
};

#endif
