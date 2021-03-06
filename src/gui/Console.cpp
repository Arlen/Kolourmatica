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
#include "View.hpp"
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

  _cs = {
    new XYZ, new xyY, new Luv, new LCHuv, new Lab, new LCHab, new AdobeRGB,
    new AppleRGB, new BestRGB, new BetaRGB, new BruceRGB, new CIERGB,
    new ColorMatchRGB, new DonRGB4, new ECIRGB, new EktaSpacePS5, new NTSCRGB,
    new PAL_SECAMRGB, new ProPhotoRGB, new SMPTE_CRGB, new sRGB,
    new WideGamutRGB
  };

  _am = {
    AdaptationMethod<Real>::_XYZScaling,
    AdaptationMethod<Real>::_VonKries,
    AdaptationMethod<Real>::_Bradford
  };

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

  for(Illuminant* rw : _rw)
    delete rw;

  for(BaseColourSpace* cs : _cs)
    delete cs;

  delete _view;
  delete _scene;
}

void Console::initWidgets(){

  QVBoxLayout* layoutA = new QVBoxLayout(this);
  layoutA->setContentsMargins(0, 0, 0, 0);
  layoutA->setSpacing(0);

  /* adding views. */
  setupViews(layoutA);

  /* adding view buttons. */
  QHBoxLayout* layoutB1 = new QHBoxLayout;
  setupViewButtons(layoutB1);
  layoutA->addLayout(layoutB1);

  /* adding controls. */
  QWidget* widget = new QWidget;
  QGridLayout* layoutB2 = new QGridLayout(widget);
  setupControls(layoutB2);
  layoutA->addWidget(widget);
  layoutA->setAlignment(widget, Qt::AlignHCenter);

  clearInput();
  clearOutput();
}

void Console::setupViews(QVBoxLayout* layout){

  _scene = new QGraphicsScene;
  _scene->setSceneRect(-1000, -1000, 2000, 2000);
  _view = new QGraphicsView(_scene);
  _view->setFrameShape(QFrame::NoFrame);
  _view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  _view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  _view->setDragMode(QGraphicsView::ScrollHandDrag);
  _view->setBackgroundBrush(QBrush(QColor(32, 32, 32, 255)));
  _frontView = new View(View::Side::Front);
  _frontView->setVisible(false);
  _leftView = new View(View::Side::Left);
  _leftView->setVisible(false);
  _rightView = new View(View::Side::Right);
  _rightView->setVisible(false);
  _topView = new View(View::Side::Top);
  _topView->setVisible(false);
  _bottomView = new View(View::Side::Bottom);
  _bottomView->setVisible(false);
  _scene->addItem(_frontView);
  _scene->addItem(_leftView);
  _scene->addItem(_rightView);
  _scene->addItem(_topView);
  _scene->addItem(_bottomView);
  layout->addWidget(_view);
}

void Console::setupViewButtons(QHBoxLayout* layout){

  _front = new QPushButton("Front");
  _front->setDisabled(true);
  _left = new QPushButton("Left");
  _left->setDisabled(true);
  _right = new QPushButton("Right");
  _right->setDisabled(true);
  _top = new QPushButton("Top");
  _top->setDisabled(true);
  _bottom = new QPushButton("Bottom");
  _bottom->setDisabled(true);
  layout->addWidget(_front);
  layout->addWidget(_left);
  layout->addWidget(_right);
  layout->addWidget(_top);
  layout->addWidget(_bottom);
  connect(_front, SIGNAL(clicked()), this, SLOT(frontView()));
  connect(_left, SIGNAL(clicked()), this, SLOT(leftView()));
  connect(_right, SIGNAL(clicked()), this, SLOT(rightView()));
  connect(_top, SIGNAL(clicked()), this, SLOT(topView()));
  connect(_bottom, SIGNAL(clicked()), this, SLOT(bottomView()));
}

