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


#include "Console.hpp"
#include "ChromaticAdaptation.hpp"

#include <QtGui/QGraphicsScene>
#include <QtGui/QGraphicsView>
#include <QtGui/QPushButton>
#include <QtGui/QDoubleValidator>
#include <QtGui/QLineEdit>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QGridLayout>
#include <QtGui/QLabel>
#include <QtGui/QComboBox>

#include <list>

using std::list;
using std::string;
using std::get;


Console::Console() : QWidget(){

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

  //pView_ = 0;
  _view = nullptr;
  _scene = nullptr;

  _fromIndex = 0;
  _toIndex = 1;
  _srwIndex = 0;
  _drwIndex = 0;
  _srcObsIndex = 0;
  _dstObsIndex = 0;
  _camIndex = 2;
  initWidgets();
  setAdaptationMethod(_camIndex);
}

Console::~Console(){

  for(Illuminant* i : _rw)
    delete i;

  delete _view;
  delete _scene;
}

void Console::initWidgets(){

  QVBoxLayout* layoutA = new QVBoxLayout(this);
  layoutA->setContentsMargins(0, 0, 0, 0);
  layoutA->setSpacing(0);

  /* adding view. */
  _scene = new QGraphicsScene;
  _view = new QGraphicsView(_scene);
  _view->setFrameShape(QFrame::NoFrame);
  _view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  _view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  _view->setDragMode(QGraphicsView::ScrollHandDrag);
  _view->setBackgroundBrush(QBrush(QColor(127, 127, 127, 255)));
  //pViewport_ = new Viewport;
  //_scene->addItem(pViewport_);
  layoutA->addWidget(_view);


  /* adding view buttons. */
  _front = new QPushButton("Front");
  _left = new QPushButton("Left");
  _right = new QPushButton("Right");
  _top = new QPushButton("Top");
  _bottom = new QPushButton("Bottom");
  QHBoxLayout* layoutB1 = new QHBoxLayout;
  layoutB1->addWidget(_front);
  layoutB1->addWidget(_left);
  layoutB1->addWidget(_right);
  layoutB1->addWidget(_top);
  layoutB1->addWidget(_bottom);
  connect(_front, SIGNAL(clicked()), this, SLOT(frontView()));
  connect(_left, SIGNAL(clicked()), this, SLOT(leftView()));
  connect(_right, SIGNAL(clicked()), this, SLOT(rightView()));
  connect(_top, SIGNAL(clicked()), this, SLOT(topView()));
  connect(_bottom, SIGNAL(clicked()), this, SLOT(bottomView()));
  layoutA->addLayout(layoutB1);


  QWidget* widget = new QWidget;
  QGridLayout* layoutB2 = new QGridLayout(widget);
  layoutB2->setContentsMargins(0, 2, 0, 2);
  layoutB2->setSpacing(1);

  QLabel* label1 = new QLabel(" From ");
  QLabel* label2 = new QLabel(" To ");

  /* the input line. */
  get<0>(_input) = new QComboBox;
  get<0>(_input)->setFixedSize(144, 28);
  get<1>(_input) = new QLineEdit;
  get<1>(_input)->setFixedSize(128, 28);
  setDoubleValidator(get<1>(_input));
  get<2>(_input) = new QLineEdit;
  get<2>(_input)->setFixedSize(128, 28);
  setDoubleValidator(get<2>(_input));
  get<3>(_input) = new QLineEdit;
  get<3>(_input)->setFixedSize(128, 28);
  setDoubleValidator(get<3>(_input));
  get<4>(_input) = new QPushButton("==");
  get<4>(_input)->setFixedSize(32, 28);
  connect(get<4>(_input), SIGNAL(clicked()),
	  this, SLOT(compute()));
  layoutB2->addItem(new QSpacerItem(128, 1,
				    QSizePolicy::MinimumExpanding,
				    QSizePolicy::Ignored), 0, 0);
  layoutB2->addWidget(label1, 0, 1);
  layoutB2->addWidget(get<0>(_input), 0, 2);
  layoutB2->addWidget(get<1>(_input), 0, 3);
  layoutB2->addWidget(get<2>(_input), 0, 4);
  layoutB2->addWidget(get<3>(_input), 0, 5);
  layoutB2->addWidget(get<4>(_input), 0, 6);
  layoutB2->addItem(new QSpacerItem(128, 1,
				    QSizePolicy::MinimumExpanding,
				    QSizePolicy::Ignored), 0, 7);

  /* the output line. */
  get<0>(_output) = new QComboBox;
  get<0>(_output)->setFixedSize(144, 28);
  get<1>(_output) = new QLineEdit;
  get<1>(_output)->setFixedSize(128, 28);
  setDoubleValidator(get<1>(_output));
  get<2>(_output) = new QLineEdit;
  get<2>(_output)->setFixedSize(128, 28);
  setDoubleValidator(get<2>(_output));
  get<3>(_output) = new QLineEdit;
  get<3>(_output)->setFixedSize(128, 28);
  setDoubleValidator(get<3>(_output));
  layoutB2->addItem(new QSpacerItem(128, 1,
				    QSizePolicy::MinimumExpanding,
				    QSizePolicy::Ignored), 1, 0);
  layoutB2->addWidget(label2, 1, 1);
  layoutB2->addWidget(get<0>(_output), 1, 2);
  layoutB2->addWidget(get<1>(_output), 1, 3);
  layoutB2->addWidget(get<2>(_output), 1, 4);
  layoutB2->addWidget(get<3>(_output), 1, 5);
  layoutB2->addItem(new QSpacerItem(128, 1,
				    QSizePolicy::MinimumExpanding,
				    QSizePolicy::Ignored), 1, 6);

  /* adding colour space names to the QComboBoxes in input and output lines. */
  for(string& s :
	list<string>{
	  "XYZ", "xyY", "Luv", "LCHuv", "Lab", "LCHab", "Adobe RGB",
	    "Apple RGB", "Best RGB", "Beta RGB", "Bruce RGB", "CIE RGB",
	    "ColorMatch RGB", "Don RGB 4", "ECI RGB", "Ekta Space PS5",
	    "NTSC RGB", "PAL/SECAM RGB", "ProPhoto RGB", "SMPTE-C RGB", "sRGB",
	    "Wide Gamut RGB"}
      ){

    get<0>(_input)->addItem(s.c_str());
    get<0>(_output)->addItem(s.c_str());
  }
  get<0>(_input)->setCurrentIndex(_fromIndex);
  connect(get<0>(_input), SIGNAL(currentIndexChanged(int)),
	  this, SLOT(setFrom(int)));
  get<0>(_output)->setCurrentIndex(_toIndex);
  connect(get<0>(_output), SIGNAL(currentIndexChanged(int)),
	  this, SLOT(setTo(int)));

  /* the reference white and chromatic adaptation lines. */
  QLabel* label3 = new QLabel(" Source Ref. White ");
  QLabel* label4 = new QLabel(" Destination Ref. White ");
  QLabel* label5 = new QLabel(" Chromatic Adaptation ");
  _comboBox1 = new QComboBox;
  _comboBox1->setFixedSize(144, 28);
  _comboBox2 = new QComboBox;
  _comboBox2->setFixedSize(92, 28);
  _comboBox3 = new QComboBox;
  _comboBox3->setFixedSize(144, 28);
  _comboBox4 = new QComboBox;
  _comboBox4->setFixedSize(92, 28);
  QComboBox* comboBox5 = new QComboBox;
  comboBox5->setFixedSize(144, 28);

  /* source reference white line. */
  layoutB2->addItem(new QSpacerItem(128, 1,
				    QSizePolicy::MinimumExpanding,
				    QSizePolicy::Ignored), 2, 0);
  layoutB2->addWidget(label3, 2, 1);
  layoutB2->addWidget(_comboBox1, 2, 2);
  layoutB2->addWidget(_comboBox2, 2, 3);
  layoutB2->addItem(new QSpacerItem(128, 1,
				    QSizePolicy::MinimumExpanding,
				    QSizePolicy::Ignored), 2, 6);

  /* destination reference white line. */
  layoutB2->addItem(new QSpacerItem(128, 1,
				    QSizePolicy::MinimumExpanding,
				    QSizePolicy::Ignored), 3, 0);
  layoutB2->addWidget(label4, 3, 1);
  layoutB2->addWidget(_comboBox3, 3, 2);
  layoutB2->addWidget(_comboBox4, 3, 3);
  layoutB2->addItem(new QSpacerItem(128, 1,
				    QSizePolicy::MinimumExpanding,
				    QSizePolicy::Ignored), 3, 6);

  /* chromatic adaptation line. */
  layoutB2->addItem(new QSpacerItem(128, 1,
				    QSizePolicy::MinimumExpanding,
				    QSizePolicy::Ignored), 4, 0);
  layoutB2->addWidget(label5, 4, 1);
  layoutB2->addWidget(comboBox5, 4, 2);
  layoutB2->addItem(new QSpacerItem(128, 1,
				    QSizePolicy::MinimumExpanding,
				    QSizePolicy::Ignored), 4, 6);

  for(string& s :
	list<string>{
	  "A", "B", "C", "D50", "D55", "D65", "D75", "E", "F1", "F2", "F3",
	    "F4", "F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12"}
      ){

    _comboBox1->addItem(s.c_str());
    _comboBox3->addItem(s.c_str());
  }

  _comboBox1->setCurrentIndex(_srwIndex);
  connect(_comboBox1, SIGNAL(currentIndexChanged(int)),
	  this, SLOT(setSrcRefWhite(int)));
  _comboBox3->setCurrentIndex(_drwIndex);
  connect(_comboBox3, SIGNAL(currentIndexChanged(int)),
	  this, SLOT(setDstRefWhite(int)));

  _comboBox2->addItem(" 1931 2" + QString(QChar(730)));
  _comboBox2->addItem(" 1964 10" + QString(QChar(730)));
  _comboBox2->setCurrentIndex(_srcObsIndex);
  connect(_comboBox2, SIGNAL(currentIndexChanged(int)),
	  this, SLOT(setSrcObserver(int)));

  _comboBox4->addItem(" 1931 2" + QString(QChar(730)));
  _comboBox4->addItem(" 1964 10" + QString(QChar(730)));
  _comboBox4->setCurrentIndex(_dstObsIndex);
  connect(_comboBox4, SIGNAL(currentIndexChanged(int)),
	  this, SLOT(setDstObserver(int)));

  for(string& s : list<string>{"XYZ Scaling", "Von Kries", "Bradford"} )
    comboBox5->addItem(s.c_str());

  comboBox5->setCurrentIndex(_camIndex);
  connect(comboBox5, SIGNAL(currentIndexChanged(int)),
	  this, SLOT(setAdaptationMethod(int)));

  layoutA->addWidget(widget);
  layoutA->setAlignment(widget, Qt::AlignHCenter);

  clearInput();
  clearOutput();
}

void Console::doCompute(){

  int srwIndex = _srwIndex + _srcObsIndex;
  int drwIndex = _drwIndex + _dstObsIndex;
  const Illuminant* srw = nullptr;
  const Illuminant* drw = nullptr;
  XYZ xyz;
  Vector3 tri;

  tri(0) = get<1>(_input)->text().toFloat();
  tri(1) = get<2>(_input)->text().toFloat();
  tri(2) = get<3>(_input)->text().toFloat();

  /* over written in the next switch statement if source is any RGB model. */
  srw = _rw[srwIndex];

  switch(_fromIndex){
  case 0:{ xyz.coords() = tri; } break;
  case 1:{ get<1>(_cs).coords() = tri; xyz.from(get<1>(_cs)); } break;
  case 2:{ get<2>(_cs).coords() = tri; xyz.from(get<2>(_cs), *_rw[srwIndex]); } break;
  case 3:{ get<3>(_cs).coords() = tri; xyz.from(get<3>(_cs), *_rw[srwIndex]); } break;
  case 4:{ get<4>(_cs).coords() = tri; xyz.from(get<4>(_cs), *_rw[srwIndex]); } break;
  case 5:{ get<5>(_cs).coords() = tri; xyz.from(get<5>(_cs), *_rw[srwIndex]); } break;
  case 6:{ get<6>(_cs).coords() = tri; xyz.from(get<6>(_cs)); srw = &get<6>(_cs).referenceWhite(); } break;
  case 7:{ get<7>(_cs).coords() = tri; xyz.from(get<7>(_cs)); srw = &get<7>(_cs).referenceWhite(); } break;
  case 8:{ get<8>(_cs).coords() = tri; xyz.from(get<8>(_cs)); srw = &get<8>(_cs).referenceWhite(); } break;
  case 9:{ get<9>(_cs).coords() = tri; xyz.from(get<9>(_cs)); srw = &get<9>(_cs).referenceWhite(); } break;
  case 10:{ get<10>(_cs).coords() = tri; xyz.from(get<10>(_cs)); srw = &get<10>(_cs).referenceWhite(); } break;
  case 11:{ get<11>(_cs).coords() = tri; xyz.from(get<11>(_cs)); srw = &get<11>(_cs).referenceWhite(); } break;
  case 12:{ get<12>(_cs).coords() = tri; xyz.from(get<12>(_cs)); srw = &get<12>(_cs).referenceWhite(); } break;
  case 13:{ get<13>(_cs).coords() = tri; xyz.from(get<13>(_cs)); srw = &get<13>(_cs).referenceWhite(); } break;
  case 14:{ get<14>(_cs).coords() = tri; xyz.from(get<14>(_cs)); srw = &get<14>(_cs).referenceWhite(); } break;
  case 15:{ get<15>(_cs).coords() = tri; xyz.from(get<15>(_cs)); srw = &get<15>(_cs).referenceWhite(); } break;
  case 16:{ get<16>(_cs).coords() = tri; xyz.from(get<16>(_cs)); srw = &get<16>(_cs).referenceWhite(); } break;
  case 17:{ get<17>(_cs).coords() = tri; xyz.from(get<17>(_cs)); srw = &get<17>(_cs).referenceWhite(); } break;
  case 18:{ get<18>(_cs).coords() = tri; xyz.from(get<18>(_cs)); srw = &get<18>(_cs).referenceWhite(); } break;
  case 19:{ get<19>(_cs).coords() = tri; xyz.from(get<19>(_cs)); srw = &get<19>(_cs).referenceWhite(); } break;
  case 20:{ get<20>(_cs).coords() = tri; xyz.from(get<20>(_cs)); srw = &get<20>(_cs).referenceWhite(); } break;
  case 21:{ get<21>(_cs).coords() = tri; xyz.from(get<21>(_cs)); srw = &get<21>(_cs).referenceWhite(); } break;
  }

  /* find out destination reference white. */
  if(_toIndex >= 0 and _toIndex < 6){
    drw = _rw[drwIndex];
  }else{
    switch(_toIndex){
    case 6:{ drw = &get<6>(_cs).referenceWhite(); } break;
    case 7:{ drw = &get<7>(_cs).referenceWhite(); } break;
    case 8:{ drw = &get<8>(_cs).referenceWhite(); } break;
    case 9:{ drw = &get<9>(_cs).referenceWhite(); } break;
    case 10:{ drw = &get<10>(_cs).referenceWhite(); } break;
    case 11:{ drw = &get<11>(_cs).referenceWhite(); } break;
    case 12:{ drw = &get<12>(_cs).referenceWhite(); } break;
    case 13:{ drw = &get<13>(_cs).referenceWhite(); } break;
    case 14:{ drw = &get<14>(_cs).referenceWhite(); } break;
    case 15:{ drw = &get<15>(_cs).referenceWhite(); } break;
    case 16:{ drw = &get<16>(_cs).referenceWhite(); } break;
    case 17:{ drw = &get<17>(_cs).referenceWhite(); } break;
    case 18:{ drw = &get<18>(_cs).referenceWhite(); } break;
    case 19:{ drw = &get<19>(_cs).referenceWhite(); } break;
    case 20:{ drw = &get<20>(_cs).referenceWhite(); } break;
    case 21:{ drw = &get<21>(_cs).referenceWhite(); } break;
    }
  }

  Matrix3 cam = chromaticAdaptationMatrix(*srw, *drw, _adaptMethod);
  xyz.coords() = cam * xyz.coords();

  switch(_toIndex){
  case 0:{ tri = xyz.coords(); } break;
  case 1:{ get<1>(_cs).from(xyz); tri = get<1>(_cs).coords(); } break;
  case 2:{ get<2>(_cs).from(xyz, *_rw[drwIndex]); tri = get<2>(_cs).coords(); } break;
  case 3:{ get<3>(_cs).from(xyz, *_rw[drwIndex]); tri = get<3>(_cs).coords(); } break;
  case 4:{ get<4>(_cs).from(xyz, *_rw[drwIndex]); tri = get<4>(_cs).coords(); } break;
  case 5:{ get<5>(_cs).from(xyz, *_rw[drwIndex]); tri = get<5>(_cs).coords(); } break;
  case 6:{ get<6>(_cs).from(xyz); tri = get<6>(_cs).coords(); } break;
  case 7:{ get<7>(_cs).from(xyz); tri = get<7>(_cs).coords(); } break;
  case 8:{ get<8>(_cs).from(xyz); tri = get<8>(_cs).coords(); } break;
  case 9:{ get<9>(_cs).from(xyz); tri = get<9>(_cs).coords(); } break;
  case 10:{ get<10>(_cs).from(xyz); tri = get<10>(_cs).coords(); } break;
  case 11:{ get<11>(_cs).from(xyz); tri = get<11>(_cs).coords(); } break;
  case 12:{ get<12>(_cs).from(xyz); tri = get<12>(_cs).coords(); } break;
  case 13:{ get<13>(_cs).from(xyz); tri = get<13>(_cs).coords(); } break;
  case 14:{ get<14>(_cs).from(xyz); tri = get<14>(_cs).coords(); } break;
  case 15:{ get<15>(_cs).from(xyz); tri = get<15>(_cs).coords(); } break;
  case 16:{ get<16>(_cs).from(xyz); tri = get<16>(_cs).coords(); } break;
  case 17:{ get<17>(_cs).from(xyz); tri = get<17>(_cs).coords(); } break;
  case 18:{ get<18>(_cs).from(xyz); tri = get<18>(_cs).coords(); } break;
  case 19:{ get<19>(_cs).from(xyz); tri = get<19>(_cs).coords(); } break;
  case 20:{ get<20>(_cs).from(xyz); tri = get<20>(_cs).coords(); } break;
  case 21:{ get<21>(_cs).from(xyz); tri = get<21>(_cs).coords(); } break;
  }

  get<1>(_output)->setText(QString::number( tri(0), 'f', 10));
  get<2>(_output)->setText(QString::number( tri(1), 'f', 10));
  get<3>(_output)->setText(QString::number( tri(2), 'f', 10));
}