void Console::setupControls(QGridLayout* layout){

  layout->setContentsMargins(0, 2, 0, 2);
  layout->setSpacing(1);

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
  connect(get<4>(_input), SIGNAL(clicked()), this, SLOT(compute()));
  layout->addItem(new QSpacerItem(128, 1,
				  QSizePolicy::MinimumExpanding,
				  QSizePolicy::Ignored), 0, 0);
  layout->addWidget(label1, 0, 1);
  layout->addWidget(get<0>(_input), 0, 2);
  layout->addWidget(get<1>(_input), 0, 3);
  layout->addWidget(get<2>(_input), 0, 4);
  layout->addWidget(get<3>(_input), 0, 5);
  layout->addWidget(get<4>(_input), 0, 6);
  layout->addItem(new QSpacerItem(128, 1,
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
  layout->addItem(new QSpacerItem(128, 1,
				  QSizePolicy::MinimumExpanding,
				  QSizePolicy::Ignored), 1, 0);
  layout->addWidget(label2, 1, 1);
  layout->addWidget(get<0>(_output), 1, 2);
  layout->addWidget(get<1>(_output), 1, 3);
  layout->addWidget(get<2>(_output), 1, 4);
  layout->addWidget(get<3>(_output), 1, 5);
  layout->addItem(new QSpacerItem(128, 1,
				  QSizePolicy::MinimumExpanding,
				  QSizePolicy::Ignored), 1, 6);

  /* adding colour space names to the QComboBoxes in input and output lines. */
  for(const char* s : {
      "XYZ", "xyY", "Luv", "LCHuv", "Lab", "LCHab", "Adobe RGB", "Apple RGB",
	"Best RGB", "Beta RGB", "Bruce RGB", "CIE RGB",	"ColorMatch RGB",
	"Don RGB 4", "ECI RGB", "Ekta Space PS5", "NTSC RGB", "PAL/SECAM RGB",
	"ProPhoto RGB", "SMPTE-C RGB", "sRGB","Wide Gamut RGB"
	}){

    get<0>(_input)->addItem(s);
    get<0>(_output)->addItem(s);
  }
  get<0>(_input)->setCurrentIndex(_fromIndex);
  connect(get<0>(_input), SIGNAL(currentIndexChanged(int)),
	  this, SLOT(setFrom(int)));

  for(View* view : {_frontView, _leftView, _rightView, _topView, _bottomView})
    connect(get<0>(_input), SIGNAL(currentIndexChanged(int)),
	    view, SLOT(setFrom(int)));

  get<0>(_output)->setCurrentIndex(_toIndex);
  connect(get<0>(_output), SIGNAL(currentIndexChanged(int)),
	  this, SLOT(setTo(int)));

  for(View* view : {_frontView, _leftView, _rightView, _topView, _bottomView})
    connect(get<0>(_output), SIGNAL(currentIndexChanged(int)),
	    view, SLOT(setTo(int)));


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
  layout->addItem(new QSpacerItem(128, 1,
				  QSizePolicy::MinimumExpanding,
				  QSizePolicy::Ignored), 2, 0);
  layout->addWidget(label3, 2, 1);
  layout->addWidget(_comboBox1, 2, 2);
  layout->addWidget(_comboBox2, 2, 3);
  layout->addItem(new QSpacerItem(128, 1,
				  QSizePolicy::MinimumExpanding,
				  QSizePolicy::Ignored), 2, 6);

  /* destination reference white line. */
  layout->addItem(new QSpacerItem(128, 1,
				  QSizePolicy::MinimumExpanding,
				  QSizePolicy::Ignored), 3, 0);
  layout->addWidget(label4, 3, 1);
  layout->addWidget(_comboBox3, 3, 2);
  layout->addWidget(_comboBox4, 3, 3);
  layout->addItem(new QSpacerItem(128, 1,
				  QSizePolicy::MinimumExpanding,
				  QSizePolicy::Ignored), 3, 6);

  /* chromatic adaptation line. */
  layout->addItem(new QSpacerItem(128, 1,
				  QSizePolicy::MinimumExpanding,
				  QSizePolicy::Ignored), 4, 0);
  layout->addWidget(label5, 4, 1);
  layout->addWidget(comboBox5, 4, 2);
  layout->addItem(new QSpacerItem(128, 1,
				  QSizePolicy::MinimumExpanding,
				  QSizePolicy::Ignored), 4, 6);

  for(const char* s : {
      "A", "B", "C", "D50", "D55", "D65", "D75", "E", "F1", "F2", "F3", "F4",
	"F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12"
	}){

    _comboBox1->addItem(s);
    _comboBox3->addItem(s);
  }

  _comboBox1->setCurrentIndex(_srwIndex);
  connect(_comboBox1, SIGNAL(currentIndexChanged(int)),
	  this, SLOT(setSrcRefWhite(int)));

  for(View* view : {_frontView, _leftView, _rightView, _topView, _bottomView})
    connect(_comboBox1, SIGNAL(currentIndexChanged(int)),
	    view, SLOT(setSrcRefWhite(int)));

  _comboBox3->setCurrentIndex(_drwIndex);
  connect(_comboBox3, SIGNAL(currentIndexChanged(int)),
	  this, SLOT(setDstRefWhite(int)));

  for(View* view : {_frontView, _leftView, _rightView, _topView, _bottomView})
    connect(_comboBox3, SIGNAL(currentIndexChanged(int)),
	    view, SLOT(setDstRefWhite(int)));

  _comboBox2->addItem(" 1931 2" + QString(QChar(730)));
  _comboBox2->addItem(" 1964 10" + QString(QChar(730)));
  _comboBox2->setCurrentIndex(_srcObsIndex);
  connect(_comboBox2, SIGNAL(currentIndexChanged(int)),
	  this, SLOT(setSrcObserver(int)));

  for(View* view : {_frontView, _leftView, _rightView, _topView, _bottomView})
    connect(_comboBox2, SIGNAL(currentIndexChanged(int)),
	    view, SLOT(setSrcObserver(int)));

  _comboBox4->addItem(" 1931 2" + QString(QChar(730)));
  _comboBox4->addItem(" 1964 10" + QString(QChar(730)));
  _comboBox4->setCurrentIndex(_dstObsIndex);
  connect(_comboBox4, SIGNAL(currentIndexChanged(int)),
	  this, SLOT(setDstObserver(int)));

  for(View* view : {_frontView, _leftView, _rightView, _topView, _bottomView})
    connect(_comboBox4, SIGNAL(currentIndexChanged(int)),
	    view, SLOT(setDstObserver(int)));

  for(const char* s : {"XYZ Scaling", "Von Kries", "Bradford"} )
    comboBox5->addItem(s);

  comboBox5->setCurrentIndex(_camIndex);
  connect(comboBox5, SIGNAL(currentIndexChanged(int)),
	  this, SLOT(setAdaptationMethod(int)));

  for(View* view : {_frontView, _leftView, _rightView, _topView, _bottomView})
    connect(comboBox5, SIGNAL(currentIndexChanged(int)),
	    view, SLOT(setAdaptationMethod(int)));
}

void Console::doCompute(){

  // Matrix3 CAM = chromaticAdaptationMatrix(SRC_RW, DST_RW, METHOD);
  // XYZ.from(SRC, SRC_RW);
  // XYZ = CAM * XYZ
  // SRC.from(XYZ, DST_RW);
  // SRC 

  int srwIndex = _srwIndex + _srcObsIndex;
  int drwIndex = _drwIndex + _dstObsIndex;
  const Illuminant* srw = nullptr;
  const Illuminant* drw = nullptr;
  XYZ xyz;
  Vector3 tri;

  tri(0) = get<1>(_input)->text().toFloat();
  tri(1) = get<2>(_input)->text().toFloat();
  tri(2) = get<3>(_input)->text().toFloat();

  if(_fromIndex >= 0 and _fromIndex <= 5)
    srw = _rw[srwIndex];
  else
    _cs[_fromIndex]->referenceWhite(srw);


  if(_toIndex >= 0 and _toIndex <= 5)
    drw = _rw[drwIndex];
  else
    _cs[_toIndex]->referenceWhite(drw);

  _cs[_fromIndex]->coords() = tri;
  xyz.coords() = _cs[_fromIndex]->to_XYZ(srw);
  Matrix3 cam = chromaticAdaptationMatrix(*srw, *drw, _am[_camIndex]);
  xyz.coords() = cam * xyz.coords();
  _cs[_toIndex]->from_XYZ(xyz.coords(), drw);
  tri = _cs[_toIndex]->coords();

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

  /*
    TODO:
    Right before disabling the comboboxes, set the comboboxes to the current
    source ref white.
   */
  _comboBox1->setDisabled(disable);
  _comboBox2->setDisabled(disable);
}

void Console::setDstRefWhiteDisabled(bool disable){

  /* if reference white is disabled, so is the observer year. */

  /*
    TODO:
    ditto
  */
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

  if(_toIndex >= 6 and _toIndex <= 21){

    setDstRefWhiteDisabled(true);
    _front->setDisabled(false);
    _left->setDisabled(false);
    _right->setDisabled(false);
    _top->setDisabled(false);
    _bottom->setDisabled(false);

  }else{

    setDstRefWhiteDisabled(false);

    if(_frontView->isVisible()) frontView();
    if(_leftView->isVisible()) leftView();
    if(_rightView->isVisible()) rightView();
    if(_topView->isVisible()) topView();
    if(_bottomView->isVisible()) bottomView();

    _front->setDisabled(true);
    _left->setDisabled(true);
    _right->setDisabled(true);
    _top->setDisabled(true);
    _bottom->setDisabled(true);
  }
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

  clearOutput();
  _camIndex = index;
}

void Console::frontView(){

  if(_front->text() == QString("Front")){
    _front->setText("::::::::  FRONT  ::::::::");
    _frontView->setVisible(true);
  }else{
    _front->setText("Front");
    _frontView->setVisible(false);
  }
}

void Console::leftView(){

  if(_left->text() == QString("Left")){
    _left->setText("::::::::  LEFT ::::::::");
    _leftView->setVisible(true);
  }else{
    _left->setText("Left");
    _leftView->setVisible(false);
  }
}

void Console::rightView(){

  if(_right->text() == QString("Right")){
    _right->setText("::::::::  RIGHT  ::::::::");
    _rightView->setVisible(true);
  }else{
    _right->setText("Right");
    _rightView->setVisible(false);
  }
}

void Console::topView(){

  if(_top->text() == QString("Top")){
    _top->setText("::::::::  TOP  ::::::::");
    _topView->setVisible(true);
  }else{
    _top->setText("Top");
    _topView->setVisible(false);
  }
}

void Console::bottomView(){

  if(_bottom->text() == QString("Bottom")){
    _bottom->setText("::::::::  BOTTOM  ::::::::");
    _bottomView->setVisible(true);
  }else{
    _bottom->setText("Bottom");
    _bottomView->setVisible(false);
  }
}

void Console::compute(){ doCompute(); }