void Console::clearInput(){

  get<1>(_input)->setText(QString::number(0.0, 'f', 10));
  get<2>(_input)->setText(QString::number(0.0, 'f', 10));
  get<3>(_input)->setText(QString::number(0.0, 'f', 10));
}

void Console::clearOutput(){

  get<1>(_output)->setText(QString::number(0.0, 'f', 10));
  get<2>(_output)->setText(QString::number(0.0, 'f', 10));
  get<3>(_output)->setText(QString::number(0.0, 'f', 10));
}

void Console::setDoubleValidator(QLineEdit* const lineEdit){

  QDoubleValidator* doubleValidator = new QDoubleValidator(lineEdit);
  doubleValidator->setNotation(QDoubleValidator::StandardNotation);
  doubleValidator->setDecimals(10);
  lineEdit->setValidator(doubleValidator);
}

void Console::setSrcRefWhiteDisabled(bool disable){

  /* if reference white is disabled, so is the observer year. */
  _comboBox1->setDisabled(disable);
  _comboBox2->setDisabled(disable);
}

void Console::setDstRefWhiteDisabled(bool disable){

  /* if reference white is disabled, so is the observer year. */
  _comboBox3->setDisabled(disable);
  _comboBox4->setDisabled(disable);
}

void Console::setFrom(int index){

  clearInput();
  _fromIndex = index;

  if(_fromIndex >= 6 and _fromIndex <= 21)
    setSrcRefWhiteDisabled(true);
  else
    setSrcRefWhiteDisabled(false);
}

void Console::setTo(int index){

  clearOutput();
  _toIndex = index;

  if(_toIndex >= 6 and _toIndex <= 21)
    setDstRefWhiteDisabled(true);
  else
    setDstRefWhiteDisabled(false);
}

void Console::setSrcRefWhite(int index){

  clearOutput();
  _srwIndex = index * 2;
}

void Console::setDstRefWhite(int index){

  clearOutput();
  _drwIndex = index * 2;
}

void Console::setSrcObserver(int index){

  clearOutput();
  _srcObsIndex = index;
}

void Console::setDstObserver(int index){

  clearOutput();
  _dstObsIndex = index;
}

void Console::setAdaptationMethod(int index){

  if(index == 0)
    _adaptMethod = AdaptationMethod<Real>::_XYZScaling;
  else if(index == 1)
    _adaptMethod = AdaptationMethod<Real>::_VonKries;
  else
    _adaptMethod = AdaptationMethod<Real>::_Bradford;
}

void Console::frontView(){

  if(_front->text() == QString("Front")){
    _front->setText("::::::::  FRONT  ::::::::");
    // stop rendering
    // hide front view
  }else{
    _front->setText("Front");
    // show front view
    // start rendering
  }
}

void Console::leftView(){

  if(_left->text() == QString("Left")){
    _left->setText("::::::::  LEFT :::");
    // stop rendering
    // hide front view
  }else{
    _left->setText("Left");
    // show front view
    // start rendering
  }
}

void Console::rightView(){

  if(_right->text() == QString("Right")){
    _right->setText("::::::::  RIGHT  ::::::::");
    // stop rendering
    // hide front view
  }else{
    _right->setText("Right");
    // show front view
    // start rendering
  }
}

void Console::topView(){

  if(_top->text() == QString("Top")){
    _top->setText("::::::::  TOP  ::::::::");
    // stop rendering
    // hide front view
  }else{
    _top->setText("Top");
    // show front view
    // start rendering
  }
}

void Console::bottomView(){

  if(_bottom->text() == QString("Bottom")){
    _bottom->setText("::::::::  BOTTOM  ::::::::");
    // stop rendering
    // hide front view
  }else{
    _bottom->setText("Bottom");
    // show front view
    // start rendering
  }
}

void Console::compute(){ doCompute(); }
